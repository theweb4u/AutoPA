<?php
session_start();
require "/home1/autotime/public_html/globals/config.inc";
if (!isset($_SESSION['user_id'])) {
    header("Location: /index.php");
    exit;
}

$user_id = $_SESSION['user_id'];
$error = '';

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
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
            $stmt = $pdo->prepare("INSERT INTO events (user_id, title, details, date, start_time, end_time, location) VALUES (?, ?, ?, ?, ?, ?, ?)");
            $stmt->execute([$user_id, $title, $details, $date, $start_time, $end_time, $location]);
            header("Location: /index.php");
            exit;
        } catch (PDOException $e) {
            $error = "Error adding event: " . $e->getMessage();
        }
    }
}
?>

<h1>Add Event</h1>
<?php if ($error): ?>
    <p style="color: red;"><?php echo htmlspecialchars($error); ?></p>
<?php endif; ?>
<form method="post">
    <label>Title: <input type="text" name="title" value="<?php echo htmlspecialchars($_POST['title'] ?? ''); ?>" required></label><br>
    <label>Details: <textarea name="details"><?php echo htmlspecialchars($_POST['details'] ?? ''); ?></textarea></label><br>
    <label>Date: <input type="date" name="date" value="<?php echo htmlspecialchars($_POST['date'] ?? ''); ?>" required></label><br>
    <label>Start Time: <input type="time" name="start_time" value="<?php echo htmlspecialchars($_POST['start_time'] ?? ''); ?>" required></label><br>
    <label>End Time: <input type="time" name="end_time" value="<?php echo htmlspecialchars($_POST['end_time'] ?? ''); ?>" required></label><br>
    <label>Location: <input type="text" name="location" value="<?php echo htmlspecialchars($_POST['location'] ?? ''); ?>"></label><br>
    <input type="submit" value="Add Event">
    <p><a href="/index.php">Back to Schedule</a></p>
</form>

<style>
    body { font-family: Arial, sans-serif; margin: 20px; }
    label { display: block; margin: 10px 0; }
    input, textarea { width: 200px; padding: 5px; }
    textarea { height: 100px; }
</style>