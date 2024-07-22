<!doctype html>
<html>
<head>
<meta charset="utf-8">
<title>php_test</title>
</head>

<body>
	<?php
		$pdo = new PDO('mysql:host=localhost;dbname=test_db;charset=utf8','takuya', 'navier0928miwa');
	
		foreach ($pdo->query('select * from product') as $row) {
			echo '<p>';
			echo $row['id'], ' : ';
			echo $row['name'], ' : ';
			echo $row['price'];
			echo '</p>';
		}
	?>
</body>
</html>