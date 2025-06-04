<?php

$inputFile = "/data/medailles.csv";

// Ouverture du fichier en lecture seule (r)
$file = fopen($inputFile, 'r');

// Titre inutile
fgetcsv($file);

// Entêtes inutiles
fgetcsv($file);

$countries = [];

// Calcul du total des médailles tant que $row n'est pas vide (tant qu'il y a des lignes)
while ($row = fgetcsv($file)) {
    // 1: or, 2: argent, 3: bronze
    $row[4] = $row[1] + $row[2] + $row[3];
    $countries[] = $row;
}

// Si résultat < 0: a > b
// Si résultat > 0: b > a
// Si résultat == 0: b == a
function fonctionTri($a, $b) {
    // Triage de l'or si les valeurs sont différentes
    if ($a[1] != $b[1]) {
        return $b[1] - $a[1];
    }

    // Sinon, si les médailles d'or sont égales, on tri l'argent
    if ($a[2] != $b[2]) {
        return $b[2] - $a[2];
    }

    // Enfin, si les médailles d'argent sont égales, on tri le bronze
    if ($a[3] != $b[3]) {
        return $b[3] - $a[3];
    }

    // Comparaison de deux strings pour l'ordre alphabétique
    return strcmp($a[0], $b[0]);
}

// Utilisation de notre propre fonction de tri
usort($countries, 'fonctionTri');

// Création du classement
$rank = 1;
$countriesWithRank = [];

foreach ($countries as $i => $country) {
    // Rang de l'itération
    $currentRank = $rank;
    // On augmente le rang pour l'itération suivante
    $rank++;

    // Premier pays
    if ($i == 0) {
        $countriesWithRank[] = [
            $currentRank,
            $country[0],
            $country[1],
            $country[2],
            $country[3],
            $country[4]
        ];
    } else {
        // Les autres pays
        $previousCountry = $countries[$i - 1];

        // Si les deux pays ont les mêmes médailles, ils sont ex-æquo
        if ($country[1] == $previousCountry[1] && $country[2] == $previousCountry[2] && $country[3] == $previousCountry[3]) {
            $currentRank = '-';
        }

        $countriesWithRank[] = [
            $currentRank,
            $country[0],
            $country[1],
            $country[2],
            $country[3],
            $country[4]
        ];
    }
}

var_dump($countriesWithRank);

fclose($file);
