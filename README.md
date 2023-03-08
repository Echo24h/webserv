<p align="center" display="flex" >
  <img height="250" src="https://github.com/gborneGit/gborneGit/blob/main/webserv_logs.png" alt="Logs console Webserv"/>
  <img height="250" src="https://github.com/gborneGit/gborneGit/blob/main/webserv_home.png" alt="Page web prédéfinie Webserv"/>
</p>

## Utilisation:
```Makefile
# Compile le projet
make

# Démarre le serveur (le fichier config est présenté ci-dessous)
./webserv <config.conf>

# Pour une config host=localhost & port=3490 entrez cette adresse dans le navigateur:
http://localhost:3490
```

## Test:
```Makefile
# Pour envoyé une requette POST à l'adresse localhost:3490
# nb = 50 envoie 50 requetes POST, nb = 1 par défaut
./request <test/post.txt> <nb>
```
D'autres requetes HTTP sont présentents au format `.txt` dans le dossiers 'test/`pour examples

## Methodes:
```
GET
POST
DELETE
```

## Config:
__(ATTENTION supprimez les commentaire si Copier/Coller)__

```cnf
server {
	host			localhost;	// Adresse du serveur
	port			3490;		// Port ouvert du serveur
	server_name		default;	// Nom du serveur
	error			err/;		// Dossier contenant le template error
	body_limit		10M;		// Taille MAX du corp de requete


	location / {				// Définie une route
		methods 	GET, POST, DELETE;	// Méthodes permises
		index		index.html;			// Index
		root		www/;				// Root
		cgi			.php, cgi/php-cgi-ubuntu;	// L'extension du cgi et son chemin
	}

	location /data {
		methods 	GET, POST, DELETE;
		index		index.html;
		root		test/;
	}
}
```

## Ressources
***
* [Variables d'environnement CGI](https://fr.wikipedia.org/wiki/Variables_d%27environnement_CGI)
* [mime.types](https://github.com/nginx/nginx/blob/master/conf/mime.types)
* [Liste des codes HTTP](https://fr.wikipedia.org/wiki/Liste_des_codes_HTTP)

* [Serveur HTTP 1](https://ncona.com/2019/04/building-a-simple-server-with-cpp/) Un serveur HTTP simple en C++
* [Serveur HTTP 2](https://github.com/Dungyichao/http_server/blob/master/README.md#1-basic-knowledge-) Connaissances basique

* [Sockets 1](http://vidalc.chez.com/lf/socket.html)
* [Sockets 2](https://www.binarytides.com/multiple-socket-connections-fdset-select-linux/) Connexion multiple avec 'select()'
* [Sockets 3](https://www.ibm.com/docs/en/i/7.3?topic=programming-how-sockets-work) Fonctionnement des sockets
