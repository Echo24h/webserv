#!/usr/bin/python3

import os

# Entête CGI indiquant le type de contenu
print("Content-type: text/html\n\n")

# Corps de la réponse CGI
print("<html>")
print("<head>")
print("<title>Affichage des cookies</title>")
print("</head>")
print("<body>")
print("<h1>Affichage des cookies</h1>")

# Récupération des cookies
cookies = os.environ.get("HTTP_COOKIE")

if cookies:
    print("<h2>Cookies :</h2>")
    print("<ul>")
    cookie_list = cookies.split(";")
    for cookie in cookie_list:
        cookie = cookie.strip()
        print("<li>{}</li>".format(cookie))
    print("</ul>")
else:
    print("<p>Aucun cookie.</p>")

print("</body>")
print("</html>")