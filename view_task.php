<?php
session_start();
require "/home1/autotime/public_html/globals/config.inc";

// Debugging
ini_set('display_errors', 1);
error_reporting(E_ALL);

if (!isset($_SESSION['user_id'])) {
    header("Location: /login.php");
    exit;
}

$user_id = $_SESSION['user_id'];
$task_id = $_GET['id'] ?? 0;
$task = null;
$error = '';
$success = '';

try {
    $stmt = $pdo->prepare("
        SELECT t.id, t.title, t.time_hrs, t.time_mins, t.priority, t.importance, t.urgency, 
               t.time_split, t.breaks, t.due_date, t.category_id, t.completed, 
               t.date, t.start_time, t.end_time, c.title AS category_name
        FROM tasks t
        LEFT JOIN categories c ON t.category_id = c.id
        WHERE t.id = ? AND t.user_id = ?
    ");
    $stmt->execute([$task_id, $user_id]);
    $task = $stmt->fetch(PDO::FETCH_ASSOC);

    if (!$task) {
        $error = "Task not found or you don't have permission to view it.";
    }
} catch (PDOException $e) {
    $error = "Error loading task: " . htmlspecialchars($e->getMessage());
}

if ($_SERVER['REQUEST_METHOD'] === 'POST' && isset($_POST['mark_done'])) {
    try {
        $stmt = $pdo->prepare("UPDATE tasks SET completed = TRUE WHERE id = ? AND user_id = ?");
        $stmt->execute([$task_id, $user_id]);
        $success = "Task marked as completed.";
        $task['completed'] = true;
    } catch (PDOException $e) {
        $error = "Error marking task as completed: " . htmlspecialchars($e->getMessage());
    }
}
?>

<h1>View Task</h1>
<?php if ($error): ?>
    <p style="color: red;"><?php echo htmlspecialchars($error); ?></p>
<?php endif; ?>
<?php if ($success): ?>
    <p style="color: green;"><?php echo htmlspecialchars($success); ?></p>
<?php endif; ?>

<?php if ($task): ?>
    <p><strong>Title:</strong> <?php echo htmlspecialchars($task['title']); ?></p>
    <p><strong>Duration:</strong> <?php echo (int)$task['time_hrs']; ?>h <?php echo (int)$task['time_mins']; ?>min</p>
    <p><strong>Priority:</strong> <?php echo (int)$task['priority']; ?></p>
    <p><strong>Importance:</strong> <?php echo htmlspecialchars($task['importance']); ?></p>
    <p><strong>Urgency:</strong> <?php echo htmlspecialchars($task['urgency']); ?></p>
    <p><strong>Time Split:</strong> <?php echo htmlspecialchars($task['time_split']); ?></p>
    <p><strong>Include Breaks:</strong> <?php echo htmlspecialchars($task['breaks']); ?></p>
    <p><strong>Due Date:</strong> <?php echo $task['due_date'] ? htmlspecialchars($task['due_date']) : 'None'; ?></p>
    <p><strong>Category:</strong> <?php echo htmlspecialchars($task['category_name'] ?: 'None'); ?></p>
    <p><strong>Status:</strong> <?php echo $task['completed'] ? 'Completed' : 'Not Completed'; ?></p>
    <?php if ($task['date'] && $task['start_time'] && $task['end_time']): ?>
        <p><strong>Scheduled:</strong> <?php echo date('d-m-Y', strtotime($task['date'])) . ' ' . 
                                          htmlspecialchars($task['start_time']) . ' to ' . 
                                          htmlspecialchars($task['end_time']); ?></p>
    <?php endif; ?>

    <?php if (!$task['completed']): ?>
        <form method="post">
            <input type="submit" name="mark_done" value="Mark as Done">
        </form>
    <?php endif; ?>
    <p>
        <a href="/edit_task.php?id=<?php echo (int)$task['id']; ?>">Edit Task</a> | 
        <a href="/schedule_view.php">Back to Schedule</a> | 
        <a href="/index.php">Back to Dashboard</a>
    </p>
<?php else: ?>
    <p>Task not found.</p>
    <p>
        <a href="/schedule_view.php">Back to Schedule</a> | 
        <a href="/index.php">Back to Dashboard</a>
    </p>
<?php endif; ?>

<style>
    body { font-family: Arial, sans-serif; margin: 20px; }
    p { margin: 5px 0; }
</style>