<?php require_once 'db.php';
    $datax[] = array('Year','PM1');
        $query="select * from data2 where date_time >= DATE_SUB(NOW(),INTERVAL 1 HOUR) LIMIT 0,120;";
        $res=mysqli_query($conn,$query);
        while($data=mysqli_fetch_array($res)){
          $year=date('  H:i',strtotime($data['date_time']));
          $pm1=$data['PM1'];
          $datax[] = array($year,(float)$pm1);
        }
    echo json_encode($datax,true);
?>