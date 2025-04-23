<?php
session_start();
require "/home1/autotime/public_html/globals/config.inc";

// Debugging
ini_set('display_errors', 1);
error_reporting(E_ALL);
file_put_contents('/home1/autotime/public_html/debug_schedule_view.log', "Accessing schedule view at " . date('Y-m-d H:i:s') . "\n", FILE_APPEND);

if (!isset($_SESSION['user_id'])) {
    header("Location: /login.php");
    exit;
}

$user_id = $_SESSION['user_id'];
$today = date('Y-m-d');
$error = '';

try {
    // Fetch schedule entries for days with tasks or events
    $stmt = $pdo->prepare("
        SELECT date, start_time, end_time, title, type, task_id
        FROM schedules 
        WHERE user_id = ? AND date >= ?
        AND date IN (
            SELECT DISTINCT date 
            FROM schedules 
            WHERE user_id = ? AND type IN ('task', 'event')
        )
        ORDER BY date, start_time
    ");
    $stmt->execute([$user_id, $today, $user_id]);
    $schedule = $stmt->fetchAll(PDO::FETCH_ASSOC);
    file_put_contents('/home1/autotime/public_html/debug_schedule_view.log', 
        "Found " . count($schedule) . " schedule entries\n", FILE_APPEND);

    // Group by date
    $schedule_by_date = [];
    foreach ($schedule as $entry) {
        $date = $entry['date'];
        if (!isset($schedule_by_date[$date])) {
            $schedule_by_date[$date] = [];
        }
        $schedule_by_date[$date][] = $entry;
    }
} catch (PDOException $e) {
    $error = "Error loading schedule: " . htmlspecialchars($e->getMessage());
    file_put_contents('/home1/autotime/public_html/debug_schedule_view.log', $error . "\n", FILE_APPEND);
}
?>

<h1>Full Schedule</h1>
<?php if ($error): ?>
    <p style="color: red;"><?php echo htmlspecialchars($error); ?></p>
<?php endif; ?>

<?php if (empty($schedule_by_date)): ?>
    <p>No schedule entries found.</p>
<?php else: ?>
    <?php foreach ($schedule_by_date as $date => $entries): ?>
        <h2><?php echo htmlspecialchars(date('d-m-Y', strtotime($date))); ?></h2>
        <?php foreach ($entries as $entry): ?>
            <p>
                <?php echo htmlspecialchars($entry['start_time']) . ' – ' . htmlspecialchars($entry['end_time']); ?> – 
                <a href="<?php echo $entry['type'] === 'task' && $entry['task_id'] ? '/view_task.php?id=' . (int)$entry['task_id'] : '#'; ?>">
                    <?php echo htmlspecialchars($entry['title']); ?>
                </a>
                (<?php echo htmlspecialchars(ucfirst($entry['type'])); ?>)
            </p>
        <?php endforeach; ?>
    <?php endforeach; ?>
<?php endif; ?>

<p><a href="/index.php">Back to Dashboard</a></p>

<style>
    body { font-family: Arial, sans-serif; margin: 20px; }
    h2 { margin-top: 20px; }
    p { margin: 5px 0; }
</style>