<?php
require "/home1/autotime/public_html/globals/config.inc";
ob_start();
file_put_contents('/home1/autotime/public_html/debug.log', date('Y-m-d H:i:s') . " - login.inc loaded\n", FILE_APPEND);

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    if (isset($_POST['login'])) {
        $username = $_POST['username'];
        $password = $_POST['password'];
        try {
            $stmt = $pdo->prepare("SELECT * FROM users WHERE username = ?");
            $stmt->execute([$username]);
            $user = $stmt->fetch(PDO::FETCH_ASSOC);
            if ($user) {
                $hash = $user['password'];
                if (password_verify($password, $hash)) {
                    $_SESSION['user_id'] = $user['uid'];
                    file_put_contents('/home1/autotime/public_html/debug.log', date('Y-m-d H:i:s') . " - Login successful for $username, redirecting\n", FILE_APPEND);
                    header("Location: /index.php");
                    ob_end_flush();
                    exit;
                } else {
                    echo "Password verification failed. Entered: $password, Stored hash: $hash, Length: " . strlen($hash);
                    file_put_contents('/home1/autotime/public_html/debug.log', date('Y-m-d H:i:s') . " - Password failed for $username\n", FILE_APPEND);
                }
            } else {
                echo "User not found: $username";
                file_put_contents('/home1/autotime/public_html/debug.log', date('Y-m-d H:i:s') . " - User not found: $username\n", FILE_APPEND);
            }
        } catch (PDOException $e) {
            echo "Database error: " . $e->getMessage();
            file_put_contents('/home1/autotime/public_html/debug.log', date('Y-m-d H:i:s') . " - Database error: " . $e->getMessage() . "\n", FILE_APPEND);
        }
    } elseif (isset($_POST['signup'])) {
        $username = $_POST['username'];
        $password = password_hash($_POST['password'], PASSWORD_DEFAULT);
        try {
            $stmt = $pdo->prepare("INSERT INTO users (username, password) VALUES (?, ?)");
            $stmt->execute([$username, $password]);
            echo "User created! Please log in.";
            file_put_contents('/home1/autotime/public_html/debug.log', date('Y-m-d H:i:s') . " - Signup successful for $username\n", FILE_APPEND);
        } catch (PDOException $e) {
            echo "Signup failed: " . $e->getMessage();
            file_put_contents('/home1/autotime/public_html/debug.log', date('Y-m-d H:i:s') . " - Signup failed: " . $e->getMessage() . "\n", FILE_APPEND);
        }
    }
}
ob_end_flush();
?>
<form method="post">
    Username: <input type="text" name="username"><br>
    Password: <input type="password" name="password"><br>
    <input type="submit" name="login" value="Login">
    <input type="submit" name="signup" value="Sign Up">
</form>