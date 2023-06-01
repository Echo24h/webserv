document.getElementById('delete-form').addEventListener('submit', function(event) {
    event.preventDefault();

    var fileName = document.getElementById('file-name').value;
    var url = '/upload/' + encodeURIComponent(fileName);

    fetch(url, {
      method: 'DELETE'
    })
    .then(function(response) {
      if (response.ok) {
        console.log('Le fichier ' + fileName + ' a été supprimé avec succès.');
      } else {
        console.log('Une erreur s\'est produite lors de la suppression du fichier ' + fileName + '.');
      }
    })
    .catch(function(error) {
      console.log('Une erreur s\'est produite lors de la suppression du fichier ' + fileName + ': ' + error.message);
    });
  });