<?php
session_start();
require "/home1/autotime/public_html/globals/config.inc";

header('Content-Type: application/json');

if (!isset($_SESSION['user_id'])) {
    echo json_encode([]);
    exit;
}

$user_id = $_SESSION['user_id'];
$category_id = (int)($_POST['category_id'] ?? 0);

try {
    $stmt = $pdo->prepare("SELECT id, title FROM categories WHERE user_id = ? AND parent_id = ? ORDER BY title");
    $stmt->execute([$user_id, $category_id]);
    $subcategories = $stmt->fetchAll(PDO::FETCH_ASSOC);
    echo json_encode($subcategories);
} catch (PDOException $e) {
    echo json_encode([]);
}
?>