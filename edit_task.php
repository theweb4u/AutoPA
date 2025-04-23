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
$task_id = (int)($_GET['id'] ?? 0);
$error = '';
$success = '';

// Fetch task
try {
    $stmt = $pdo->prepare("
        SELECT title, category_id, sub_category_id, time_hrs, time_mins, due_date, priority
        FROM tasks WHERE id = ? AND user_id = ?
    ");
    $stmt->execute([$task_id, $user_id]);
    $task = $stmt->fetch(PDO::FETCH_ASSOC);
    if (!$task) {
        $error = "Task not found.";
    }
} catch (PDOException $e) {
    $error = "Error loading task: " . htmlspecialchars($e->getMessage());
}

// Fetch categories (top-level)
try {
    $stmt = $pdo->prepare("SELECT id, title FROM categories WHERE user_id = ? AND parent_id IS NULL ORDER BY title");
    $stmt->execute([$user_id]);
    $categories = $stmt->fetchAll(PDO::FETCH_ASSOC);
} catch (PDOException $e) {
    $error = "Error loading categories: " . htmlspecialchars($e->getMessage());
}

// Fetch subcategories for current category
$subcategories = [];
if ($task && $task['category_id']) {
    $stmt = $pdo->prepare("SELECT id, title FROM categories WHERE user_id = ? AND parent_id = ? ORDER BY title");
    $stmt->execute([$user_id, $task['category_id']]);
    $subcategories = $stmt->fetchAll(PDO::FETCH_ASSOC);
}

if ($_SERVER['REQUEST_METHOD'] === 'POST' && !$error) {
    $title = trim($_POST['title'] ?? '');
    $category_id = (int)($_POST['category_id'] ?? 0);
    $sub_category_id = (int)($_POST['sub_category_id'] ?? 0) ?: null;
    $time_hrs = (int)($_POST['time_hrs'] ?? 0);
    $time_mins = (int)($_POST['time_mins'] ?? 0);
    $due_date = $_POST['due_date'] ?? null;
    $priority = (int)($_POST['priority'] ?? 0);

    if (empty($title)) {
        $error = "Task title is required.";
    } elseif ($time_hrs == 0 && $time_mins == 0) {
        $error = "Task duration is required.";
    } else {
        try {
            $stmt = $pdo->prepare("
                UPDATE tasks 
                SET title = ?, category_id = ?, sub_category_id = ?, time_hrs = ?, time_mins = ?, due_date = ?, priority = ?
                WHERE id = ? AND user_id = ?
            ");
            $stmt->execute([$title, $category_id, $sub_category_id, $time_hrs, $time_mins, $due_date, $priority, $task_id, $user_id]);
            $success = "Task updated successfully.";
            header("Location: /index.php?success=task_updated");
            exit;
        } catch (PDOException $e) {
            $error = "Error updating task: " . htmlspecialchars($e->getMessage());
        }
    }
}
?>

<h1>Edit Task</h1>
<?php if ($error): ?>
    <p style="color: red;"><?php echo htmlspecialchars($error); ?></p>
<?php endif; ?>
<?php if ($success): ?>
    <p style="color: green;"><?php echo htmlspecialchars($success); ?></p>
<?php endif; ?>

<?php if ($task): ?>
<form method="post" id="taskForm">
    <label>
        Title: <input type="text" name="title" value="<?php echo htmlspecialchars($task['title']); ?>" required>
    </label>
    <label>
        Category:
        <select name="category_id" id="category_id" required>
            <option value="">Select Category</option>
            <?php foreach ($categories as $cat): ?>
                <option value="<?php echo $cat['id']; ?>" <?php echo $task['category_id'] == $cat['id'] ? 'selected' : ''; ?>>
                    <?php echo htmlspecialchars($cat['title']); ?>
                </option>
            <?php endforeach; ?>
        </select>
    </label>
    <label>
        Subcategory:
        <select name="sub_category_id" id="sub_category_id">
            <option value="">None</option>
            <?php foreach ($subcategories as $subcat): ?>
                <option value="<?php echo $subcat['id']; ?>" <?php echo $task['sub_category_id'] == $subcat['id'] ? 'selected' : ''; ?>>
                    <?php echo htmlspecialchars($subcat['title']); ?>
                </option>
            <?php endforeach; ?>
        </select>
    </label>
    <label>
        Duration:
        <input type="number" name="time_hrs" min="0" value="<?php echo htmlspecialchars($task['time_hrs']); ?>"> hrs
        <input type="number" name="time_mins" min="0" max="59" value="<?php echo htmlspecialchars($task['time_mins']); ?>"> mins
    </label>
    <label>
        Due Date: <input type="date" name="due_date" value="<?php echo htmlspecialchars($task['due_date'] ?? ''); ?>">
    </label>
    <label>
        Priority: <input type="number" name="priority" min="0" value="<?php echo htmlspecialchars($task['priority']); ?>">
    </label>
    <input type="submit" value="Update Task">
</form>
<?php endif; ?>

<p><a href="/index.php">Back to Dashboard</a></p>

<script>
document.getElementById('category_id').addEventListener('change', function() {
    let categoryId = this.value;
    let subcategorySelect = document.getElementById('sub_category_id');
    subcategorySelect.innerHTML = '<option value="">None</option>';

    if (categoryId) {
        fetch('/get_subcategories.php', {
            method: 'POST',
            headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
            body: 'category_id=' + encodeURIComponent(categoryId)
        })
        .then(response => response.json())
        .then(data => {
            data.forEach(subcat => {
                let option = document.createElement('option');
                option.value = subcat.id;
                option.textContent = subcat.title;
                subcategorySelect.appendChild(option);
            });
        })
        .catch(error => console.error('Error fetching subcategories:', error));
    }
});
</script>

<style>
    body { font-family: Arial, sans-serif; margin: 20px; }
    form { margin-bottom: 20px; }
    label { display: block; margin: 10px 0; }
    input, select { width: 200px; padding: 5px; }
</style>