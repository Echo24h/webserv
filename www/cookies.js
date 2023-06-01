var fileInput = document.getElementById('file-input');
var uploadButton = document.getElementById('upload-button');
var messageContainer = document.getElementById('message-container');

uploadButton.addEventListener('click', function() {
    var files = fileInput.files;
    var formData = new FormData();

    for (var i = 0; i < files.length; i++) {
    formData.append('file[]', files[i]);
    }

    var xhr = new XMLHttpRequest();
    xhr.open('POST', 'upload');
    xhr.onload = function() {
    if (xhr.status === 204 || xhr.status === 200) {
        showMessage('Files uploaded successfully.', 'success');
    } else {
        showMessage('Error uploading files.', 'error');
    }
    };
    xhr.send(formData);
});

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

function showMessage(message, type) {
    var messageElement = document.createElement('div');
    messageElement.classList.add('message');
    messageElement.classList.add(type);
    messageElement.textContent = message;

    messageContainer.appendChild(messageElement);

    setTimeout(function() {
    messageContainer.removeChild(messageElement);
    }, 3000);
}