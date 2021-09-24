<?php
$servername = "localhost";
$username = "wivach";
$password = "BananaLavender14";
$dbname = "data_AJnitigan";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);

if($conn){
	//echo "db Connection Success";
}else{
    echo "Connection Failed";
}
?>