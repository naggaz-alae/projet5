# Takuzu en C

Ce projet est une version complète du jeu de logique **Takuzu** (qu'on appelle aussi Binairo), écrite en C pendant notre licence d'informatique à l'Université de Bordeaux, en 2023. On l'a construit à trois, étape par étape : d'abord le moteur du jeu, puis une version dans le terminal, un solveur automatique, une interface graphique en SDL2 et enfin une version jouable directement dans le navigateur grâce à WebAssembly.

## Le principe du jeu

On part d'une grille où quelques cases sont déjà remplies. Le but est de compléter toutes les autres avec des cases blanches (0) ou noires (1), en respectant trois règles simples :

1. Jamais plus de deux cases de la même couleur à la suite, que ce soit sur une ligne ou dans une colonne.
2. Chaque ligne et chaque colonne contient autant de blanches que de noires.
3. Selon l'option choisie, deux lignes (ou deux colonnes) ne peuvent pas être identiques.

Les cases données au départ sont fixes : impossible de les modifier. Tout le reste, c'est à vous de le déduire.

## Ce que contient le projet

Le cœur du projet est une bibliothèque qui gère tout ce qui concerne une partie. Autour d'elle, on a construit plusieurs façons de jouer.

| Programme | Ce qu'il fait |
|---|---|
| `game_text` | Le jeu dans le terminal, on tape les coups au clavier |
| `game_sdl` | La version graphique, avec une vraie fenêtre et la souris |
| `game_solve` | Un solveur qui trouve la solution d'une grille ou compte combien il en existe |
| `web/` | La version navigateur, compilée en WebAssembly |
| `game_test_*` | Les tests unitaires de chaque fonction |

La bibliothèque sait aussi gérer des grilles de n'importe quelle taille, un mode où la grille « boucle » sur ses bords (la dernière colonne touche la première), et l'annulation ou le rétablissement des coups joués.

## Compiler le projet

Le projet a été pensé pour Linux. Sous Windows, le plus simple est de passer par WSL avec Ubuntu.

Commencez par installer les outils et la bibliothèque SDL2 :

```bash
sudo apt update
sudo apt install build-essential cmake libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev
```

Ensuite, depuis le dossier du projet :

```bash
mkdir build && cd build
cmake ..
make
```

Pour vérifier que tout fonctionne, lancez les tests :

```bash
ctest
```

## Jouer

### Dans le terminal

```bash
./game_text
```

La grille s'affiche, et vous jouez en tapant une lettre suivie des coordonnées de la case (ligne puis colonne) :

| Commande | Action |
|---|---|
| `w i j` | Place une case blanche en (i, j) |
| `b i j` | Place une case noire en (i, j) |
| `e i j` | Vide la case (i, j) |
| `z` | Annule le dernier coup |
| `y` | Rétablit le coup annulé |
| `r` | Recommence la partie |
| `h` | Affiche l'aide |
| `q` | Quitte le jeu |

### Avec l'interface graphique

```bash
./game_sdl            # grille par défaut
./game_sdl grille.txt # ou une grille de votre choix
```

On pointe une case avec la souris, puis on appuie sur une touche :

| Touche | Action |
|---|---|
| `W` | Case blanche |
| `B` | Case noire |
| `E` | Vider la case |
| `Z` / `Y` | Annuler / rétablir |
| `R` | Recommencer |
| `S` | Laisser le solveur finir la grille |
| `H` | Afficher ou masquer l'aide |
| `Q` ou `Échap` | Quitter |

### Dans le navigateur

Le dossier `web/` contient une version déjà compilée. Comme le navigateur refuse de charger du WebAssembly ouvert directement depuis le disque, il faut un petit serveur local :

```bash
cd web
python3 -m http.server
```

Ouvrez ensuite `http://localhost:8000/game.html`. Des boutons permettent de recommencer, d'annuler, de résoudre la grille ou d'en générer une nouvelle au hasard.

Pour recompiler cette version vous même, il faut [Emscripten](https://emscripten.org/), puis un simple `make` dans le dossier `web/`.

## Le solveur

```bash
./game_solve -s grille.txt              # affiche la solution
./game_solve -s grille.txt solution.txt # l'enregistre dans un fichier
./game_solve -c grille.txt              # compte le nombre de solutions
```

## Le format des grilles

Une grille est un simple fichier texte. La première ligne donne le nombre de lignes, le nombre de colonnes, puis deux options valant 0 ou 1 : le mode où la grille boucle sur ses bords, et l'obligation d'avoir des lignes toutes différentes. Viennent ensuite les cases, une ligne de texte par ligne de la grille.

```
4 4 0 0
BWee
WBee
BBee
WWee
```

| Lettre | Signification |
|---|---|
| `W` / `B` | Case blanche ou noire fixée au départ |
| `w` / `b` | Case blanche ou noire jouée |
| `e` | Case vide |

## Organisation du code

| Fichier | Rôle |
|---|---|
| `game.c` | Les fonctions de base : créer une grille, jouer un coup, détecter les erreurs, savoir si la partie est gagnée |
| `game_aux.c` | L'affichage et la grille par défaut |
| `game_ext.c` | Les grilles de taille libre, le mode qui boucle, l'annulation des coups |
| `queue.c` | La structure de données qui garde l'historique des coups |
| `game_tools.c` | Le chargement, la sauvegarde et le solveur |
| `model.c` | Toute la logique de l'interface SDL2 |

Les fichiers d'en tête (`.h`) et `game_private.c` nous ont été fournis par l'université : ils définissent l'interface que notre code devait respecter.

## L'équipe

Projet réalisé par moi, Yassine et Simo, dans le cadre du cours de projet technologique de l'Université de Bordeaux.
