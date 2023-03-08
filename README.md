Utilisation :
```
make
./webserv <config.conf>
```

Test: (le serveur doit être allumé en local)
```C
# Envoie plusieurs requetes pour stress test le serveur
./stress <nb_of_request>
# Envoie une requete issus d'un fichier text (voir test/post.txt ou test/get.txt)
./request <test/request.txt>
```

methods:
```
GET
POST
DELETE
```

![alt text](https://github.com/gborneGit/webserv/blob/main/webserv_logs.png)

Exemple d'un fichier config:
```
server {
	host			localhost;
	port			3490;
	server_name		default;
	error			err/;
	body_limit		10M;

	location / {
		methods 	GET, POST, DELETE;
		index		index.html;
		root		www/;
		cgi			.php, cgi/php-cgi-ubuntu;
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
