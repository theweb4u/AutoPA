<?php
ob_start();
require "/home1/autotime/public_html/globals/config.inc";
if (!isset($_SESSION['user_id'])) {
    header("Location: /index.php");
    exit;
}

$user_id = $_SESSION['user_id'];
$success = '';
$error = '';

if (isset($_GET['success'])) {
    $success = htmlspecialchars($_GET['success']);
}
if (isset($_GET['error'])) {
    $error = htmlspecialchars($_GET['error']);
}

try {
    $stmt = $pdo->prepare("
        SELECT t.*, c.title AS category_title, sc.title AS sub_category_title
        FROM tasks t
        LEFT JOIN categories c ON t.category_id = c.id
        LEFT JOIN categories sc ON t.sub_category_id = sc.id
        WHERE t.user_id = ? 
        ORDER BY 
            CASE 
                WHEN t.urgency = 'urgent' AND t.importance = 'important' THEN 1
                WHEN t.importance = 'important' THEN 2
                WHEN t.urgency = 'urgent' THEN 3
                ELSE 4
            END,
            t.priority ASC, 
            COALESCE(t.due_date, '9999-12-31') ASC
    ");
    $stmt->execute([$user_id]);
    $tasks = $stmt->fetchAll(PDO::FETCH_ASSOC);

    $stmt = $pdo->prepare("
        SELECT id, title, details, date, start_time, end_time, location
        FROM events 
        WHERE user_id = ? 
        ORDER BY date ASC, start_time ASC
    ");
    $stmt->execute([$user_id]);
    $events = $stmt->fetchAll(PDO::FETCH_ASSOC);
} catch (PDOException $e) {
    $error = "Error loading schedule: " . htmlspecialchars($e->getMessage());
}
?>

<h1>Your Schedule</h1>
<?php if ($error): ?>
    <p style="color: red;"><?php echo $error; ?></p>
<?php endif; ?>
<?php if ($success): ?>
    <p style="color: green;"><?php echo $success; ?></p>
<?php endif; ?>

<form method="post" action="/schedule_tasks.php">
    <label>Schedule Start Date: 
        <input type="date" name="target_date" value="<?php echo date('Y-m-d'); ?>" required>
    </label>
    <input type="submit" value="Schedule Now">
</form>
<p><a href="/schedule_view.php" target="_blank">View Full Schedule</a></p>

<h2>Tasks</h2>
<ul>
<?php foreach ($tasks as $task): ?>
    <li>
        <strong><?php echo htmlspecialchars($task['title'] ?? 'Untitled'); ?></strong> 
        [<a href="/edit_task.php?id=<?php echo (int)$task['id']; ?>">Edit</a>] 
        [<a href="/view_task.php?id=<?php echo (int)$task['id']; ?>">View</a>]<br>
        Priority: <?php echo (int)($task['priority'] ?? 3); ?><br>
        Importance: <?php echo ucfirst(str_replace('_', ' ', $task['importance'] ?? 'not_important')); ?><br>
        Urgency: <?php echo ucfirst(str_replace('_', ' ', $task['urgency'] ?? 'not_urgent')); ?><br>
        <?php if (!empty($task['start_time']) && !empty($task['end_time'])): ?>
            Scheduled: <?php echo htmlspecialchars($task['start_time']); ?> 
            to <?php echo htmlspecialchars($task['end_time']); ?>
        <?php else: ?>
            Unscheduled
        <?php endif; ?>
    </li>
<?php endforeach; ?>
</ul>

<h2>Events</h2>
<ul>
<?php foreach ($events as $event): ?>
    <li>
        <strong><?php echo htmlspecialchars($event['title'] ?? 'Untitled'); ?></strong> 
        [<a href="/edit_event.php?id=<?php echo (int)$event['id']; ?>">Edit</a>] 
        [<a href="/view_event.php?id=<?php echo (int)$event['id']; ?>">View</a>] 
        [<a href="/edit_event.php?id=<?php echo (int)$event['id']; ?>&delete=1" onclick="return confirm('Delete this event?');">Delete</a>]<br>
        Date: <?php echo htmlspecialchars($event['date'] ?? ''); ?><br>
        Time: <?php echo htmlspecialchars($event['start_time'] ?? ''); ?> to <?php echo htmlspecialchars($event['end_time'] ?? ''); ?>
    </li>
<?php endforeach; ?>
</ul>

<a href="/add_task.php">Add Task</a> | 
<a href="/add_event.php">Add Event</a> | 
<a href="/preferences.php">Preferences</a> | 
<a href="/logout.php">Logout</a>

<style>
    body { font-family: Arial, sans-serif; margin: 20px; }
    ul { list-style-type: none; padding: 0; }
    li { margin: 20px 0; border: 1px solid #ccc; padding: 10px; }
    a { margin-right: 10px; }
    input[type="submit"], input[type="date"] { padding: 10px; margin: 5px; }
    input[type="submit"] { background: #4CAF50; color: white; border: none; cursor: pointer; }
    input[type="submit"]:hover { background: #45a049; }
</style>

<?php ob_end_flush(); ?>