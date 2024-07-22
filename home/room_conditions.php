<!doctype html>
<html>
<head>
<meta charset="utf-8">
<title>home HK | room_conditions</title>
<link rel="stylesheet" href="../css/normalize.css">
<link rel="stylesheet" href="../css/style.css">
    <link rel="stylesheet" href="style.css">
</head>

<body>
    <header>
  		<h1>Takuyaのホームページ</h1>
		<nav>
   			<ul>
      			<li><a href="../index.html">HOME</a></li>
      			<li><a href="../home/room_conditions.php">家のHKデータ</a></li>
      			<li><a href="../satellite/satellite.html">衛星軌道</a></li>
      			<li><a href="../BBS/BBS.php">BBS</a></li>
    		</ul>
  		</nav>
	</header>
    <main>
	<?php
    
    $pdo = new PDO('mysql:host=localhost;dbname=homeHK;charset=utf8', 'user_homeHK', 'qAfbUr3rD7i44UihR');
    $workroom_record = $pdo->query('select * from BME680_workroom order by date_and_time desc limit 1');
    $workroom_data = $workroom_record->fetch();	
    
    $outside_record = $pdo->query('select * from BME680_outside order by date_and_time desc limit 1');
    $outside_data = $outside_record->fetch();	
	
#	$command = "/usr/bin/python3 door_key.py 2>&1";
#	exec($command, $door);
#	$command = "cat /sys/class/thermal/thermal_zone0/temp 2>&1";
#	exec($command, $cpu_temp);
	echo "作業部屋
        <table border=\"1\">
			<tr>
				<th>Time</th>
				<td>$workroom_data[0] JST</td>
			</tr>
			<tr>
				<th>Temperature</th>
				<td>$workroom_data[1] degC</td>
			</tr>
			<tr>
				<th>Pressure</th>
				<td>$workroom_data[2] Pa</td>
			</tr>
			<tr>
				<th>Pressure at sea level</th>
				<td>$workroom_data[3] Pa</td>
			</tr>
			<tr>
				<th>Humidity</th>
				<td>$workroom_data[4] %</td>
			</tr>
        </table>
        <span class=\"outside-temp\">
            屋外
        <table border=\"1\">
            <tr>
				<th>Time</th>
				<td>$outside_data[0] JST</td>
			</tr>
			<tr>
				<th>Temperature</th>
				<td>$outside_data[1] degC</td>
			</tr>
			<tr>
				<th>Pressure</th>
				<td>$outside_data[2] Pa</td>
			</tr>
			<tr>
				<th>Pressure at sea level</th>
				<td>$outside_data[3] Pa</td>
			</tr>
			<tr>
				<th>Humidity</th>
				<td>$outside_data[4] %</td>
			</tr>
        </table>
        </span>
            ";
    
    $command1 = "/usr/bin/python3 /var/www/html/home/plot_temp_last24h.py 2>&1";
    exec($command1, $output, $result);
    echo "
    <img src=\"temp/temperature_last24H_temp.jpg\" alt=\"temperature history\" width=\"640\">
    ";
#			<tr>
#				<th>Door key</th>
#				<td>";
#	
#	if ($door[0] == 1)
#		echo "close";
#	else
#		echo "open";
#	
#	echo "</td>
#			</tr>
#			<tr>
#				<th>CPU temperature</th>
#				<td>";
#	echo (int)$cpu_temp[0]/1000;
#	echo " degC</td>
#			</tr>
#		</table>";
	?>
    </main>
    <footer>
		
		<ul>
			<li><a href="index.html">トップページ</a></li>
		</ul>
		<p>2023 Takuyaのホームページ</p>
	</footer>
</body>
</html>
