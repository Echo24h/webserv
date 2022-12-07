// Get the modal
var modal = document.getElementById("modal-contact");
var modal_project_pimpmywatch = document.getElementById("modal-project-pimpmywatch");
var modal_project_lamassagere = document.getElementById("modal-project-lamassagere");
var modal_project_gregooz = document.getElementById("modal-project-gregooz");
var modal_project_student = document.getElementById("modal-project-student");
var modal_project_sharko = document.getElementById("modal-project-sharko");
var modal_mentions = document.getElementById("modal-mentions");
var modal_politique = document.getElementById("modal-politique");

// Get the <span> element that closes the modal
var span = document.getElementsByClassName("close");

// When the user clicks on <span> (x), close the modal
span[0].onclick = function () {
	modal.style.display = "none";
}

span[1].onclick = function () {
	modal_project_pimpmywatch.style.display = "none";
}

span[2].onclick = function () {
	modal_project_lamassagere.style.display = "none";
}

span[3].onclick = function () {
	modal_project_gregooz.style.display = "none";
}

span[4].onclick = function () {
	modal_project_student.style.display = "none";
}

span[5].onclick = function () {
	modal_project_sharko.style.display = "none";
}

span[6].onclick = function () {
	modal_mentions.style.display = "none";
}

span[7].onclick = function () {
	modal_politique.style.display = "none";
}

// When the user clicks anywhere outside of the modal, close it
window.onclick = function (event) {
	if (event.target == modal) {
		modal.style.display = "none";
	}
	if (event.target == modal_project_pimpmywatch) {
		modal_project_pimpmywatch.style.display = "none";
	}
	if (event.target == modal_project_lamassagere) {
		modal_project_lamassagere.style.display = "none";
	}
	if (event.target == modal_project_gregooz) {
		modal_project_gregooz.style.display = "none";
	}
	if (event.target == modal_project_student) {
		modal_project_student.style.display = "none";
	}
	if (event.target == modal_project_sharko) {
		modal_project_sharko.style.display = "none";
	}
	if (event.target == modal_mentions) {
		modal_mentions.style.display = "none";
	}
	if (event.target == modal_politique) {
		modal_politique.style.display = "none";
	}
}

function modalContact() {
	modal.style.display = "block";
}

function modalProjectPimpmywatch() {
	modal_project_pimpmywatch.style.display = "block";
}

function modalProjectLamassagere() {
	modal_project_lamassagere.style.display = "block";
}

function modalProjectGregooz() {
	modal_project_gregooz.style.display = "block";
}

function modalProjectStudent() {
	modal_project_student.style.display = "block";
}

function modalProjectSharko() {
	modal_project_sharko.style.display = "block";
}

function modalMentions() {
	modal_mentions.style.display = "block";
}

function modalPolitique() {
	modal_politique.style.display = "block";
}
