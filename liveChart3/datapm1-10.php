<?php require_once 'db.php';
    $datax[] = array('Year','PM1','PM2.5','PM4','PM10');
        $query="select * from data2 where date_time >= DATE_SUB(NOW(),INTERVAL 1 HOUR) LIMIT 0,120;";
        $res=mysqli_query($conn,$query);
        while($data=mysqli_fetch_array($res)){
          $year=date(' H:i',strtotime($data['date_time']));
          $pm1=$data['PM1'];
          $pm2=$data['PM2.5'];
          $pm4=$data['PM4'];
          $pm10=$data['PM10'];
          $datax[] = array($year,(int)$pm1,(int)$pm2,(int)$pm4,(int)$pm10);
        }
    echo json_encode($datax,true);
?>