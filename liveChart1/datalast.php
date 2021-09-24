<?php require_once 'db.php';
        $query="select * from data1 ORDER BY id DESC LIMIT 1;";
        $res=mysqli_query($conn,$query);
        $resultx = $res->fetch_assoc();
        echo json_encode($resultx);
?>