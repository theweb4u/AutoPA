<?php
session_start();
require "/home1/autotime/public_html/globals/config.inc";

// Debugging
ini_set('display_errors', 1);
error_reporting(E_ALL);

if (!isset($_SESSION['user_id'])) {
    header("Location: /index.php");
    exit;
}

$user_id = $_SESSION['user_id'];
$error = '';
$success = '';
$time_zone = 'UTC';
$working_hours = [];
$breaks = [];
$categories = [];

// List of common time zones
$time_zones = DateTimeZone::listIdentifiers(DateTimeZone::ALL);

try {
    // Fetch time zone
    $stmt = $pdo->prepare("SELECT time_zone FROM preferences WHERE user_id = ?");
    $stmt->execute([$user_id]);
    $pref = $stmt->fetch(PDO::FETCH_ASSOC);
    $time_zone = $pref['time_zone'] ?? 'UTC';

    // Fetch working hours
    $stmt = $pdo->prepare("SELECT day, start_time, end_time FROM working_hours WHERE user_id = ?");
    $stmt->execute([$user_id]);
    $working_hours = $stmt->fetchAll(PDO::FETCH_ASSOC);
    $hours_by_day = [];
    foreach ($working_hours as $wh) {
        $hours_by_day[$wh['day']] = $wh;
    }

    // Fetch breaks
    $stmt = $pdo->prepare("SELECT type, start_time, end_time, variance FROM breaks WHERE user_id = ?");
    $stmt->execute([$user_id]);
    $breaks = $stmt->fetchAll(PDO::FETCH_ASSOC);
    $breaks_by_type = [];
    foreach ($breaks as $b) {
        $breaks_by_type[$b['type']] = $b;
    }

    // Fetch categories
    $stmt = $pdo->prepare("SELECT id, title, description, parent_id FROM categories WHERE user_id = ? ORDER BY parent_id IS NULL DESC, title");
    $stmt->execute([$user_id]);
    $categories = $stmt->fetchAll(PDO::FETCH_ASSOC);

    // Organize categories hierarchically
    $category_tree = [];
    foreach ($categories as $cat) {
        if ($cat['parent_id'] === null) {
            $category_tree[$cat['id']] = [
                'title' => $cat['title'],
                'id' => $cat['id'],
                'subcategories' => []
            ];
        } else {
            $category_tree[$cat['parent_id']]['subcategories'][$cat['id']] = [
                'title' => $cat['title'],
                'id' => $cat['id']
            ];
        }
    }
} catch (PDOException $e) {
    $error = "Error loading preferences: " . htmlspecialchars($e->getMessage()) . ". Defaulting to UTC for time zone.";
}

// Handle category deletion
if ($_SERVER['REQUEST_METHOD'] === 'GET' && isset($_GET['delete_category']) && isset($_GET['cat_id'])) {
    $cat_id = (int)$_GET['cat_id'];
    try {
        // Check if category is used in tasks
        $stmt = $pdo->prepare("SELECT COUNT(*) FROM tasks WHERE user_id = ? AND (category_id = ? OR sub_category_id = ?)");
        $stmt->execute([$user_id, $cat_id, $cat_id]);
        $task_count = $stmt->fetchColumn();

        // Check if category is a parent
        $stmt = $pdo->prepare("SELECT COUNT(*) FROM categories WHERE user_id = ? AND parent_id = ?");
        $stmt->execute([$user_id, $cat_id]);
        $subcategory_count = $stmt->fetchColumn();

        if ($task_count > 0) {
            $error = "Cannot delete category: It is used by tasks.";
        } elseif ($subcategory_count > 0) {
            $error = "Cannot delete category: It has subcategories.";
        } else {
            $stmt = $pdo->prepare("DELETE FROM categories WHERE id = ? AND user_id = ?");
            $stmt->execute([$cat_id, $user_id]);
            $success = "Category deleted successfully.";
            header("Location: /preferences.php?success=category_deleted");
            exit;
        }
    } catch (PDOException $e) {
        $error = "Error deleting category: " . htmlspecialchars($e->getMessage());
    }
}

// Handle form submissions
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Time Zone
    if (isset($_POST['update_time_zone'])) {
        $time_zone = $_POST['time_zone'] ?? 'UTC';
        try {
            $stmt = $pdo->prepare("
                INSERT INTO preferences (user_id, time_zone) 
                VALUES (?, ?) 
                ON DUPLICATE KEY UPDATE time_zone = ?
            ");
            $stmt->execute([$user_id, $time_zone, $time_zone]);
            $success = "Time zone updated successfully.";
            header("Location: /preferences.php?success=time_zone_updated");
            exit;
        } catch (PDOException $e) {
            $error = "Error updating time zone: " . htmlspecialchars($e->getMessage());
        }
    }

    // Working Hours
    if (isset($_POST['update_hours'])) {
        try {
            $pdo->beginTransaction();
            $stmt = $pdo->prepare("DELETE FROM working_hours WHERE user_id = ?");
            $stmt->execute([$user_id]);
            foreach (['Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat', 'Sun'] as $day) {
                $start = $_POST["start_$day"] ?? null;
                $end = $_POST["end_$day"] ?? null;
                if ($start && $end) {
                    $stmt = $pdo->prepare("INSERT INTO working_hours (user_id, day, start_time, end_time) VALUES (?, ?, ?, ?)");
                    $stmt->execute([$user_id, $day, $start, $end]);
                }
            }
            $pdo->commit();
            $success = "Working hours updated.";
            header("Location: /preferences.php?success=hours_updated");
            exit;
        } catch (PDOException $e) {
            $pdo->rollBack();
            $error = "Error updating hours: " . htmlspecialchars($e->getMessage());
        }
    }

    // Breaks
    if (isset($_POST['update_breaks'])) {
        try {
            $pdo->beginTransaction();
            $stmt = $pdo->prepare("DELETE FROM breaks WHERE user_id = ?");
            $stmt->execute([$user_id]);
            foreach (['Morning Tea', 'Lunch', 'Afternoon Tea'] as $type) {
                $start = $_POST["break_start_" . str_replace(' ', '_', $type)] ?? null;
                $end = $_POST["break_end_" . str_replace(' ', '_', $type)] ?? null;
                $variance = (int)($_POST["variance_" . str_replace(' ', '_', $type)] ?? 0);
                if ($start && $end) {
                    $stmt = $pdo->prepare("INSERT INTO breaks (user_id, type, start_time, end_time, variance) VALUES (?, ?, ?, ?, ?)");
                    $stmt->execute([$user_id, $type, $start, $end, $variance]);
                }
            }
            $pdo->commit();
            $success = "Breaks updated.";
            header("Location: /preferences.php?success=breaks_updated");
            exit;
        } catch (PDOException $e) {
            $pdo->rollBack();
            $error = "Error updating breaks: " . htmlspecialchars($e->getMessage());
        }
    }
}

// Handle success messages from redirect
if (isset($_GET['success'])) {
    switch ($_GET['success']) {
        case 'time_zone_updated':
            $success = "Time zone updated.";
            break;
        case 'hours_updated':
            $success = "Working hours updated.";
            break;
        case 'breaks_updated':
            $success = "Breaks updated.";
            break;
        case 'category_deleted':
            $success = "Category deleted.";
            break;
    }
}
?>

<h1>Preferences</h1>
<?php if ($error): ?>
    <p style="color: red;"><?php echo htmlspecialchars($error); ?></p>
<?php endif; ?>
<?php if ($success): ?>
    <p style="color: green;"><?php echo htmlspecialchars($success); ?></p>
<?php endif; ?>

<!-- Time Zone -->
<h2>Time Zone</h2>
<form method="post">
    <p>
        <label>
            Time Zone:
            <select name="time_zone">
                <?php foreach ($time_zones as $tz): ?>
                    <option value="<?php echo htmlspecialchars($tz); ?>" 
                            <?php echo $time_zone === $tz ? 'selected' : ''; ?>>
                        <?php echo htmlspecialchars($tz); ?>
                    </option>
                <?php endforeach; ?>
            </select>
        </label>
    </p>
    <input type="submit" name="update_time_zone" value="Save Time Zone">
</form>

<!-- Working Hours -->
<h2>Working Hours</h2>
<form method="post">
    <?php foreach (['Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat', 'Sun'] as $day): ?>
        <label>
            <?php echo $day; ?>:
            <input type="time" name="start_<?php echo $day; ?>" value="<?php echo $hours_by_day[$day]['start_time'] ?? ''; ?>">
            to
            <input type="time" name="end_<?php echo $day; ?>" value="<?php echo $hours_by_day[$day]['end_time'] ?? ''; ?>">
        </label><br>
    <?php endforeach; ?>
    <input type="submit" name="update_hours" value="Save Working Hours">
</form>

<!-- Breaks -->
<h2>Breaks</h2>
<form method="post">
    <?php foreach (['Morning Tea', 'Lunch', 'Afternoon Tea'] as $type): ?>
        <label>
            <?php echo $type; ?>:
            <input type="time" name="break_start_<?php echo str_replace(' ', '_', $type); ?>" value="<?php echo $breaks_by_type[$type]['start_time'] ?? ''; ?>">
            to
            <input type="time" name="break_end_<?php echo str_replace(' ', '_', $type); ?>" value="<?php echo $breaks_by_type[$type]['end_time'] ?? ''; ?>">
            Variance (mins): <input type="number" name="variance_<?php echo str_replace(' ', '_', $type); ?>" min="0" value="<?php echo $breaks_by_type[$type]['variance'] ?? 0; ?>">
        </label><br>
    <?php endforeach; ?>
    <input type="submit" name="update_breaks" value="Save Breaks">
</form>

<!-- Categories -->
<h2>Categories</h2>
<a href="/add_category.php">Add New Category</a>
<ul>
    <?php foreach ($category_tree as $cat_id => $cat): ?>
        <li>
            <?php echo htmlspecialchars($cat['title']); ?>
            <a href="/edit_category.php?id=<?php echo (int)$cat['id']; ?>">Edit</a>
            <a href="/preferences.php?delete_category=1&cat_id=<?php echo (int)$cat['id']; ?>" onclick="return confirm('Delete this category?');">Delete</a>
            <?php if (!empty($cat['subcategories'])): ?>
                <ul>
                    <?php foreach ($cat['subcategories'] as $subcat_id => $subcat): ?>
                        <li>
                            <?php echo htmlspecialchars($subcat['title']); ?>
                            <a href="/edit_category.php?id=<?php echo (int)$subcat['id']; ?>">Edit</a>
                            <a href="/preferences.php?delete_category=1&cat_id=<?php echo (int)$subcat['id']; ?>" onclick="return confirm('Delete this subcategory?');">Delete</a>
                        </li>
                    <?php endforeach; ?>
                </ul>
            <?php endif; ?>
        </li>
    <?php endforeach; ?>
</ul>

<p><a href="/index.php">Back to Dashboard</a> | <a href="/logout.php">Logout</a></p>

<style>
    body { font-family: Arial, sans-serif; margin: 20px; }
    h2 { margin-top: 20px; }
    form { margin-bottom: 20px; }
    label { display: block; margin: 10px 0; }
    input, select { width: 200px; padding: 5px; }
    input[type="submit"] { margin