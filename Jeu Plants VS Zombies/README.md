# 🌻 Plants vs. Zombies (Version C)

![Language](https://img.shields.io/badge/Language-C-blue)
![Compiler](https://img.shields.io/badge/Compiler-GCC-green)

Implémentation du jeu *Plants vs. Zombies* codée en langage C.

## 📂 1. Prérequis et Fichiers
[cite_start]Avant de commencer, vous devez impérativement enregistrer les fichiers suivants **sous le même répertoire** sur votre ordinateur[cite: 1, 2]:

* [cite_start]**Code source :** `projet.c` [cite: 2]
* [cite_start]**Fichiers de niveaux :** `partie.txt`, `partie2.txt`, `partie3.txt`, `partie4.txt`, `partie5.txt`, `partie6.txt` [cite: 2]
* [cite_start]**Fichier de sauvegarde :** `meilleurs_scores.txt` (⚠️ Important : ce fichier doit être présent dans le dossier, même s'il est vide)[cite: 3].

## 🛠️ 2. Compilation
1.  [cite_start]Ouvrez votre terminal[cite: 4].
2.  [cite_start]Placez-vous dans le répertoire contenant les fichiers grâce à la commande `cd`[cite: 4, 5].
3.  [cite_start]Compilez le projet en entrant la commande suivante[cite: 6]:

```bash
gcc -Wall projet.c
🚀 3. Exécution du jeu
Une fois la compilation terminée, lancez le programme avec la commande suivante:

Bash

./a.out
⚠️ Note importante sur le gameplay

Au tout début de l'exécution, vous devez appuyer sur la touche "Entrée" pour lancer le jeu.


Information : Cette instruction est indiquée à l'écran au démarrage, mais par souci de fluidité du gameplay, elle n'est pas rappelée par la suite.

🎮 4. Jouer avec vos propres niveaux
Vous avez la possibilité de créer et jouer vos propres configurations de jeu:

Créez votre fichier de niveau.

Nommez-le impérativement partie.txt (ceci remplacera le niveau par défaut fourni avec le sujet).

Lancez le jeu.

Lorsque le programme vous demandera à quelle partie vous voulez jouer, entrez le chiffre "1".

Projet réalisé en C.
