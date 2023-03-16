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
		<div id="top" class="container">
			<img src="img/logo.png" alt="Logo Webserv" width="200px" height="200px">
		</div>
		<div class="container">

			<div id="container-cookie">

				<div class="list">
					<?php foreach($_COOKIE as $name => $value) { ?>
						<div class="cookie-element">
						<img src="img/cookie.svg" alt="cookie" class="cookie" width="25px" height="25px" >
						<p><?php echo "{$name} = {$value}"; ?></p>
						</div>
					<?php } ?>
				</div>

				<form id="cookie-form">
					<label for="cookieName">Cookie name:</label>
					<input type="text" id="cookieName" name="cookieName"><br>

					<label for="cookieValue">Cookie value:</label>
					<input type="text" id="cookieValue" name="cookieValue"><br>

					<input type="submit" value="Create cookie">
				</form>
			</div>

			<div id="container-upload">
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

				<form id="upload-form" action="upload.php" method="post" enctype="multipart/form-data">
					<input type="file" name="file[]" multiple>
					<div class="text">
						<p>Drag your files here or click in this area.</p>
						<p>(use 'Ctrl' for multiple selection)</p>
					</div>
					<button type="submit" name="submit">Upload</button>
				</form>
			</div>
			<script>
				// Ajoute un écouteur d'événements pour le soumission du formulaire
				document.querySelector('#cookie-form').addEventListener('submit', function(event) {
					// Empêche le comportement par défaut de soumission du formulaire
					event.preventDefault();

					// Récupère les valeurs des champs de formulaire
					const cookieName = document.getElementById('cookieName').value;
					const cookieValue = document.getElementById('cookieValue').value;

					// Crée le cookie en utilisant les valeurs récupérées
					document.cookie = `${cookieName}=${cookieValue}; path=/;`;

					// Affiche un message pour indiquer que le cookie a été créé
					alert(`Cookie ${cookieName} submit success: ${cookieValue}.`);

					// Efface les valeurs des champs de formulaire
					document.getElementById('cookieName').value = '';
					document.getElementById('cookieValue').value = '';
				});
			</script>
		</div>
		<div class="container">
		</div>
	</div>
</body>
</html>
