<?php require_once 'db.php';
    $datax[] = array('Year','Temperature');
        $query="select * from data3 where date_time >= DATE_SUB(NOW(),INTERVAL 1 HOUR) LIMIT 0,120;";
        $res=mysqli_query($conn,$query);
        while($data=mysqli_fetch_array($res)){
          $year=date('H:i',strtotime($data['date_time']));
          $temp=$data['Temperature'];
          $datax[] = array($year,(float)$temp);
        }
    echo json_encode($datax,true);
?>