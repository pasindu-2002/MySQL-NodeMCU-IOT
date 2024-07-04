<?php
// Assuming you have a database connection established already
// Database connection parameters
$host = "localhost"; // Change this to your host if needed
$dbname = "smart_home"; // Change this to your database name
$username = "root"; // Change this to your database username
$password = ""; // Change this to your database password

// Establish a connection to the database
try {
    $pdo = new PDO("mysql:host=$host;dbname=$dbname", $username, $password);
    // Set the PDO error mode to exception
    $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
} catch(PDOException $e) {
    // Handle connection errors
    echo "Connection failed: " . $e->getMessage();
    exit();
}

// Get the parameters sent via POST
$bulbId = $_POST['bulbId'];
$status = $_POST['status'];

// $bulbId = 'bulb2';
// $status = 'on';
$value = 3;

switch ($status) {
  case 'on':
      $value = 1;
      break;
  case 'off':
      $value = 0;
      break;
  default:
      // Handle unexpected status values here
      break;
}

// Update the database based on the received parameters
// Example SQL query
$sql = "UPDATE bulb SET status = :status WHERE id = :id";
$stmt = $pdo->prepare($sql);
$stmt->execute(['status' => $value, 'id' => $bulbId]);

// Send a response back (you can customize the response as needed)
$response = ['success' => true];
echo json_encode($response);
?>
