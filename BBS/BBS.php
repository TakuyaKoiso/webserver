<!doctype html>
<html>
<head>
<meta charset="utf-8">
<title>BBS</title>
</head>

<body>
	<header>
		<h1>BBS</h1>
	</header>
	
	<p>メッセージを記入してください</p>
	<form action='BBS.php' method=post>
		<input type="text" name="BBS">
		<input type="submit" value="post">
	</form>
	
	<?php
		date_default_timezone_set("Asia/Tokyo");
	
		$file = "BBS.txt";
		$log = array();
	
		if (file_exists($file)) {
			$log = json_decode(file_get_contents($file));
		}
		
		if (isset($_REQUEST["BBS"])) {
			array_unshift($log, $_REQUEST["BBS"]);
			array_unshift($log, date('Y/m/d H:i:s'));		

			file_put_contents($file, json_encode($log));
		}
	
		print("<p>*-*-*-*-*-* 過去の投稿 *-*-*-*-*-*</p>");
	
		for ($i = 0; $i < count($log); $i++) {
			echo "<p>$log[$i]<br>" ;
			$i++;
			echo "$log[$i]<br>--------------<p>";
		}
	?>
</body>
</html>