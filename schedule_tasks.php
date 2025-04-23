<?php
session_start();
require "/home1/autotime/public_html/globals/config.inc";

// Debugging
ini_set('display_errors', 1);
error_reporting(E_ALL);
file_put_contents('/home1/autotime/public_html/debug_schedule.log', "Starting scheduling at " . date('Y-m-d H:i:s') . "\n", FILE_APPEND);

if (!isset($_SESSION['user_id'])) {
    header("Location: /login.php");
    exit;
}

$user_id = $_SESSION['user_id'];
$target_date = $_POST['target_date'] ?? date('Y-m-d');
$target_time = $_POST['target_time'] ?? '00:00';
$target_datetime = "$target_date $target_time:00";
$success = '';
$error = '';

try {
    // Begin transaction
    $pdo->beginTransaction();

    // Clear previous schedule
    $stmt = $pdo->prepare("DELETE FROM schedules WHERE user_id = ?");
    $stmt->execute([$user_id]);
    file_put_contents('/home1/autotime/public_html/debug_schedule.log', "Cleared previous schedule\n", FILE_APPEND);

    // Reset task scheduling fields
    $stmt = $pdo->prepare("UPDATE tasks SET scheduled = FALSE, date = NULL, start_time = NULL, end_time = NULL WHERE user_id = ?");
    $stmt->execute([$user_id]);
    file_put_contents('/home1/autotime/public_html/debug_schedule.log', "Reset task scheduling fields\n", FILE_APPEND);

    // Fetch unscheduled tasks
    $stmt = $pdo->prepare("
        SELECT id, title, time_hrs, time_mins, due_date 
        FROM tasks 
        WHERE user_id = ? AND scheduled = FALSE AND completed = FALSE
        ORDER BY priority, due_date
    ");
    $stmt->execute([$user_id]);
    $tasks = $stmt->fetchAll(PDO::FETCH_ASSOC);
    foreach ($tasks as &$task) {
        $task['scheduled'] = false; // Initialize to avoid warnings
    }
    file_put_contents('/home1/autotime/public_html/debug_schedule.log', "Unscheduled tasks found: " . count($tasks) . "\n", FILE_APPEND);

    // Fetch breaks
    $stmt = $pdo->prepare("SELECT type, start_time, end_time FROM breaks WHERE user_id = ? ORDER BY start_time");
    $stmt->execute([$user_id]);
    $breaks = $stmt->fetchAll(PDO::FETCH_ASSOC);
    file_put_contents('/home1/autotime/public_html/debug_schedule.log', "Breaks found: " . count($breaks) . "\n", FILE_APPEND);

    $scheduled_count = 0;
    $current_datetime = new DateTime($target_datetime);

    while ($current_datetime->format('Y-m-d') <= date('Y-m-d', strtotime('+30 days'))) {
        $current_date = $current_datetime->format('Y-m-d');
        file_put_contents('/home1/autotime/public_html/debug_schedule.log', "Scheduling for: $current_date\n", FILE_APPEND);

        // Fetch work hours
        $dow = substr(strtolower($current_datetime->format('D')), 0, 3); // Mon, Tue, etc.
        $stmt = $pdo->prepare("SELECT start_time, end_time FROM working_hours WHERE user_id = ? AND day = ?");
        $stmt->execute([$user_id, ucfirst($dow)]);
        $work_hours = $stmt->fetch(PDO::FETCH_ASSOC);
        if (!$work_hours || !$work_hours['start_time'] || !$work_hours['end_time']) {
            $current_datetime->modify('+1 day');
            $current_datetime->setTime(0, 0);
            continue;
        }

        $work_start = $work_hours['start_time'];
        $work_end = $work_hours['end_time'];
        file_put_contents('/home1/autotime/public_html/debug_schedule.log', 
            "Work hours for $dow: $work_start to $work_end\n", FILE_APPEND);

        // Fetch events for the day
        $stmt = $pdo->prepare("SELECT title, start_time, end_time FROM events WHERE user_id = ? AND date = ?");
        $stmt->execute([$user_id, $current_date]);
        $events = $stmt->fetchAll(PDO::FETCH_ASSOC);
        file_put_contents('/home1/autotime/public_html/debug_schedule.log', 
            "Events found for $current_date: " . count($events) . "\n", FILE_APPEND);

        // Create available slots
        $slots = [];
        $start_time = $current_datetime->format('H:i:s') > $work_start ? 
                      $current_datetime->format('H:i:s') : $work_start;
        $end_time = $work_end;
        $current_time = new DateTime("$current_date $start_time");
        $end_datetime = new DateTime("$current_date $end_time");

        while ($current_time < $end_datetime) {
            $slot_end = clone $current_time;
            $slot_end->modify('+30 minutes');
            if ($slot_end > $end_datetime) {
                break;
            }
            $slots[] = [
                'start' => $current_time->format('H:i:s'),
                'end' => $slot_end->format('H:i:s'),
                'available' => true
            ];
            $current_time = $slot_end;
        }

        // Schedule breaks
        foreach ($breaks as $break) {
            $stmt = $pdo->prepare("
                INSERT INTO schedules (user_id, title, date, start_time, end_time, type)
                VALUES (?, ?, ?, ?, ?, 'break')
            ");
            $stmt->execute([$user_id, $break['type'], $current_date, $break['start_time'], $break['end_time']]);
            foreach ($slots as &$slot) {
                if (!($slot['end'] <= $break['start_time'] || $slot['start'] >= $break['end_time'])) {
                    $slot['available'] = false;
                }
            }
        }

        // Schedule events
        foreach ($events as $event) {
            $stmt = $pdo->prepare("
                INSERT INTO schedules (user_id, title, date, start_time, end_time, type)
                VALUES (?, ?, ?, ?, ?, 'event')
            ");
            $stmt->execute([$user_id, $event['title'], $current_date, $event['start_time'], $event['end_time']]);
            foreach ($slots as &$slot) {
                if (!($slot['end'] <= $event['start_time'] || $slot['start'] >= $event['end_time'])) {
                    $slot['available'] = false;
                }
            }
        }

        // Schedule tasks
        foreach ($tasks as &$task) {
            if ($task['scheduled']) {
                continue; // Skip already scheduled tasks
            }

            if ($task['due_date'] && $task['due_date'] < $current_date) {
                file_put_contents('/home1/autotime/public_html/debug_schedule.log', 
                    "Task '{$task['title']}' (ID: {$task['id']}): cannot be scheduled, due date {$task['due_date']} passed\n", 
                    FILE_APPEND);
                continue;
            }

            $remaining_duration = ($task['time_hrs'] * 3600) + ($task['time_mins'] * 60);
            if ($remaining_duration <= 0) {
                $task['scheduled'] = true;
                continue;
            }

            file_put_contents('/home1/autotime/public_html/debug_schedule.log', 
                "Processing task '{$task['title']}' (ID: {$task['id']}): remaining $remaining_duration seconds on $current_date\n", 
                FILE_APPEND);

            $consecutive_slots = [];
            $part_duration = 0;

            foreach ($slots as $slot) {
                if (!$slot['available']) {
                    if ($consecutive_slots && $part_duration > 0) {
                        // Save partial task segment
                        $end_datetime = new DateTime("$current_date " . $consecutive_slots[0]['start']);
                        $end_datetime->modify("+$part_duration seconds");
                        $end_time = $end_datetime->format('H:i:s');
                        $part_title = $remaining_duration == ($task['time_hrs'] * 3600 + $task['time_mins'] * 60) 
                            ? $task['title'] 
                            : "{$task['title']} (Part)";

                        $stmt = $pdo->prepare("
                            INSERT INTO schedules (user_id, task_id, title, date, start_time, end_time, type)
                            VALUES (?, ?, ?, ?, ?, ?, 'task')
                        ");
                        $stmt->execute([$user_id, $task['id'], $part_title, $current_date, 
                                      $consecutive_slots[0]['start'], $end_time]);
                        file_put_contents('/home1/autotime/public_html/debug_schedule.log', 
                            "Scheduled task '{$part_title}' (ID: {$task['id']}): " . 
                            $consecutive_slots[0]['start'] . " to $end_time on $current_date\n", 
                            FILE_APPEND);

                        $remaining_duration -= $part_duration;
                        $scheduled_count++;

                        // Mark slots as used
                        foreach ($consecutive_slots as $used_slot) {
                            foreach ($slots as &$s) {
                                if ($s['start'] === $used_slot['start']) {
                                    $s['available'] = false;
                                }
                            }
                        }
                    }
                    $consecutive_slots = [];
                    $part_duration = 0;
                    continue;
                }

                $consecutive_slots[] = $slot;
                $part_duration += 1800; // 30 minutes in seconds

                if ($part_duration >= $remaining_duration) {
                    // Schedule final part
                    $end_datetime = new DateTime("$current_date " . $consecutive_slots[0]['start']);
                    $end_datetime->modify("+$remaining_duration seconds");
                    $end_time = $end_datetime->format('H:i:s');
                    $part_title = $remaining_duration == ($task['time_hrs'] * 3600 + $task['time_mins'] * 60) 
                        ? $task['title'] 
                        : "{$task['title']} (Part)";

                    $stmt = $pdo->prepare("
                        INSERT INTO schedules (user_id, task_id, title, date, start_time, end_time, type)
                        VALUES (?, ?, ?, ?, ?, ?, 'task')
                    ");
                    $stmt->execute([$user_id, $task['id'], $part_title, $current_date, 
                                  $consecutive_slots[0]['start'], $end_time]);
                    file_put_contents('/home1/autotime/public_html/debug_schedule.log', 
                        "Scheduled task '{$part_title}' (ID: {$task['id']}): " . 
                        $consecutive_slots[0]['start'] . " to $end_time on $current_date\n", 
                        FILE_APPEND);

                    $remaining_duration = 0;
                    $scheduled_count++;

                    // Mark slots as used
                    foreach ($consecutive_slots as $used_slot) {
                        foreach ($slots as &$s) {
                            if ($s['start'] === $used_slot['start']) {
                                $s['available'] = false;
                            }
                        }
                    }

                    $task['scheduled'] = true;
                    break;
                }
            }

            // Save any remaining part
            if ($consecutive_slots && $part_duration > 0 && $remaining_duration > 0) {
                $end_datetime = new DateTime("$current_date " . $consecutive_slots[0]['start']);
                $end_datetime->modify("+$part_duration seconds");
                $end_time = $end_datetime->format('H:i:s');
                $part_title = $remaining_duration == ($task['time_hrs'] * 3600 + $task['time_mins'] * 60) 
                    ? $task['title'] 
                    : "{$task['title']} (Part)";

                $stmt = $pdo->prepare("
                    INSERT INTO schedules (user_id, task_id, title, date, start_time, end_time, type)
                    VALUES (?, ?, ?, ?, ?, ?, 'task')
                ");
                $stmt->execute([$user_id, $task['id'], $part_title, $current_date, 
                              $consecutive_slots[0]['start'], $end_time]);
                file_put_contents('/home1/autotime/public_html/debug_schedule.log', 
                    "Scheduled task '{$part_title}' (ID: {$task['id']}): " . 
                    $consecutive_slots[0]['start'] . " to $end_time on $current_date\n", 
                    FILE_APPEND);

                $remaining_duration -= $part_duration;
                $scheduled_count++;

                // Mark slots as used
                foreach ($consecutive_slots as $used_slot) {
                    foreach ($slots as &$s) {
                        if ($s['start'] === $used_slot['start']) {
                            $s['available'] = false;
                        }
                    }
                }
            }

            if ($remaining_duration <= 0) {
                $task['scheduled'] = true;
            }
        }

        $current_datetime->modify('+1 day');
        $current_datetime->setTime(0, 0);
    }

    // Update scheduled tasks in database
    foreach ($tasks as $task) {
        if ($task['scheduled']) {
            $stmt = $pdo->prepare("UPDATE tasks SET scheduled = TRUE WHERE id = ? AND user_id = ?");
            $stmt->execute([$task['id'], $user_id]);
        }
    }

    $pdo->commit();
    $success = "Scheduled $scheduled_count task(s) starting from $target_datetime.";
    file_put_contents('/home1/autotime/public_html/debug_schedule.log', $success . "\n", FILE_APPEND);
} catch (PDOException $e) {
    $pdo->rollBack();
    $error = "Error scheduling tasks: " . htmlspecialchars($e->getMessage());
    file_put_contents('/home1/autotime/public_html/debug_schedule.log', $error . "\n", FILE_APPEND);
}
?>

<h1>Schedule Tasks</h1>
<?php if ($error): ?>
    <p style="color: red;"><?php echo htmlspecialchars($error); ?></p>
<?php endif; ?>
<?php if ($success): ?>
    <p style="color: green;"><?php echo htmlspecialchars($success); ?></p>
<?php endif; ?>

<p><a href="/index.php">Back to Dashboard</a></p>

<style>
    body { font-family: Arial, sans-serif; margin: 20px; }
</style>