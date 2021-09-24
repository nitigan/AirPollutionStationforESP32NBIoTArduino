<?php require_once 'db.php';
    $datax[] = array('Year','PM2.5');
        $query="select * from data2 where date_time >= DATE_SUB(NOW(),INTERVAL 1 HOUR) LIMIT 0,120;";
        $res=mysqli_query($conn,$query);
        while($data=mysqli_fetch_array($res)){
          $year=date(' H:i',strtotime($data['date_time']));
          $pm2=$data['PM2'];
          $datax[] = array($year,(float)$pm2);
        }
    echo json_encode($datax,true);
?>