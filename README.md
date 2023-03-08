<p align="center" display="flex" >
  <img height="250" src="https://github.com/gborneGit/gborneGit/blob/main/webserv_logs.png" alt="Logs console Webserv"/>
  <img height="250" src="https://github.com/gborneGit/gborneGit/blob/main/webserv_home.png" alt="Page web prédéfinie Webserv"/>
</p>

# Serveur HTTP en C++98 inspiré de NGINX

Webserv est un serveur HTTP écrit en C++98 inspiré de NGINX. Le serveur prend en charge les méthodes HTTP GET, POST et DELETE et peut être configuré pour gérer plusieurs connexions simultanément. 

## Utilisation:

Pour utiliser Webserv, il vous suffit de compiler le projet en exécutant la commande suivante :
```
make
```
Vous pouvez ensuite démarrer le serveur en exécutant la commande suivante, en remplaçant `<config.conf>` par le chemin vers le fichier de configuration de votre serveur :
```
./webserv <config.conf>
```

Pour une config host=localhost & port=3490 entrez cette adresse dans le navigateur
```
http://localhost:3490
```

## Test:

Webserv est livré avec un mini programme pour envoyer des requêtes au serveur. Vous pouvez l'utiliser pour tester le fonctionnement du serveur en envoyant plusieurs requêtes POST à l'adresse `localhost:3490`. Voici comment l'utiliser :
```
./request test/post.txt 50
```
Ceci enverra 50 requêtes POST à l'adresse `localhost:3490`. Vous pouvez également utiliser d'autres fichiers de requêtes HTTP au format `.txt` qui se trouvent dans le dossier `test/`. 

## Methodes:

Webserv prend en charge les méthodes HTTP suivantes :
```
GET
POST
DELETE
```

## Config:

La configuration de Webserv se fait via des fichiers `.conf`, qui sont stockés dans le dossier `conf/` par défaut. Vous pouvez configurer l'adresse et le port du serveur, le nom du serveur, le dossier contenant les pages d'erreur, la taille maximale du corps de requête et les routes disponibles.

Voici un exemple de fichier de configuration :

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
