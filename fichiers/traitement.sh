#!/bin/bash

# Étape 1 : Nettoyer le tableau et calculer le total des médailles
# Démarrage du container en arrière-plan
docker run --rm -d --name exo php:8.3-cli-alpine tail -f /dev/null

# Création du dossier /data dans lequel on va mettre les fichiers de l'exo
docker exec exo mkdir /data
# Transfert des fichiers de l'exo vers le container
docker container cp medailles.csv exo:/data/medailles.csv
docker container cp tri.php exo:/data/tri.php

# Triage des médailles
docker exec exo php /data/tri.php

# Arrêt du container
docker kill exo

# Étape 2 : Télécharger les drapeaux nécessaires
docker run --rm -d --name exo2 sae103-wget tail -f /dev/null
docker container cp medailles.csv exo2:/data/medailles.csv
docker container cp flags.php exo2:/data/flags.sh
docker exec exo2 bash /data/flags.sh
docker container cp exo2:/data/flags/ flags/
docker kill exo

# Étape 3 : Générer le fichier PDF final
# ...

echo "Traitement terminé. Résultats dans le dossier 'output'."
