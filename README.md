# 🌻 Plants vs. Zombies (Version C)

![Language](https://img.shields.io/badge/Language-C-blue)
![Compiler](https://img.shields.io/badge/Compiler-GCC-green)

[cite_start]Une implémentation du célèbre jeu de stratégie développée en langage C[cite: 1]. Ce projet permet de gérer des vagues de zombies, de charger des niveaux personnalisés via des fichiers texte et de sauvegarder les meilleurs scores.

## 🚀 Fonctionnalités

* **Moteur de Jeu :** Boucle principale gérant l'apparition des zombies et les interactions.
* [cite_start]**Système de Niveaux :** Chargement dynamique de scénarios via fichiers texte (`partie.txt`, `partie2.txt`...)[cite: 2].
* [cite_start]**Sauvegarde des Scores :** Gestion persistante des records via le fichier `meilleurs_scores.txt`[cite: 3].
* [cite_start]**Personnalisation :** Possibilité pour l'utilisateur de créer et jouer ses propres niveaux[cite: 10].

## 🛠️ Installation & Compilation

1.  **Préparer l'environnement :**
    [cite_start]Assurez-vous que tous les fichiers suivants sont dans le **même dossier**[cite: 2]:
    * `projet.c` (Code source)
    * `partie.txt` à `partie6.txt` (Niveaux)
    * [cite_start]`meilleurs_scores.txt` (Même vide) [cite: 3]

2.  **Ouvrir le terminal :**
    Placez-vous dans le répertoire du projet :
    ```bash
    cd chemin/vers/le/dossier
    ```
    [cite_start][cite: 4, 5]

3.  **Compiler le jeu :**
    Utilisez `gcc` pour créer l'exécutable :
    ```bash
    gcc -Wall projet.c
    ```
    [cite_start][cite: 6, 7]

## 💻 Usage

* **Lancer le Jeu :**
    Une fois compilé, exécutez le programme :
    ```bash
    ./a.out
    ```
    [cite_start][cite: 8, 9]

    > [cite_start]**⚠️ IMPORTANT :** Au tout début de l'exécution, vous devez appuyer sur la touche **"Entrée"** pour passer l'écran d'accueil et lancer le jeu[cite: 12].

* **Jouer un Niveau Personnalisé :**
    1.  [cite_start]Créez votre fichier de niveau et nommez-le **`partie.txt`** (remplace celui par défaut)[cite: 10].
    2.  Lancez le jeu.
    3.  [cite_start]Au menu, tapez **`1`** pour charger ce niveau spécifique[cite: 11].

## ⚠️ Avertissement
Ce projet est développé dans un cadre pédagogique pour l'apprentissage du langage C.

## 👤 Auteur
**Joshua LOZANO** - [Mon LinkedIn](https://www.linkedin.com/in/joshua-lozano-ing/)
