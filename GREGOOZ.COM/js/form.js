var nameError = document.getElementById('name-error');
var phoneError = document.getElementById('phone-error');
var emailError = document.getElementById('email-error');
var messageError = document.getElementById('message-error');
var submitError = document.getElementById('submit-error');

function validateName() {
	var name = document.getElementById('name').value;
	if (name.length == 0)
	{
		nameError.innerHTML = 'Nom & Prénom requis';
		return false;
	}
	nameError.innerHTML = '<i class="bx bxs-check-circle"></i>';
	return true;

}

function validatePhone() {
	var phone = document.getElementById('tel').value;

	phone = phone.replace(/\s+/g, '');

	if (phone.length == 0)
	{
		phoneError.innerHTML = 'Téléphone requis';
		return false;
	}
	if (!phone.match(/^[0-9]/))
	{
		phoneError.innerHTML = 'Uniquement des chiffres svp';
		return false;
	}
	if (phone.length !== 10)
	{
		phoneError.innerHTML = '10 chiffres requis';
		return false;
	}
	if (!phone.match(/^[0-9]{10}$/))
	{
		phoneError.innerHTML = 'Uniquement des chiffres svp';
		return false;
	}
	phoneError.innerHTML = '<i class="bx bxs-check-circle"></i>';
	return true;
}

function validateEmail() {
	var email = document.getElementById('email').value;

	if (email.length == 0)
	{
		emailError.innerHTML = 'Email requis';
		return false;
	}
	if (!email.match(/^[A-Za-z\._\-[0-9]*[@][A-Za-z]*[\.][a-z]{2,4}$/))
	{
		emailError.innerHTML = 'Email non valide';
		return false;
	}
	emailError.innerHTML = '<i class="bx bxs-check-circle"></i>';
	return true;
}

function validateMessage() {
	var message = document.getElementById('projet').value;
	var required = 30;
	var left = required - message.length;

	if (left > 0)
	{
		messageError.innerHTML = left + ' caractère(s) supplémentaire(s) requis';
		return false;
	}
	messageError.innerHTML = '<i class="bx bxs-check-circle"></i>';
	return true;
}

function resetForm() {
	document.getElementById('name').value = '';
	document.getElementById('email').value = '';
	document.getElementById('tel').value = '';
	document.getElementById('projet').value = '';
}

function resultForm( message ) {
	if(message.localeCompare("OK") == 0)
		alert("Demande envoyée! Réponse sous 72h.");
	else
		alert(message);
}

function validateForm() {
	if(!validateName() || !validatePhone() || !validateEmail() || !validateMessage())
	{
		submitError.style.color = "red";
		submitError.style.display = 'block';
		submitError.innerHTML = "Veuillez corriger l'erreur svp";
		setTimeout(function(){submitError.style.display = 'none';}, 3000);
		return false;
	}
	Email.send({
		Host : "smtp.elasticemail.com",
		Username : "gregborne.pro@gmail.com",
		Password : "3A8EAB6C0696F6A8BED2C4253AD14960E920",
		To : 'contact@gregooz.com',
		From : "contact@gregooz.com",
		Subject : "Demande de devis: " + document.getElementById('name').value,
		Body : '<b>Nom</b>: ' + document.getElementById('name').value
			+ '<br/><b>Email</b>: ' + document.getElementById('email').value
			+ '<br/><b>Télephone</b>: ' + document.getElementById('tel').value
			+ '<br/><br/><b>Message</b>: ' + document.getElementById('projet').value,
	}).then(
			message => resultForm(message)
	);
	resetForm();
	submitError.style.color = "green";
	submitError.style.display = 'block';
	submitError.innerHTML = "Demande envoyée !";
	return false;
}
