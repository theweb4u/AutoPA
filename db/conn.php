<?
//opening  connection and selecting the treasures database
$conn = new mysqli("127.0.0.1", "autotime_tm", "56#09-a9^aG.");

// Check connection
if ($conn->connect_error) {
  die("Connection failed: " . $conn->connect_error);
}else{
	$conn -> select_db("autotime_tm");
}
?>
