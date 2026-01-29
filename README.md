# 🌻 Plants vs. Zombies (Version C)

![Language](https://img.shields.io/badge/Language-C-blue)
![Compiler](https://img.shields.io/badge/Compiler-GCC-green)

Une implémentation du célèbre jeu de stratégie développée en langage C. Ce projet permet de gérer des vagues de zombies, de charger des niveaux personnalisés via des fichiers texte et de sauvegarder les meilleurs scores.

## 🚀 Fonctionnalités

* **Moteur de Jeu :** Boucle principale gérant l'apparition des zombies et les interactions.
* **Système de Niveaux :** Chargement dynamique de scénarios via fichiers texte (`partie.txt`, `partie2.txt`...).
* **Sauvegarde des Scores :** Gestion persistante des records via le fichier `meilleurs_scores.txt`.
* **Personnalisation :** Possibilité pour l'utilisateur de créer et jouer ses propres niveaux.

## 🛠️ Installation

1.  **Préparation des fichiers :**
    Assurez-vous que tous les fichiers suivants sont présents dans le **même répertoire** :
    * `projet.c` (Code source)
    * `partie.txt` jusqu'à `partie6.txt` (Les niveaux)
    * `meilleurs_scores.txt` (Le fichier de sauvegarde, même vide)

2.  **Compilation :**
    Ouvrez un terminal dans le dossier du projet et lancez la commande :
    ```bash
    gcc -Wall projet.c
    ```

## 💻 Usage

* **Lancer le Jeu :**
    Une fois la compilation terminée, exécutez le programme :
    ```bash
    ./a.out
    ```

    > **⚠️ IMPORTANT :** Au tout début de l'exécution, vous devez appuyer sur la touche **"Entrée"** pour passer l'écran d'accueil et lancer le jeu.

* **Jouer un Niveau Personnalisé :**
    1.  Créez votre fichier de niveau et nommez-le **`partie.txt`** (ceci remplacera le niveau par défaut).
    2.  Lancez le jeu.
    3.  Au menu principal, tapez **`1`** pour charger ce niveau spécifique.

## ⚠️ Avertissement
Ce projet est développé dans un cadre pédagogique pour l'apprentissage du langage C.

## 👤 Auteurs
**Joshua LOZANO** - **Clovis ROLLET**
