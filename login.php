<?php
session_start();
require "/home1/autotime/public_html/globals/config.inc";

// Debugging
ini_set('display_errors', 1);
error_reporting(E_ALL);
file_put_contents('/home1/autotime/public_html/debug_login.log', "Accessing login.php at " . date('Y-m-d H:i:s') . "\n", FILE_APPEND);

if (isset($_SESSION['user_id'])) {
    file_put_contents('/home1/autotime/public_html/debug_login.log', "User already logged in, redirecting to index.php\n", FILE_APPEND);
    header("Location: /index.php");
    exit;
}

// Include login.inc
include "/home1/autotime/public_html/includes/login.inc";
?>

<style>
    body { font-family: Arial, sans-serif; margin: 20px; }
    label { display: inline-block; width: 100px; }
    input[type="text"], input[type="password"] { width: 200px; padding: 5px; }
</style>