<?php
session_start();
ob_start();
require "/home1/autotime/public_html/globals/config.inc";
if (!isset($_SESSION['user_id'])) {
    header("Location: /index.php");
    exit;
}

$user_id = $_SESSION['user_id'];
$event_id = (int)($_GET['id'] ?? 0);
$error = '';

if (!$event_id) {
    $error = "No event specified.";
    header("Location: /index.php?error=" . urlencode($error));
    exit;
}

try {
    $stmt = $pdo->prepare("
        SELECT title, details, date, start_time, end_time, location
        FROM events 
        WHERE id = ? AND user_id = ?
    ");
    $stmt->execute([$event_id, $user_id]);
    $event = $stmt->fetch(PDO::FETCH_ASSOC);
    if (!$event) {
        $error = "Event not found.";
        header("Location: /index.php?error=" . urlencode($error));
        exit;
    }
} catch (PDOException $e) {
    $error = "Error loading event: " . htmlspecialchars($e->getMessage());
}
?>

<h1>View Event</h1>
<?php if ($error): ?>
    <p style="color: red;"><?php echo $error; ?></p>
<?php endif; ?>

<p><strong>Title:</strong> <?php echo htmlspecialchars($event['title'] ?? 'Untitled'); ?></p>
<p><strong>Details:</strong> <?php echo htmlspecialchars($event['details'] ?? 'None'); ?></p>
<p><strong>Date:</strong> <?php echo htmlspecialchars($event['date'] ?? 'None'); ?></p>
<p><strong>Time:</strong> <?php echo htmlspecialchars($event['start_time'] ?? ''); ?> to <?php echo htmlspecialchars($event['end_time'] ?? ''); ?></p>
<p><strong>Location:</strong> <?php echo htmlspecialchars($event['location'] ?? 'None'); ?></p>

<p>
    <a href="/edit_event.php?id=<?php echo (int)$event_id; ?>">Edit Event</a> | 
    <a href="/schedule_view.php">Back to Schedule</a> | 
    <a href="/index.php">Back to Dashboard</a>
</p>

<style>
    body { font-family: Arial, sans-serif; margin: 20px; }
    p { margin: 10px 0; }
    a { margin-right: 10px; }
</style>

<?php ob_end_flush(); ?>