<?php
session_start();
require "/home1/autotime/public_html/globals/config.inc";

// Debugging
ini_set('display_errors', 1);
error_reporting(E_ALL);
file_put_contents('/home1/autotime/public_html/debug_index.log', "Accessing index.php at " . date('Y-m-d H:i:s') . "\n", FILE_APPEND);

if (!isset($_SESSION['user_id'])) {
    file_put_contents('/home1/autotime/public_html/debug_index.log', "No user_id, redirecting to login.php\n", FILE_APPEND);
    header("Location: /login.php");
    exit;
}

$user_id = $_SESSION['user_id'];
$error = $_GET['error'] ?? '';
$success = $_GET['success'] ?? '';
$tasks = [];
$events = [];
$categories = [];

// Fetch user time zone from preferences
$time_zone = 'UTC';
try {
    $stmt = $pdo->prepare("SELECT time_zone FROM preferences WHERE user_id = ?");
    $stmt->execute([$user_id]);
    $pref = $stmt->fetch(PDO::FETCH_ASSOC);
    $time_zone = $pref['time_zone'] ?? 'UTC';
} catch (PDOException $e) {
    file_put_contents('/home1/autotime/public_html/debug_index.log', 
        "Error fetching time zone: " . $e->getMessage() . ", defaulting to UTC\n", FILE_APPEND);
}

// Set local time zone
date_default_timezone_set($time_zone);
$local_datetime = new DateTime();

if ($_SERVER['REQUEST_METHOD'] === 'POST' && isset($_POST['delete_done'])) {
    try {
        $stmt = $pdo->prepare("DELETE FROM tasks WHERE user_id = ? AND completed = TRUE");
        $stmt->execute([$user_id]);
        $success = "All completed tasks deleted.";
    } catch (PDOException $e) {
        $error = "Error deleting completed tasks: " . htmlspecialchars($e->getMessage());
    }
}

if ($_SERVER['REQUEST_METHOD'] === 'POST' && isset($_POST['clean_database'])) {
    try {
        $stmt = $pdo->prepare("DELETE FROM events WHERE user_id = ? AND date < ?");
        $stmt->execute([$user_id, date('Y-m-d')]);
        $stmt = $pdo->prepare("DELETE FROM tasks WHERE user_id = ? AND date < ? AND scheduled = TRUE");
        $stmt->execute([$user_id, date('Y-m-d')]);
        $success = "Old events and tasks deleted.";
    } catch (PDOException $e) {
        $error = "Error cleaning database: " . htmlspecialchars($e->getMessage());
    }
}

try {
    // Fetch tasks (include completed)
    $stmt = $pdo->prepare("
        SELECT t.id, t.title, t.time_hrs, t.time_mins, t.category_id, t.completed, c.title AS category_name
        FROM tasks t
        LEFT JOIN categories c ON t.category_id = c.id
        WHERE t.user_id = ?
        ORDER BY c.title, t.title
    ");
    $stmt->execute([$user_id]);
    $tasks = $stmt->fetchAll(PDO::FETCH_ASSOC);

    // Fetch events
    $stmt = $pdo->prepare("
        SELECT id, title, date, start_time 
        FROM events 
        WHERE user_id = ? AND date >= ?
        ORDER BY date, start_time
    ");
    $stmt->execute([$user_id, date('Y-m-d')]);
    $events = $stmt->fetchAll(PDO::FETCH_ASSOC);

    // Fetch categories
    $stmt = $pdo->prepare("SELECT id, title FROM categories WHERE user_id = ?");
    $stmt->execute([$user_id]);
    $categories = $stmt->fetchAll(PDO::FETCH_ASSOC);
} catch (PDOException $e) {
    $error = "Error loading dashboard: " . htmlspecialchars($e->getMessage());
}
?>

<h1>Dashboard</h1>
<?php if ($error): ?>
    <p style="color: red;"><?php echo htmlspecialchars($error); ?></p>
<?php endif; ?>
<?php if ($success): ?>
    <p style="color: green;"><?php echo htmlspecialchars($success); ?></p>
<?php endif; ?>

<!-- Top Buttons -->
<p>
    <a href="/add_task.php">Add Task</a> | 
    <a href="/add_event.php">Add Event</a> | 
    <a href="/schedule_view.php">View Full Schedule</a>
</p>
<form method="post" action="/schedule_tasks.php" target="_blank">
    <p>
        <label>Schedule Start Date:</label>
        <input type="date" name="target_date" value="<?php echo $local_datetime->format('Y-m-d'); ?>">
        <label>Time:</label>
        <input type="time" name="target_time" value="<?php echo $local_datetime->format('H:i'); ?>">
        <input type="submit" value="Schedule Now">
    </p>
</form>

<!-- Tasks by Category -->
<?php if (!empty($tasks)): ?>
    <h2>Tasks</h2>
    <?php
    $current_category = '';
    foreach ($tasks as $task) {
        $category = $task['category_name'] ?: 'Uncategorized';
        if ($category !== $current_category) {
            if ($current_category !== '') {
                echo '</ul>';
            }
            echo '<h3>' . htmlspecialchars($category) . '</h3><ul>';
            $current_category = $category;
        }
        $duration = sprintf("%dh %dmin", $task['time_hrs'], $task['time_mins']);
        $completed = $task['completed'] ? ' [Completed]' : '';
        echo '<li>' . htmlspecialchars($duration) . ' – ' . 
             htmlspecialchars($task['title']) . $completed . 
             ' [<a href="/view_task.php?id=' . (int)$task['id'] . '">View</a>] ' . 
             ' [<a href="/edit_task.php?id=' . (int)$task['id'] . '">Edit</a>]</li>';
    }
    if (!empty($tasks)) {
        echo '</ul>';
    }
    ?>
<?php else: ?>
    <p>No tasks available.</p>
<?php endif; ?>

<!-- Events by Date/Time -->
<?php if (!empty($events)): ?>
    <h2>Events</h2>
    <ul>
        <?php foreach ($events as $event): ?>
            <li><?php echo date('d-m-Y', strtotime($event['date'])) . ' ' . 
                        htmlspecialchars($event['start_time']) . ' – ' . 
                        htmlspecialchars($event['title']); ?>
                [<a href="/view_event.php?id=<?php echo (int)$event['id']; ?>">View</a>]
            </li>
        <?php endforeach; ?>
    </ul>
<?php else: ?>
    <p>No events scheduled.</p>
<?php endif; ?>

<!-- Bottom Buttons -->
<p>
    <a href="/preferences.php">Preferences</a> | 
    <a href="/warnings.php">View Warnings</a> | 
    <form method="post" style="display: inline;">
        <input type="submit" name="delete_done" value="Delete All Done Tasks">
    </form> | 
    <form method="post" style="display: inline;">
        <input type="submit" name="clean_database" value="Clean Database">
    </form> | 
    <a href="/logout.php">Logout</a>
</p>

<style>
    body { font-family: Arial, sans-serif; margin: 20px; }
    a, input[type="submit"] { margin-right: 10px; }
    ul { list-style-type: none; padding-left: 20px; }
    li { margin: 5px 0; }
    h2, h3 { margin-top: 20px; }
    form { display: inline-block; margin-right: 10px; }
</style>