<?php
    $json = file_get_contents('php://input');
    $data = json_decode($json, false);
    
    $mysqli = new mysqli("localhost","wivach","BananaLavender14","data_AJnitigan");
    //$mysqli = new mysqli("10.100.2.12","root","DNEdmt37261","data_AJnitigan");
    $mysqli -> set_charset("utf8");
    $sql = "INSERT INTO `data3`(`Station`,`PM1`, `PM2.5`, `PM10`, `CO`, `NO2`, `SO2`, `O3`, `Temperature`, `Humidity`, `Wind_Speed`, `Wind_dir`) VALUES ('$data->Station','$data->PM1','$data->PM2','$data->PM10','$data->CO','$data->NO2','$data->SO2','$data->O3','$data->Temp','$data->Humi','$data->WindSp','$data->WindDir')";
    $mysqli ->query($sql);
?>