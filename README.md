# 🎲 Yam's contre Hal

> Un jeu de dés en console, écrit en C++ : affrontez **Hal**, un ordinateur qui triche, en 5 manches.

![C++20](https://img.shields.io/badge/C++-20-00599C?style=flat&logo=cplusplus&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-3.20+-064F8C?style=flat&logo=cmake&logoColor=white)
![Tests](https://img.shields.io/badge/tests-34%20v%C3%A9rifications-2ea44f?style=flat)

Les dés sont dessinés en caractères Unicode, Hal lance des dés truqués qui suivent une loi normale, et toute la saisie est protégée contre les erreurs de frappe.

## 📸 Aperçu

```
━━━━━━━━━━━━━━━━  Manche 3 / 5  ━━━━━━━━━━━━━━━━

Dés de Hal :
┏━━━━━━━━━┓   ┏━━━━━━━━━┓
┃ ●     ● ┃   ┃ ●     ● ┃
┃         ┃   ┃ ●     ● ┃
┃ ●     ● ┃   ┃ ●     ● ┃
┗━━━━━━━━━┛   ┗━━━━━━━━━┛
    Dé 1          Dé 2
→ 4 + 6 = 10 points

Dés de Nassim :
┏━━━━━━━━━┓   ┏━━━━━━━━━┓
┃         ┃   ┃ ●       ┃
┃    ●    ┃   ┃    ●    ┃
┃         ┃   ┃       ● ┃
┗━━━━━━━━━┛   ┗━━━━━━━━━┛
    Dé 1          Dé 2
→ 1 + 3 = 4 points

Que voulez-vous faire ? (relances restantes : 1)
  1 - Garder ces dés
  2 - Relancer le dé 1
  3 - Relancer le dé 2
  4 - Relancer les deux dés
-> 2

Dés de Nassim :
┏━━━━━━━━━┓   ┏━━━━━━━━━┓
┃ ●       ┃   ┃ ●       ┃
┃    ●    ┃   ┃    ●    ┃
┃       ● ┃   ┃       ● ┃
┗━━━━━━━━━┛   ┗━━━━━━━━━┛
    Dé 1          Dé 2
→ Double 3 : 3 + 3 + 20 = 26 points

Manche remportée par Nassim (26 contre 10) !
Score total : Nassim 41 – Hal 45
```

En fin de partie, une feuille de score récapitule chaque manche :

```
┏━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━┓
┃ Manche ┃ Nassim               ┃ Hal                  ┃
┣━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━━┫
┃ 1      ┃ 4 et 3 : 7 pts       ┃ 5 et 2 : 7 pts       ┃
┃ 2      ┃ 6 et 2 : 8 pts       ┃ 4 et 4 : 28 pts      ┃
┃ 3      ┃ 3 et 3 : 26 pts      ┃ 4 et 6 : 10 pts      ┃
┃ 4      ┃ 2 et 5 : 7 pts       ┃ 5 et 6 : 11 pts      ┃
┃ 5      ┃ 5 et 5 : 30 pts      ┃ 4 et 4 : 28 pts      ┃
┣━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━━┫
┃ Total  ┃ 78 pts               ┃ 84 pts               ┃
┗━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━┛
```

## 📜 Règles du jeu

- À chaque manche, Hal puis le joueur lancent **2 dés**.
- Le joueur peut garder ses dés ou **relancer** le premier, le second ou les deux (1 relance par manche).
- Points = somme des dés. Un **double** rapporte **+20**, un **double 1** rapporte **+40** (42 points, le meilleur coup).
- Après **5 manches**, le plus gros total l'emporte.

## ✨ Fonctionnalités

- **Dés dessinés en Unicode** grâce à un encodeur UTF-8 écrit à la main, sans bibliothèque externe.
- **Un adversaire qui triche** : les dés de Hal suivent une loi normale de moyenne 4, générée avec la méthode de Box-Muller.
- **Feuille de score alignée**, même quand le nom du joueur contient des accents.
- **Saisie robuste** : « abc », « 7 » ou « 2x » sont refusés et la question est reposée ; le programme s'arrête proprement si l'entrée se ferme.
- **Plusieurs parties** à la suite, avec un compteur de victoires.
- **34 vérifications automatiques** lancées avec `ctest`.

## 🚀 Compiler et lancer

Il faut un compilateur C++20 (GCC/MinGW, Clang ou MSVC) et CMake 3.20 ou plus récent.

```bash
git clone https://github.com/NassimSeguer/yams-cpp.git
cd yams-cpp
cmake -S . -B build
cmake --build build
./build/yams
```

Sous Windows, lancez `build\yams.exe`. Avec CLion, il suffit d'ouvrir le dossier puis d'exécuter la cible `yams`.

### Lancer les tests

```bash
ctest --test-dir build --output-on-failure
```

## 🗂️ Organisation du code

```
yams-cpp/
├── CMakeLists.txt
├── include/          les en-têtes : chaque fonction y est documentée
│   ├── types.h       constantes du jeu et structures TManche, TJoueur
│   ├── unicode.h
│   ├── des.h
│   ├── score.h
│   ├── saisie.h
│   ├── affichage.h
│   └── partie.h
├── src/              le code des modules
└── tests/
    └── tests.cpp     tests automatiques
```

| Module | Rôle |
|---|---|
| `unicode` | Convertir un code Unicode en UTF-8 et afficher les caractères de dessin |
| `des` | Tirages au hasard : dé équilibré pour le joueur, dé truqué pour Hal |
| `score` | Règles de calcul des points et désignation du gagnant |
| `saisie` | Lecture du clavier sans plantage |
| `affichage` | Tout ce qui s'écrit à l'écran : dés, bilans, feuille de score |
| `partie` | Enchaînement des tours, des manches et de la partie |

Chaque module a une seule responsabilité : par exemple, `affichage` ne décide d'aucune règle et `score` n'écrit rien à l'écran. Chaque fonction est documentée dans son en-tête (rôle, but, paramètres, valeur de retour).

## 🧠 Points techniques

### Afficher un dé en Unicode

La console n'affiche pas un numéro Unicode tel quel : elle attend du texte encodé en **UTF-8**, où un caractère occupe de 1 à 4 octets. La fonction `unicodeVersUtf8` répartit les bits du code dans le bon nombre d'octets :

| Codes | Octets UTF-8 | Exemple |
|---|---|---|
| U+0000 à U+007F | `0xxxxxxx` | `A` → `41` |
| U+0080 à U+07FF | `110xxxxx 10xxxxxx` | `é` → `C3 A9` |
| U+0800 à U+FFFF | `1110xxxx 10xxxxxx 10xxxxxx` | `┏` → `E2 94 8F` |
| U+10000 à U+10FFFF | `11110xxx 10xxxxxx 10xxxxxx 10xxxxxx` | 🎲 → `F0 9F 8E B2` |

Pour aligner le tableau, `largeurAffichee` compte les caractères et non les octets : elle ignore les octets de la forme `10xxxxxx`. « Hélène » compte ainsi 6 caractères, alors qu'il fait 8 octets.

### Le dé truqué de Hal

Un dé honnête donne chaque face avec une probabilité de 1/6. Le dé de Hal utilise la **méthode de Box-Muller** : à partir de deux nombres uniformes *u₁* et *u₂* dans [0, 1[,

```
z = √(−2 ln(1 − u₁)) × cos(2π u₂)
```

suit une loi normale de moyenne 0 et d'écart-type 1. Hal tire donc `4 + z`, arrondi puis ramené entre 1 et 6. On utilise `1 − u₁`, qui n'est jamais nul, car `ln(0)` n'existe pas.

| Face | 1 | 2 | 3 | 4 | 5 | 6 |
|---|---|---|---|---|---|---|
| Dé honnête | 16,7 % | 16,7 % | 16,7 % | 16,7 % | 16,7 % | 16,7 % |
| Dé de Hal | 0,6 % | 6,1 % | 24,2 % | 38,3 % | 24,2 % | 6,7 % |

Hal fait donc un double dans **27 %** des cas, contre 17 % pour un dé honnête. Il marque en moyenne 13,4 points par manche, contre 10,9 pour un joueur qui ne relance jamais : pour le battre, il faut bien utiliser ses relances. En revanche, Hal n'obtient presque jamais le double 1.

### Une saisie qui ne plante jamais

Toutes les saisies lisent une ligne entière avec `fgets`, puis l'analysent avec `sscanf(tampon, "%d %c", ...)`. Le choix n'est accepté que si `sscanf` lit exactement un entier, sans texte derrière, et que cet entier est dans l'intervalle attendu. On évite ainsi les pièges classiques de `scanf`, comme les caractères laissés en attente ou une boucle infinie sur une lettre.

## 🔭 Pistes d'amélioration

- Donner à Hal une vraie stratégie de relance.
- Passer au Yam's complet : 5 dés, brelan, full, carré, suites.
- Sauvegarder les meilleurs scores dans un fichier.

## 👤 Auteur

**Nassim Seguer**, étudiant en L3 Informatique à l'UFR MIM de Metz · [LinkedIn](https://linkedin.com/in/nassim-seguer)

*Projet inspiré d'un TP de Licence Informatique, entièrement réécrit et enrichi : architecture en modules, encodeur UTF-8, saisie sécurisée, feuille de score et tests automatiques.*
