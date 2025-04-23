<?php
session_start();
require "/home1/autotime/public_html/globals/config.inc";

// Debugging
ini_set('display_errors', 1);
error_reporting(E_ALL);
file_put_contents('/home1/autotime/public_html/debug_warnings.log', "Accessing warnings.php at " . date('Y-m-d H:i:s') . "\n", FILE_APPEND);

if (!isset($_SESSION['user_id'])) {
    header("Location: /login.php");
    exit;
}

$user_id = $_SESSION['user_id'];
$tasks = [];
$error = '';

try {
    // Fetch unscheduled tasks
    $stmt = $pdo->prepare("
        SELECT id, title, due_date
        FROM tasks 
        WHERE user_id = ? AND scheduled = FALSE AND completed = FALSE
        ORDER BY title
    ");
    $stmt->execute([$user_id]);
    $tasks = $stmt->fetchAll(PDO::FETCH_ASSOC);
} catch (PDOException $e) {
    $error = "Error loading warnings: " . htmlspecialchars($e->getMessage());
}
?>

<h1>Task Warnings</h1>
<?php if ($error): ?>
    <p style="color: red;"><?php echo htmlspecialchars($error); ?></p>
<?php endif; ?>

<?php if (!empty($tasks)): ?>
    <ul>
        <?php foreach ($tasks as $task): ?>
            <?php
            $reason = 'Not scheduled';
            if ($task['due_date'] && strtotime($task['due_date']) < time()) {
                $reason = "Due date " . htmlspecialchars($task['due_date']) . " has passed";
            }
            ?>
            <li>
                <?php echo htmlspecialchars($task['title']); ?> – 
                <?php echo htmlspecialchars($reason); ?> 
                [<a href="/edit_task.php?id=<?php echo (int)$task['id']; ?>">Edit</a>]
            </li>
        <?php endforeach; ?>
    </ul>
<?php else: ?>
    <p>No unscheduled tasks.</p>
<?php endif; ?>

<p><a href="/index.php">Back to Dashboard</a></p>

<style>
    body { font-family: Arial, sans-serif; margin: 20px; }
    ul { list-style-type: none; padding-left: 20px; }
    li { margin: 5px 0; }
</style>