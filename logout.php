<?php
session_start();
require "/home1/autotime/public_html/globals/config.inc";

// Debugging
file_put_contents('/home1/autotime/public_html/debug_logout.log', "Logging out at " . date('Y-m-d H:i:s') . "\n", FILE_APPEND);

$_SESSION = [];
session_destroy();
header("Location: /login.php");
exit;
?>