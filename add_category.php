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

// Fetch existing categories for parent_id dropdown
try {
    $stmt = $pdo->prepare("SELECT id, title FROM categories WHERE user_id = ? AND parent_id IS NULL ORDER BY title");
    $stmt->execute([$user_id]);
    $categories = $stmt->fetchAll(PDO::FETCH_ASSOC);
} catch (PDOException $e) {
    $error = "Error loading categories: " . htmlspecialchars($e->getMessage());
}

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $title = trim($_POST['title'] ?? '');
    $description = trim($_POST['description'] ?? '');
    $parent_id = (int)($_POST['parent_id'] ?? 0) ?: null;

    if (empty($title)) {
        $error = "Category title is required.";
    } else {
        try {
            // Check for duplicate title
            $stmt = $pdo->prepare("SELECT COUNT(*) FROM categories WHERE user_id = ? AND title = ?");
            $stmt->execute([$user_id, $title]);
            if ($stmt->fetchColumn() > 0) {
                $error = "Category title '$title' already exists.";
            } else {
                $stmt = $pdo->prepare("INSERT INTO categories (user_id, title, description, parent_id) VALUES (?, ?, ?, ?)");
                $stmt->execute([$user_id, $title, $description, $parent_id]);
                $success = "Category added successfully.";
                header("Location: /preferences.php?success=category_added");
                exit;
            }
        } catch (PDOException $e) {
            $error = "Error adding category: " . htmlspecialchars($e->getMessage());
        }
    }
}
?>

<h1>Add Category</h1>
<?php if ($error): ?>
    <p style="color: red;"><?php echo htmlspecialchars($error); ?></p>
<?php endif; ?>
<?php if ($success): ?>
    <p style="color: green;"><?php echo htmlspecialchars($success); ?></p>
<?php endif; ?>

<form method="post">
    <label>
        Title: <input type="text" name="title" value="<?php echo htmlspecialchars($_POST['title'] ?? ''); ?>" required>
    </label>
    <label>
        Description: <textarea name="description"><?php echo htmlspecialchars($_POST['description'] ?? ''); ?></textarea>
    </label>
    <label>
        Parent Category:
        <select name="parent_id">
            <option value="">None</option>
            <?php foreach ($categories as $cat): ?>
                <option value="<?php echo $cat['id']; ?>" <?php echo ($_POST['parent_id'] ?? '') == $cat['id'] ? 'selected' : ''; ?>>
                    <?php echo htmlspecialchars($cat['title']); ?>
                </option>
            <?php endforeach; ?>
        </select>
    </label>
    <input type="submit" value="Add Category">
</form>

<p><a href="/preferences.php">Back to Preferences</a></p>

<style>
    body { font-family: Arial, sans-serif; margin: 20px; }
    form { margin-bottom: 20px; }
    label { display: block; margin: 10px 0; }
    input, select, textarea { width: 200px; padding: 5px; }
    textarea { height: 100px; }
</style>