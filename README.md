## Utilisation:
```Makefile
# Compile le projet
make
# Démarre le serveur (le fichier config est présenté ci-dessous)
./webserv <config.conf>
```

## Test:
```Makefile
# Pour envoyé une requette POST au serveur à l'adresse localhost:3490
# nb__of_request = 1 si non défini
./request <test/post.txt> <nb_of_request>
```

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
		cgi			.php, cgi/php-cgi-ubuntu;
	}
}

server {
	host			localhost;
	port			3491;
	server_name		bis;
	error			err/;
	body_limit		50K;

	location / {
		methods 	GET;
		index		index.html;
		root		www/;
		cgi			.php, cgi/php-cgi-ubuntu;
	}
}
```

![alt text](https://github.com/gborneGit/webserv/blob/main/webserv_logs.png)

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
