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
		<div class="list">
		<?php
			if ($handle = opendir('uploads')) {

				while (false !== ($entry = readdir($handle))) {

					if ($entry != "." && $entry != "..") {
					?>
						<div class="element">
							<a href="delete.php?delete=<?php echo $entry ?>">
								<img src="img/trash.svg" alt="corbeille" class="trash" width="20px" height="20px" >
							</a>
							<p><?php echo $entry; ?></p>
						</div>
					<?php
					}
				}

				closedir($handle);
			}
		?>
		</div>
		<div id="top" class="container">
			<img src="img/logo.png" alt="Logo Webserv" width="200px" height="200px">
		</div>
		<div class="container">
			<form action="upload.php" method="post" enctype="multipart/form-data">
				<input type="file" name="file[]" multiple>
				<div class="text">
					<p>Drag your files here or click in this area.</p>
					<p>(use 'Ctrl' for multiple selection)</p>
				</div>
				<button type="submit" name="submit">Upload</button>
			</form>
		</div>
		<div class="container">
		</div>
	</div>
</body>
</html>
