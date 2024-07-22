<!doctype html>
<html>
<head>
<meta charset="utf-8">
<title>無題ドキュメント</title>
</head>

<body>
	<?php
		$command = "/usr/bin/python3 /home/takuya/Electric/arduino_serial.py";
		exec($command, $output);
	?>
	エクスポテンシャル信号が出力されました．
</body>
</html>