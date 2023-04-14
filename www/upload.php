<?php
require("session.class.php");
$session = new Session();

$target_dir = "uploads/";

// Check if image file is a actual image or fake image
if(isset($_POST["submit"])) {

	$countfiles = count($_FILES["file"]["name"]);

	//if ($countfiles > 5)
	//	$session->setFlash("Sorry, there was an error uploading your file.");


	for($i = 0; $i < $countfiles; $i++) {

		$target_file = $target_dir . basename($_FILES["file"]["name"][$i]);
		$imageFileType = strtolower(pathinfo($target_file,PATHINFO_EXTENSION));

		// Check if file is an image
		$check = getimagesize($_FILES["file"]["tmp_name"][$i]);
		if($check !== false) {

			// Check if file already exists
			if (!file_exists($target_file)) {

				// Check file size
				if ($_FILES["file"]["size"][$i] <= 50000000) {

					// Allow certain file formats
					if($imageFileType == "jpg" || $imageFileType == "png" || $imageFileType == "jpeg"
					|| $imageFileType == "gif" ) {

						if (move_uploaded_file($_FILES["file"]["tmp_name"][$i], $target_file)) {
							$session->setFlash("The file ". htmlspecialchars( basename( $_FILES["file"]["name"][$i])). " has been uploaded.", 'info');
						}
						else {
							$session->setFlash("Sorry, there was an error uploading your file.");
						}
					}
					else {
						$session->setFlash("Sorry, only JPG, JPEG, PNG & GIF files are allowed.");
					}
				}
				else {
					$session->setFlash("Sorry, your file is too large.");
				}
			}
			else {
				$session->setFlash("Sorry, file '" . $target_file . "' already exists.");
			}
		}
		else {
			$session->setFlash("File is not an image or too big.");
		}
	}
}
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
			<?php $session->flash(); ?>
		</div>
	</div>
</body>
</html>
