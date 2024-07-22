<!doctype html>
<html>
<head>
<meta charset="utf-8">
<title>QZS-1R | satellite track</title>
</head>

<body>
	<p>準天頂衛星　みちびき初号機後継機（QZS-1R）</p>
	<?php
		date_default_timezone_set('Asia/Tokyo');
		echo "<p>Time : ", date("Y/m/d H:i:s"), " (JST)</p>";
	
		$pdo = new PDO('mysql:host=localhost;dbname=sat_TLE;charset=utf8', 'user_sat_TLE', '9ZnSZMUEN#');
		$TLE_record = $pdo->query('select Epoch,First_derivative,Inclination,Ascension,Eccentricity,Perigee,Anomaly,Motion from QZS1R order by Epoch desc limit 1');
		$TLE_data = $TLE_record->fetch();	
			
		$command = "/var/www/html/satellite/location_from_tle.exe $TLE_data[0] $TLE_data[1] $TLE_data[2] $TLE_data[3] $TLE_data[4] $TLE_data[5] $TLE_data[6] $TLE_data[7]";
		exec($command, $output, $result);
		if($output[6] < 0) {
			echo sprintf("Latitude : %.2f S<br>", -$output[6]);
		} else {
			echo sprintf("Latitude : %.2f N<br>", $output[6]);
		}
	
		if($output[7] < 0) {
			echo sprintf("Longitude : %.2f W<br>", -$output[7]);
		} else {
			echo sprintf("Longitude : %.2f E<br>", $output[7]);
		}
		
		echo sprintf("Altitude : %.1f km<br>", $output[8]);
		echo "<br>";
		echo sprintf("Using TLE data at %04d/%02d/%02d %02d:%02d:%02d (JST) for calculation.<br>", $output[0], $output[1], $output[2], $output[3], $output[4], $output[5]);
		echo sprintf("\n<iframe width=\"750\" height=\"650\" src=\"http://maps.google.co.jp/maps?ll=%.5f, %.5f&amp;q=%.5f, %.5f&amp;output=embed&amp;t=m&amp;z=3&amp;hl=ja\" frameborder=\"0\" marginwidth=\"0\" marginheight=\"0\" scrolling=\"no\"></iframe>", $output[6], $output[7], $output[6], $output[7])
	?>
</body>
</html>