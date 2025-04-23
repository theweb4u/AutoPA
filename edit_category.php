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
$category = null;

// Get category ID from URL
$cat_id = isset($_GET['id']) ? (int)$_GET['id'] : 0;

try {
    // Fetch category details
    $stmt = $pdo->prepare("SELECT id, title, description, parent_id FROM categories WHERE id = ? AND user_id = ?");
    $stmt->execute([$cat_id, $user_id]);
    $category = $stmt->fetch(PDO::FETCH_ASSOC);

    if (!$category) {
        $error = "Category not found.";
    }

    // Fetch all categories for parent_id dropdown
    $stmt = $pdo->prepare("SELECT id, title, parent_id FROM categories WHERE user_id = ? AND id != ? ORDER BY parent_id IS NULL DESC, title");
    $stmt->execute([$user_id, $cat_id]);
    $categories = $stmt->fetchAll(PDO::FETCH_ASSOC);

    // Build category tree to prevent cycles
    $category_tree = [];
    $descendant_ids = [$cat_id];
    foreach ($categories as $cat) {
        if ($cat['parent_id'] === $cat_id) {
            $descendant_ids[] = $cat['id'];
        }
        $category_tree[$cat['id']] = $cat;
    }
} catch (PDOException $e) {
    $error = "Error loading category: " . htmlspecialchars($e->getMessage());
}

// Handle form submission
if ($_SERVER['REQUEST_METHOD'] === 'POST' && isset($_POST['edit_category'])) {
    $title = trim($_POST['title'] ?? '');
    $description = trim($_POST['description'] ?? '');
    $parent_id = (int)($_POST['parent_id'] ?? 0) ?: null;

    if (empty($title)) {
        $error = "Category title is required.";
    } elseif (in_array($parent_id, $descendant_ids)) {
        $error = "Cannot set parent to self or a subcategory.";
    } else {
        try {
            // Check for duplicate title
            $stmt = $pdo->prepare("SELECT COUNT(*) FROM categories WHERE user_id = ? AND title = ? AND id != ?");
            $stmt->execute([$user_id, $title, $cat_id]);
            if ($stmt->fetchColumn() > 0) {
                $error = "Category title '$title' already exists.";
            } else {
                $stmt = $pdo->prepare("UPDATE categories SET title = ?, description = ?, parent_id = ? WHERE id = ? AND user_id = ?");
                $stmt->execute([$title, $description, $parent_id, $cat_id, $user_id]);
                $success = "Category updated successfully.";
                header("Location: /preferences.php?success=category_updated");
                exit;
            }
        } catch (PDOException $e) {
            $error = "Error updating category: " . htmlspecialchars($e->getMessage());
        }
    }
}
?>

<h1>Edit Category</h1>
<?php if ($error): ?>
    <p style="color: red;"><?php echo htmlspecialchars($error); ?></p>
<?php endif; ?>
<?php if ($success): ?>
    <p style="color: green;"><?php echo htmlspecialchars($success); ?></p>
<?php endif; ?>

<?php if ($category): ?>
    <form method="post">
        <input type="hidden" name="edit_category" value="1">
        <p>
            <label>
                Title:
                <input type="text" name="title" value="<?php echo htmlspecialchars($category['title']); ?>" required>
            </label>
        </p>
        <p>
            <label>
                Description:
                <textarea name="description"><?php echo htmlspecialchars($category['description'] ?? ''); ?></textarea>
            </label>
        </p>
        <p>
            <label>
                Parent Category:
                <select name="parent_id">
                    <option value="">None</option>
                    <?php foreach ($categories as $parent): ?>
                        <?php if (!in_array($parent['id'], $descendant_ids)): ?>
                            <option value="<?php echo $parent['id']; ?>" <?php echo $category['parent_id'] == $parent['id'] ? 'selected' : ''; ?>>
                                <?php echo htmlspecialchars($parent['title']); ?>
                            </option>
                        <?php endif; ?>
                    <?php endforeach; ?>
                </select>
            </label>
        </p>
        <input type="submit" value="Update Category">
    </form>
<?php endif; ?>

<p><a href="/preferences.php">Back to Preferences</a></p>

<style>
    body { font-family: Arial, sans-serif; margin: 20px; }
    form { margin-bottom: 20px; }
    label { display: block; margin: 10px 0; }
    input, select, textarea { width: 200px; padding: 5px; }
    textarea { height: 100px; }
    input[type="submit"] { margin-top: 10px; }
</style>