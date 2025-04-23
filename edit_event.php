<?php
session_start();
require "/home1/autotime/public_html/globals/config.inc";
if (!isset($_SESSION['user_id'])) {
    header("Location: /index.php");
    exit;
}

$user_id = $_SESSION['user_id'];
$event_id = (int)($_GET['id'] ?? 0);
$error = '';

if (!$event_id) {
    header("Location: /index.php");
    exit;
}

// Fetch event
try {
    $stmt = $pdo->prepare("SELECT title, details, date, start_time, end_time, location FROM events WHERE id = ? AND user_id = ?");
    $stmt->execute([$event_id, $user_id]);
    $event = $stmt->fetch(PDO::FETCH_ASSOC);
    if (!$event) {
        header("Location: /index.php");
        exit;
    }
} catch (PDOException $e) {
    $error = "Error loading event: " . $e->getMessage();
}

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    if (isset($_POST['update_event'])) {
        $title = trim($_POST['title'] ?? '');
        $details = trim($_POST['details'] ?? '');
        $date = $_POST['date'] ?? '';
        $start_time = $_POST['start_time'] ?? '';
        $end_time = $_POST['end_time'] ?? '';
        $location = trim($_POST['location'] ?? '');

        // Validation
        if (empty($title)) {
            $error = "Title is required.";
        } elseif (empty($date)) {
            $error = "Date is required.";
        } elseif (empty($start_time) || empty($end_time)) {
            $error = "Start and end times are required.";
        } else {
            try {
                $stmt = $pdo->prepare("
                    UPDATE events 
                    SET title = ?, details = ?, date = ?, start_time = ?, end_time = ?, location = ? 
                    WHERE id = ? AND user_id = ?
                ");
                $stmt->execute([$title, $details, $date, $start_time, $end_time, $location, $event_id, $user_id]);
                header("Location: /index.php?success=event_updated");
                exit;
            } catch (PDOException $e) {
                $error = "Error updating event: " . $e->getMessage();
            }
        }
    } elseif (isset($_POST['delete_event'])) {
        try {
            $stmt = $pdo->prepare("DELETE FROM events WHERE id = ? AND user_id = ?");
            $stmt->execute([$event_id, $user_id]);
            header("Location: /index.php?success=event_deleted");
            exit;
        } catch (PDOException $e) {
            $error = "Error deleting event: " . $e->getMessage();
        }
    }
}
?>

<h1>Edit Event</h1>
<?php if ($error): ?>
    <p style="color: red;"><?php echo htmlspecialchars($error); ?></p>
<?php endif; ?>
<form method="post">
    <label>Title: <input type="text" name="title" value="<?php echo htmlspecialchars($event['title'] ?? ''); ?>" required></label><br>
    <label>Details: <textarea name="details"><?php echo htmlspecialchars($event['details'] ?? ''); ?></textarea></label><br>
    <label>Date: <input type="date" name="date" value="<?php echo htmlspecialchars($event['date'] ?? ''); ?>" required></label><br>
    <label>Start Time: <input type="time" name="start_time" value="<?php echo htmlspecialchars($event['start_time'] ?? ''); ?>" required></label><br>
    <label>End Time: <input type="time" name="end_time" value="<?php echo htmlspecialchars($event['end_time'] ?? ''); ?>" required></label><br>
    <label>Location: <input type="text" name="location" value="<?php echo htmlspecialchars($event['location'] ?? ''); ?>"></label><br>
    <input type="submit" name="update_event" value="Update Event">
    <input type="submit" name="delete_event" value="Delete Event" onclick="return confirm('Delete this event?');">
</form>
<p><a href="/index.php">Back to Schedule</a></p>

<style>
    body { font-family: Arial, sans-serif; margin: 20px; }
    label { display: block; margin: 10px 0; }
    input, textarea { width: 200px; padding: 5px; }
    textarea { height: 100px; }
    input[type="submit"] { margin-right: 10px; }
</style>