# KeyHack
====================
Script Arduino pour Board Micro (et toutes celles possédant un ATmega32u4).
Simuler un clavier + routine sur carte SD.

J'ai commencé ce script après avoir découvert l'existance de l'USB Rubber Ducky.
Retrouvez la vidéo de démonstration ici : https://youtu.be/sbKN8FhGnqg

![Schéma du montage](/small.png?raw=true "Schéma du montage")

Ce schéma de montage comprend plusieurs ports USB pour une raison précise.
Pour masquer le hack, j'ai pensé mettre un disque dur externe 2"5 ainsi que l'Arduino dans un boitier HDD 3"5.
Le script exécute le code et actionne les relais qui privent l'Arduino de D+ et D-, pour les faire passer au disque dur.

Du coup, une fois relié à l'ordinateur, le clavier est détecté, le script s'exécute, et enfin le disque dur prend le relai.
Vous pouvez simplement oublier les élements suivant ci cela ne vous intéresse pas :
 - 2 relais,
 - USB HDD,
 - USB Arduino,
 - USB Externe.

Une diode est ajoutée dans le but de nous éclaircir sur la progression.
Pour finir, trois boutons sont utilisés pour choisir le système sur lequel sera branché notre appareil.
Ceci est très important, car chaque système d'exploitation utilise ses propres codes ( Windows: ALT+F4, OSX: CMD+Q ).

## Plus

Il est possible d'insérer plusieurs scripts sur la carte SD et d'en sélectionner un via un fichier de configuration.
En créant le fichier "config.ini" et en écrivant ce qui suit, le script prendra le fichier demandé.

>[KeyHack]
>default=fichier.kh

Attention, le nom du fichier est limité à 7 caractères (sans compter l'extension).

## Notes

Plusieurs problèmes viennent avec ce code.
Il est configuré comme clavier QWERTY et je n'ai pas encore tout testé.

N'hésitez donc pas à partager vos modifications et/ou critiques (constructives).
