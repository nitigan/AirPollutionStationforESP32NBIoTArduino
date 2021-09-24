<?php require_once 'db.php';
    $datax[] = array('Year','Humidity');
        $query="select * from data3 where date_time >= DATE_SUB(NOW(),INTERVAL 1 HOUR) LIMIT 0,120;";
        $res=mysqli_query($conn,$query);
        while($data=mysqli_fetch_array($res)){
          $year=date(' H:i',strtotime($data['date_time']));
          $humid=$data['Humidity'];
          $datax[] = array($year,(float)$humid);
        }
    echo json_encode($datax,true);
?>