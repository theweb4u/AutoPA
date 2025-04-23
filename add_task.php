<?php
session_start();
require "/home1/autotime/public_html/globals/config.inc";

// Debugging
ini_set('display_errors', 1);
error_reporting(E_ALL);

if (!isset($_SESSION['user_id'])) {
    header("Location: /login.php");
    exit;
}

$user_id = $_SESSION['user_id'];
$success = '';
$error = '';

try {
    // Fetch categories
    $stmt = $pdo->prepare("SELECT id, title, parent_id FROM categories WHERE user_id = ? ORDER BY parent_id IS NULL DESC, title");
    $stmt->execute([$user_id]);
    $categories = $stmt->fetchAll(PDO::FETCH_ASSOC);

    $category_options = [];
    $subcategories = [];
    foreach ($categories as $cat) {
        if ($cat['parent_id'] === null) {
            $category_options[$cat['id']] = $cat['title'];
        } else {
            $subcategories[$cat['parent_id']][$cat['id']] = $cat['title'];
        }
    }

    // Handle form submission
    if ($_SERVER['REQUEST_METHOD'] === 'POST') {
        // Handle task submission
        if (isset($_POST['task_submit'])) {
            $title = trim($_POST['title'] ?? '');
            $description = trim($_POST['description'] ?? '');
            $category_id = (int)($_POST['category_id'] ?? 0);
            $sub_category_id = !empty($_POST['sub_category_id']) ? (int)$_POST['sub_category_id'] : null;
            $time_hrs = (int)($_POST['time_hrs'] ?? 0);
            $time_mins = (int)($_POST['time_mins'] ?? 0);
            $due_date = !empty($_POST['due_date']) ? $_POST['due_date'] : null;
            $importance = $_POST['importance'] ?? 'Not Important';
            $urgency = $_POST['urgency'] ?? 'Not Urgent';
            $priority = (int)($_POST['priority'] ?? 3); // Default to medium

            if (empty($title)) {
                $error = "Task title is required.";
            } elseif ($category_id <= 0 || !isset($category_options[$category_id])) {
                $error = "Invalid category selected.";
            } elseif ($sub_category_id && (!isset($subcategories[$category_id]) || !isset($subcategories[$category_id][$sub_category_id]))) {
                $error = "Invalid subcategory selected.";
            } elseif (!in_array($importance, ['Important', 'Not Important'])) {
                $error = "Invalid importance selected.";
            } elseif (!in_array($urgency, ['Urgent', 'Not Urgent'])) {
                $error = "Invalid urgency selected.";
            } elseif ($priority < 1 || $priority > 5) {
                $error = "Invalid priority selected.";
            } else {
                $stmt = $pdo->prepare("
                    INSERT INTO tasks (
                        user_id, title, description, category_id, sub_category_id, 
                        time_hrs, time_mins, due_date, 
                        importance, urgency, priority, completed, scheduled
                    ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, FALSE, FALSE)
                ");
                $stmt->execute([
                    $user_id, $title, $description ?: null, $category_id, $sub_category_id, 
                    $time_hrs, $time_mins, $due_date, 
                    $importance, $urgency, $priority
                ]);
                $success = "Task added successfully.";
            }
        }

        // Handle category submission via AJAX
        if (isset($_POST['category_submit'])) {
            $category_title = trim($_POST['category_title'] ?? '');
            $category_description = trim($_POST['category_description'] ?? '');
            $parent_id = !empty($_POST['parent_id']) ? (int)$_POST['parent_id'] : null;

            if (empty($category_title)) {
                header('Content-Type: application/json');
                echo json_encode(['success' => false, 'error' => 'Category title is required.']);
                exit;
            }

            $stmt = $pdo->prepare("INSERT INTO categories (user_id, title, description, parent_id) VALUES (?, ?, ?, ?)");
            $stmt->execute([$user_id, $category_title, $category_description ?: null, $parent_id]);
            $new_category_id = $pdo->lastInsertId();

            header('Content-Type: application/json');
            echo json_encode([
                'success' => true,
                'category_id' => $new_category_id,
                'category_title' => $category_title,
                'parent_id' => $parent_id
            ]);
            exit;
        }
    }
} catch (PDOException $e) {
    $error = "Error: " . htmlspecialchars($e->getMessage());
}
?>

<h1>Add Task</h1>

<?php if ($error): ?>
    <p style="color: red;"><?php echo htmlspecialchars($error); ?></p>
<?php endif; ?>
<?php if ($success): ?>
    <p style="color: green;"><?php echo htmlspecialchars($success); ?></p>
<?php endif; ?>

<form method="POST" id="task_form">
    <input type="hidden" name="task_submit" value="1">
    <label for="title">Task Title:</label><br>
    <input type="text" id="title" name="title" required><br><br>

    <label for="description">Description:</label><br>
    <textarea id="description" name="description" rows="4" cols="50"></textarea><br><br>

    <label for="category_id">Category:</label><br>
    <select id="category_id" name="category_id" required>
        <option value="">Select Category</option>
        <?php foreach ($category_options as $id => $title): ?>
            <option value="<?php echo (int)$id; ?>">
                <?php echo htmlspecialchars($title); ?>
            </option>
        <?php endforeach; ?>
    </select>
    <a href="#" id="add_category_link">+ Add Category/Subcategory</a><br><br>

    <label for="sub_category_id">Subcategory:</label><br>
    <select id="sub_category_id" name="sub_category_id">
        <option value="">None</option>
        <!-- Populated by JavaScript -->
    </select><br><br>

    <label for="time_hrs">Time Required:</label><br>
    <input type="number" id="time_hrs" name="time_hrs" min="0" value="0"> Hours 
    <input type="number" id="time_mins" name="time_mins" min="0" max="59" value="0"> Minutes<br><br>

    <label for="due_date">Due Date:</label><br>
    <input type="date" id="due_date" name="due_date"><br><br>

    <label for="importance">Importance:</label><br>
    <select id="importance" name="importance" required>
        <option value="Important" selected>Important</option>
        <option value="Not Important">Not Important</option>
    </select><br><br>

    <label for="urgency">Urgency:</label><br>
    <select id="urgency" name="urgency" required>
        <option value="Urgent" selected>Urgent</option>
        <option value="Not Urgent">Not Urgent</option>
    </select><br><br>

    <label for="priority">Priority:</label><br>
    <select id="priority" name="priority" required>
        <option value="1">1 (Highest)</option>
        <option value="2">2</option>
        <option value="3" selected>3 (Medium)</option>
        <option value="4">4</option>
        <option value="5">5 (Lowest)</option>
    </select><br><br>

    <button type="submit">Add Task</button>
</form>

<!-- Popup Modal -->
<div id="category_modal" style="display: none; position: fixed; top: 0; left: 0; width: 100%; height: 100%; background: rgba(0,0,0,0.5);">
    <div style="background: white; width: 400px; margin: 100px auto; padding: 20px; border-radius: 5px;">
        <h2>Add Category/Subcategory</h2>
        <form id="category_form">
            <input type="hidden" name="category_submit" value="1">
            <label for="category_title">Title:</label><br>
            <input type="text" id="category_title" name="category_title" required><br><br>
            <label for="category_description">Description:</label><br>
            <textarea id="category_description" name="category_description" rows="4" cols="40"></textarea><br><br>
            <label for="parent_id">Parent Category (for Subcategory):</label><br>
            <select id="parent_id" name="parent_id">
                <option value="">None (Main Category)</option>
                <?php foreach ($category_options as $id => $title): ?>
                    <option value="<?php echo (int)$id; ?>">
                        <?php echo htmlspecialchars($title); ?>
                    </option>
                <?php endforeach; ?>
            </select><br><br>
            <button type="submit">Add</button>
            <button type="button" id="close_modal">Cancel</button>
        </form>
    </div>
</div>

<p><a href="/index.php">Back to Dashboard</a></p>

<script>
    const subcategories = <?php echo json_encode($subcategories); ?>;
    const categorySelect = document.getElementById('category_id');
    const subcategorySelect = document.getElementById('sub_category_id');
    const addCategoryLink = document.getElementById('add_category_link');
    const categoryModal = document.getElementById('category_modal');
    const closeModal = document.getElementById('close_modal');
    const categoryForm = document.getElementById('category_form');

    function updateSubcategories() {
        const categoryId = categorySelect.value;
        subcategorySelect.innerHTML = '<option value="">None</option>';
        if (subcategories[categoryId]) {
            for (const [id, title] of Object.entries(subcategories[categoryId])) {
                const option = document.createElement('option');
                option.value = id;
                option.textContent = title;
                subcategorySelect.appendChild(option);
            }
        }
    }

    // Show modal
    addCategoryLink.addEventListener('click', (e) => {
        e.preventDefault();
        categoryModal.style.display = 'block';
    });

    // Close modal
    closeModal.addEventListener('click', () => {
        categoryModal.style.display = 'none';
        categoryForm.reset();
    });

    // Handle category form submission via AJAX
    categoryForm.addEventListener('submit', async (e) => {
        e.preventDefault();
        const formData = new FormData(categoryForm);

        try {
            const response = await fetch('', {
                method: 'POST',
                body: formData
            });
            const result = await response.json();

            if (result.success) {
                // Update category dropdown
                const newOption = document.createElement('option');
                newOption.value = result.category_id;
                newOption.textContent = result.category_title;
                categorySelect.appendChild(newOption);
                categorySelect.value = result.category_id;

                // Update subcategories if added as subcategory
                if (result.parent_id) {
                    if (!subcategories[result.parent_id]) {
                        subcategories[result.parent_id] = {};
                    }
                    subcategories[result.parent_id][result.category_id] = result.category_title;
                    updateSubcategories();
                }

                // Close modal
                categoryModal.style.display = 'none';
                categoryForm.reset();
            } else {
                alert('Error: ' + result.error);
            }
        } catch (err) {
            alert('Error adding category: ' + err.message);
        }
    });

    categorySelect.addEventListener('change', updateSubcategories);
    updateSubcategories(); // Initial call
</script>

<style>
    body { font-family: Arial, sans-serif; margin: 20px; }
    label { display: inline-block; width: 150px; }
    input, select, textarea { margin-bottom: 10px; }
    textarea { vertical-align: top; }
    #add_category_link { margin-left: 10px; text-decoration: none; color: blue; }
    #category_modal button { margin-right: 10px; }
    #category_modal textarea { width: 100%; }
</style>