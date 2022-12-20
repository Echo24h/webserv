<?php
	require("session.class.php");
	$session = new Session();

	$target = $_GET['delete'];

	if(unlink("uploads/" . $target))
		$session->setFlash("The file ". $target . " has been deleted.", 'info');
	else
		$session->setFlash("File " . $target . " not found");
?>

<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8">
	<meta http-equiv="X-UA-Compatible" content="IE=edge">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<title>WebServ</title>

	<link rel="stylesheet" href="style.css">

</head>
<body>
	<div id="main">
		<div class="container">
			<a href="index.php" class="back"><<</a>
		</div>
		<div id="top" class="container">
			<img src="img/logo.png" alt="Logo Webserv" width="200px" height="200px">
		</div>
		<div class="container">
			<?php
				$session->flash();
			?>
		</div>
	</div>
</body>
</html>
