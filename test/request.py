#!/usr/bin/python3

import requests

# URL cible pour la requête
url = "http://localhost:3498/-1"

# Paramètres de la requête (si nécessaire)
params = {
    "timeout": "5"
}

# En-têtes de la requête (si nécessaire)
headers = {
    "User-Agent": "-1",
    "Content-Lenght": "-1"
}

# Effectuer une requête GET
response = requests.get(url, params=params, headers=headers)

# Vérifier le code de statut de la réponse
if response.status_code == 200:
    # Afficher le contenu de la réponse
    print(response.text)
else:
    # Afficher le code de statut en cas d'erreur
    print("La requête a échoué avec le code de statut :", response.status_code)