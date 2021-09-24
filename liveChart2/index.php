<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <!-- The above 3 meta tags *must* come first in the head; any other head content must come *after* these tags -->
    <title>Live Chart Station 2</title>

    <!-- Bootstrap -->
    <!-- Latest compiled and minified CSS -->
    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css"
        integrity="sha384-BVYiiSIFeK1dGmJRAkycuHAHRg32OmUcww7on3RYdg4Va+PmSTsz/K68vbdEjh4u" crossorigin="anonymous">

    <!-- HTML5 shim and Respond.js for IE8 support of HTML5 elements and media queries -->
    <!-- WARNING: Respond.js doesn't work if you view the page via file:// -->
    <!--[if lt IE 9]>
      <script src="https://oss.maxcdn.com/html5shiv/3.7.3/html5shiv.min.js"></script>
      <script src="https://oss.maxcdn.com/respond/1.4.2/respond.min.js"></script>
    <![endif]-->
</head>
<?php
    require_once 'db.php';
    $query="select * from data2 ORDER BY id DESC LIMIT 1;";
    $res=mysqli_query($conn,$query);
    $resultx = $res->fetch_assoc();
?>

<body style="padding: 50px;">
    <div class="container">
    <h2 class="panel-heading"> Station 2</h2>
        <p id="demo"></p>

		<script>
			const d = new Date();
			document.getElementById("demo").innerHTML = d;
		</script>
    
        <div class="row">
            <div class="col-lg-12">
                <ul class="nav nav-pills">
                    <li class="active"><a href="#">PM1<span class="badge"
                                id="txtpm1"><?php echo $resultx['PM1'];?></span></a>
                    </li>
                    <li class="active"><a href="#">PM2.5<span class="badge"
                                id="txtpm2"><?php echo $resultx['PM2'];?></span></a>
                    </li>
                    <li class="active"><a href="#">PM4<span class="badge"
                                id="txtpm4"><?php echo $resultx['PM4'];?></span></a>
                    </li>
                    <li class="active"><a href="#">PM10<span class="badge"
                                id="txtpm10"><?php echo $resultx['PM10'];?></span></a>
                    </li>
                	<li class="active"><a href="#">อุณหภูมิ<span class="badge"
                                id="txttemp"><?php echo $resultx['Temperature'];?></span> °C</a>
                    </li>
                	<li class="active"><a href="#">ความชื้น<span class="badge"
                                id="txthumid"><?php echo $resultx['Humidity'];?></span> %</a>
                	</li>
                	<li class="active"><a href="#">ความเร็วลม<span class="badge"
                                id="txtwindspeed"><?php echo $resultx['Wind_Speed'];?></span> ม./วินาที</a> 
                    </li>
                	<li class="active"><a href="#">ทิศทางลม<span class="badge"
                                id="txtwinddirect"><?php echo $resultx['Wind_dir'];?></span></a>
               		</li>
                </ul>
            </div>
        </div>
        <hr>
        <div class="row">

            <div class="col-lg-3">
                <div class="panel panel-primary">
                    <div class="panel-heading">
                        <h3 class="panel-title">PM1</h3>
                    </div>
                    <div class="panel-body">
                        <div id="curve_chart" style="width: 100%; height: 200px"></div>
                    </div>
                </div>
            </div>

            <div class="col-lg-3">
                <div class="panel panel-primary">
                    <div class="panel-heading">
                        <h3 class="panel-title">PM2.5</h3>
                    </div>
                    <div class="panel-body">
                        <div id="curve_chart2" style="width: 100%; height: 200px"></div>
                    </div>
                </div>
            </div>

            <div class="col-lg-3">
                <div class="panel panel-primary">
                    <div class="panel-heading">
                        <h3 class="panel-title">PM4</h3>
                    </div>
                    <div class="panel-body">
                        <div id="curve_chart3" style="width: 100%; height: 200px"></div>
                    </div>
                </div>
            </div>

            <div class="col-lg-3">
                <div class="panel panel-primary">
                    <div class="panel-heading">
                        <h3 class="panel-title">PM10</h3>
                    </div>
                    <div class="panel-body">
                        <div id="curve_chart4" style="width: 100%; height: 200px"></div>
                    </div>
                </div>
            </div>
        
        	<div class="col-lg-6">
                <div class="panel panel-primary">
                    <div class="panel-heading">
                        <h3 class="panel-title">Temperature</h3>
                    </div>
                    <div class="panel-body">
                        <div id="curve_chart5" style="width: 100%; height: 300px"></div>
                    </div>
                </div>
            </div>
        
        	<div class="col-lg-6">
                <div class="panel panel-primary">
                    <div class="panel-heading">
                        <h3 class="panel-title">Humidity</h3>
                    </div>
                    <div class="panel-body">
                        <div id="curve_chart6" style="width: 100%; height: 300px"></div>
                    </div>
                </div>
            </div>
		
        </div>
    </div>
    <!-- jQuery (necessary for Bootstrap's JavaScript plugins) -->
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.12.4/jquery.min.js"></script>
    <script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
    <!-- Include all compiled plugins (below), or include individual files as needed -->
    <!-- Latest compiled and minified JavaScript -->
    <script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js"
        integrity="sha384-Tc5IQib027qvyjSMfHjOMaLkfuWVxZxUPnCJA7l2mCWNIpG9mGCD8wGNIcPD7Txa" crossorigin="anonymous">
    </script>
    <script type="text/javascript">
    google.charts.load('current', {
        'packages': ['corechart']
    });
    google.charts.setOnLoadCallback(drawChartpm1); //เรียกใช้ chart
    google.charts.setOnLoadCallback(drawChartpm25); //เรียกใช้ chart
    google.charts.setOnLoadCallback(drawChartpm4); //เรียกใช้ chart
    google.charts.setOnLoadCallback(drawChartpm10); //เรียกใช้ chart
    google.charts.setOnLoadCallback(drawCharttemp); //เรียกใช้ chart
    google.charts.setOnLoadCallback(drawCharthumid); //เรียกใช้ chart

    function drawChartpm1() {
        var jsonData = $.ajax({
            url: 'datapm1.php', //ดึงข้อมูล
            dataType: "json",
            async: false,
            success: function(jsonData) {
                var data = google.visualization.arrayToDataTable(jsonData);

                var options = {
                    title: 'Live Chart PM1',
                    curveType: 'function',
                    legend: {
                        position: 'bottom'
                    }
                };


                var chart = new google.visualization.LineChart(document.getElementById('curve_chart'));

                chart.draw(data, options);
            }
        }).responseText;
    }

    function drawChartpm25() {
        var jsonData = $.ajax({
            url: 'datapm25.php', //ดึงข้อมูล
            dataType: "json",
            async: false,
            success: function(jsonData) {
                var data = google.visualization.arrayToDataTable(jsonData);

                var options = {
                    title: 'Live Chart PM2.5',
                    curveType: 'function',
                    legend: {
                        position: 'bottom'
                    }
                };


                var chart = new google.visualization.LineChart(document.getElementById('curve_chart2'));

                chart.draw(data, options);
            }
        }).responseText;
    }

    function drawChartpm4() {
        var jsonData = $.ajax({
            url: 'datapm4.php', //ดึงข้อมูล
            dataType: "json",
            async: false,
            success: function(jsonData) {
                var data = google.visualization.arrayToDataTable(jsonData);

                var options = {
                    title: 'Live Chart PM4',
                    curveType: 'function',
                    legend: {
                        position: 'bottom'
                    }
                };


                var chart = new google.visualization.LineChart(document.getElementById('curve_chart3'));

                chart.draw(data, options);
            }
        }).responseText;
    }

    function drawChartpm10() {
        var jsonData = $.ajax({
            url: 'datapm10.php', //ดึงข้อมูล
            dataType: "json",
            async: false,
            success: function(jsonData) {
                var data = google.visualization.arrayToDataTable(jsonData);

                var options = {
                    title: 'Live Chart PM10',
                    curveType: 'function',
                    legend: {
                        position: 'bottom'
                    }
                };


                var chart = new google.visualization.LineChart(document.getElementById('curve_chart4'));

                chart.draw(data, options);
            }
        }).responseText;
    }
    
    function drawCharttemp() {
        var jsonData = $.ajax({
            url: 'dataTemp.php', //ดึงข้อมูล
            dataType: "json",
            async: false,
            success: function(jsonData) {
                var data = google.visualization.arrayToDataTable(jsonData);

                var options = {
                    title: 'Live Chart Temperature',
                    curveType: 'function',
                    legend: {
                        position: 'bottom'
                    }
                };


                var chart = new google.visualization.LineChart(document.getElementById('curve_chart5'));

                chart.draw(data, options);
            }
        }).responseText;
    }
    
    function drawCharthumid() {
        var jsonData = $.ajax({
            url: 'dataHumid.php', //ดึงข้อมูล
            dataType: "json",
            async: false,
            success: function(jsonData) {
                var data = google.visualization.arrayToDataTable(jsonData);

                var options = {
                    title: 'Live Chart Humidity',
                    curveType: 'function',
                    legend: {
                        position: 'bottom'
                    }
                };


                var chart = new google.visualization.LineChart(document.getElementById('curve_chart6'));

                chart.draw(data, options);
            }
        }).responseText;
    }

    function updateStatus() {
        var jsonData = $.ajax({
            url: 'datalast.php', //ดึงข้อมูล
            dataType: "json",
            async: false,
            success: function(jsonData) {
                console.log(jsonData);
                $("#txtpm1").html(jsonData.PM1);
                $("#txtpm2").html(jsonData.PM2);
                $("#txtpm4").html(jsonData.PM4);
                $("#txtpm10").html(jsonData.PM10);
            	$("#txttemp").html(jsonData.Temperature);
                $("#txthumid").html(jsonData.Humidity);
            	$("#txtwindspeed").html(jsonData.Wind_Speed);
            	$("#txtwinddirect").html(jsonData.Wind_dir);
            }
        }).responseText;
    }
    setInterval(function() {
        drawChartpm1();
        drawChartpm25();
        drawChartpm4();
        drawChartpm10();
    	drawCharttemp();
    	drawCharthumid();
        updateStatus();
    }, 30000); //ตัวกำหนดเวลา
    </script>
</body>

</html>