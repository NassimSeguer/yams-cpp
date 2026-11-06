// ============================================================================
//  Module affichage : dessin des dés, bilans et feuille de score.
//  La documentation des fonctions publiques se trouve dans include/affichage.h.
// ============================================================================

#include <stdio.h>
#include "types.h"
#include "unicode.h"
#include "score.h"
#include "affichage.h"

#define TAILLE_GRILLE      3                         // une face de dé = grille de 3 x 3 emplacements
#define LARGEUR_DE         (TAILLE_GRILLE * 3)       // chaque emplacement occupe 3 colonnes : " ● "
#define ESPACE_ENTRE_DES   "   "
#define LARGEUR_COL_MANCHE 6                         // largeur du texte "Manche"
#define LARGEUR_COL_JOUEUR TAILLE_NOM                // assez large pour le nom le plus long

/*
 * Position des points sur chaque face d'un dé, vue comme une grille 3 x 3.
 * MOTIFS[v - 1][ligne][colonne] vaut 1 s'il y a un point à cet endroit sur
 * la face de valeur v. Par exemple, le 5 a un point dans chaque coin et un
 * au centre.
 */
static const int MOTIFS[NB_FACES][TAILLE_GRILLE][TAILLE_GRILLE] = {
    {{0, 0, 0}, {0, 1, 0}, {0, 0, 0}},  // 1
    {{1, 0, 0}, {0, 0, 0}, {0, 0, 1}},  // 2
    {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}},  // 3
    {{1, 0, 1}, {0, 0, 0}, {1, 0, 1}},  // 4
    {{1, 0, 1}, {0, 1, 0}, {1, 0, 1}},  // 5
    {{1, 0, 1}, {1, 0, 1}, {1, 0, 1}},  // 6
};

/**
 * Rôle : affiche un certain nombre d'espaces.
 * But : compléter un texte trop court pour aligner les colonnes.
 * Paramètres :
 *   - nombre : nombre d'espaces à écrire (rien si nombre <= 0)
 */
static void afficherEspaces(int nombre) {
    for (int i = 0; i < nombre; i++) {
        printf(" ");
    }
}

/**
 * Rôle : affiche un texte puis le complète par des espaces jusqu'à la largeur
 *        demandée.
 * But : aligner les colonnes du tableau. printf("%-20s") ne convient pas car
 *       il compte les octets : un nom accentué serait mal aligné.
 * Paramètres :
 *   - texte   : texte UTF-8 à afficher
 *   - largeur : nombre de colonnes que doit occuper le texte
 */
static void afficherTexteAligne(const char *texte, int largeur) {
    printf("%s", texte);
    afficherEspaces(largeur - largeurAffichee(texte));
}

/**
 * Rôle : affiche une ligne horizontale de dé (┏━━━━━━━━━┓ ou ┗━━━━━━━━━┛)
 *        pour chaque dé, côte à côte.
 * But : dessiner le haut et le bas des dés sans répéter le même code.
 * Paramètres :
 *   - coinGauche : caractère du coin gauche (┏ ou ┗)
 *   - coinDroit  : caractère du coin droit (┓ ou ┛)
 *   - nbDes      : nombre de dés
 */
static void afficherBordDes(char32_t coinGauche, char32_t coinDroit, int nbDes) {
    for (int i = 0; i < nbDes; i++) {
        afficherCaractere(coinGauche);
        afficherCaractereRepete(CAR_HORIZONTAL, LARGEUR_DE);
        afficherCaractere(coinDroit);
        printf("%s", ESPACE_ENTRE_DES);
    }
    printf("\n");
}

/**
 * Rôle : affiche le détail du calcul des points d'un lancer.
 * But : rendre la règle des bonus visible au joueur.
 * Paramètres :
 *   - de1 : valeur du premier dé
 *   - de2 : valeur du second dé
 */
static void afficherDetailPoints(int de1, int de2) {
    int points = calculerPoints(de1, de2);

    if (de1 == de2 && de1 == 1) {
        printf("→ Double 1, le coup parfait : 1 + 1 + %d = %d points\n", BONUS_DOUBLE_UN, points);
    } else if (de1 == de2) {
        printf("→ Double %d : %d + %d + %d = %d points\n", de1, de1, de2, BONUS_DOUBLE, points);
    } else {
        printf("→ %d + %d = %d points\n", de1, de2, points);
    }
}

/**
 * Rôle : affiche une ligne de séparation du tableau des scores, par exemple
 *        ┏━━━━━━━━┳━━━━...━━┳━━━━...━━┓.
 * But : tracer le haut, le milieu et le bas du tableau avec une seule fonction.
 * Paramètres :
 *   - gauche : caractère de l'extrémité gauche
 *   - milieu : caractère placé entre deux colonnes
 *   - droite : caractère de l'extrémité droite
 */
static void afficherSeparationTableau(char32_t gauche, char32_t milieu, char32_t droite) {
    // + 2 pour l'espace de marge de chaque côté du texte
    afficherCaractere(gauche);
    afficherCaractereRepete(CAR_HORIZONTAL, LARGEUR_COL_MANCHE + 2);
    afficherCaractere(milieu);
    afficherCaractereRepete(CAR_HORIZONTAL, LARGEUR_COL_JOUEUR + 2);
    afficherCaractere(milieu);
    afficherCaractereRepete(CAR_HORIZONTAL, LARGEUR_COL_JOUEUR + 2);
    afficherCaractere(droite);
    printf("\n");
}

/**
 * Rôle : affiche une ligne de texte du tableau des scores (3 colonnes).
 * But : garantir que toutes les lignes ont exactement la même largeur.
 * Paramètres :
 *   - colonne1 : texte de la colonne "Manche"
 *   - colonne2 : texte de la colonne du joueur humain
 *   - colonne3 : texte de la colonne de Hal
 */
static void afficherLigneTableau(const char *colonne1, const char *colonne2, const char *colonne3) {
    afficherCaractere(CAR_VERTICAL);
    printf(" ");
    afficherTexteAligne(colonne1, LARGEUR_COL_MANCHE);
    printf(" ");
    afficherCaractere(CAR_VERTICAL);
    printf(" ");
    afficherTexteAligne(colonne2, LARGEUR_COL_JOUEUR);
    printf(" ");
    afficherCaractere(CAR_VERTICAL);
    printf(" ");
    afficherTexteAligne(colonne3, LARGEUR_COL_JOUEUR);
    printf(" ");
    afficherCaractere(CAR_VERTICAL);
    printf("\n");
}

/**
 * Rôle : écrit dans un texte le résumé d'une manche, par exemple "4 et 4 : 28 pts".
 * But : remplir les cases du tableau des scores.
 * Paramètres :
 *   - manche : la manche à résumer
 *   - texte  : tableau qui reçoit le résumé
 *   - taille : nombre de cases du tableau texte
 */
static void formaterManche(const TManche *manche, char texte[], int taille) {
    snprintf(texte, taille, "%d et %d : %d pts", manche->des[0], manche->des[1], manche->points);
}

void afficherCadre(const char *texte) {
    int largeur = largeurAffichee(texte) + 4;  // 2 espaces de marge de chaque côté

    afficherCaractere(CAR_COIN_HAUT_GAUCHE);
    afficherCaractereRepete(CAR_HORIZONTAL, largeur);
    afficherCaractere(CAR_COIN_HAUT_DROIT);
    printf("\n");

    afficherCaractere(CAR_VERTICAL);
    printf("  %s  ", texte);
    afficherCaractere(CAR_VERTICAL);
    printf("\n");

    afficherCaractere(CAR_COIN_BAS_GAUCHE);
    afficherCaractereRepete(CAR_HORIZONTAL, largeur);
    afficherCaractere(CAR_COIN_BAS_DROIT);
    printf("\n");
}

void afficherTitre() {
    char titre[64];

    snprintf(titre, sizeof(titre), "Y A M ' S   ·   %d manches pour battre Hal", NB_MANCHES);
    printf("\n");
    afficherCadre(titre);
}

void afficherRegles() {
    printf("\nRègles du jeu :\n");
    printf("  - À chaque manche, Hal puis vous lancez %d dés.\n", NB_DES);
    printf("  - Vous pouvez ensuite relancer un dé ou les deux (%d relance par manche).\n", NB_TIRAGES - 1);
    printf("  - Points = somme des dés. Un double rapporte %d points de bonus,\n", BONUS_DOUBLE);
    printf("    et un double 1 en rapporte %d : c'est le meilleur coup.\n", BONUS_DOUBLE_UN);
    printf("  - Après %d manches, le plus gros total gagne.\n", NB_MANCHES);
    printf("  - Méfiez-vous : les dés de Hal sont légèrement truqués...\n\n");
}

void afficherDes(const int des[], int nbDes) {
    afficherBordDes(CAR_COIN_HAUT_GAUCHE, CAR_COIN_HAUT_DROIT, nbDes);

    // La console écrit ligne par ligne : pour placer les dés côte à côte,
    // on dessine sur chaque ligne le morceau correspondant de chaque dé.
    for (int ligne = 0; ligne < TAILLE_GRILLE; ligne++) {
        for (int i = 0; i < nbDes; i++) {
            afficherCaractere(CAR_VERTICAL);
            for (int colonne = 0; colonne < TAILLE_GRILLE; colonne++) {
                if (MOTIFS[des[i] - 1][ligne][colonne]) {
                    printf(" ");
                    afficherCaractere(CAR_POINT);
                    printf(" ");
                } else {
                    printf("   ");
                }
            }
            afficherCaractere(CAR_VERTICAL);
            printf("%s", ESPACE_ENTRE_DES);
        }
        printf("\n");
    }

    afficherBordDes(CAR_COIN_BAS_GAUCHE, CAR_COIN_BAS_DROIT, nbDes);

    // Étiquette centrée sous chaque dé : 4 espaces + "Dé n" + 3 espaces = largeur d'un dé
    for (int i = 0; i < nbDes; i++) {
        printf("    Dé %d   %s", i + 1, ESPACE_ENTRE_DES);
    }
    printf("\n");
}

void afficherTirage(const char *nom, const TManche *manche) {
    printf("\nDés de %s :\n", nom);
    afficherDes(manche->des, NB_DES);
    afficherDetailPoints(manche->des[0], manche->des[1]);
}

void afficherEnTeteManche(int indiceManche) {
    printf("\n\n");
    afficherCaractereRepete(CAR_HORIZONTAL, 16);
    printf("  Manche %d / %d  ", indiceManche + 1, NB_MANCHES);
    afficherCaractereRepete(CAR_HORIZONTAL, 16);
    printf("\n");
}

void afficherBilanManche(const TJoueur *humain, const TJoueur *hal, int indiceManche) {
    int pointsHumain = humain->manches[indiceManche].points;
    int pointsHal = hal->manches[indiceManche].points;

    printf("\n");
    switch (determinerGagnant(pointsHumain, pointsHal)) {
        case GAGNANT_HUMAIN:
            printf("Manche remportée par %s (%d contre %d) !\n", humain->nom, pointsHumain, pointsHal);
            break;
        case GAGNANT_HAL:
            printf("Manche remportée par %s (%d contre %d).\n", hal->nom, pointsHal, pointsHumain);
            break;
        default:
            printf("Égalité sur cette manche (%d partout).\n", pointsHumain);
    }

    printf("Score total : %s %d – %s %d\n",
           humain->nom, calculerTotal(humain, indiceManche + 1),
           hal->nom, calculerTotal(hal, indiceManche + 1));
}

void afficherFeuilleDeScore(const TJoueur *humain, const TJoueur *hal, int nbManchesJouees) {
    char numero[16];
    char caseHumain[48];
    char caseHal[48];

    printf("\n\nFeuille de score :\n");
    afficherSeparationTableau(CAR_COIN_HAUT_GAUCHE, CAR_T_HAUT, CAR_COIN_HAUT_DROIT);
    afficherLigneTableau("Manche", humain->nom, hal->nom);
    afficherSeparationTableau(CAR_T_GAUCHE, CAR_CROIX, CAR_T_DROIT);

    for (int i = 0; i < nbManchesJouees; i++) {
        snprintf(numero, sizeof(numero), "%d", i + 1);
        formaterManche(&humain->manches[i], caseHumain, (int) sizeof(caseHumain));
        formaterManche(&hal->manches[i], caseHal, (int) sizeof(caseHal));
        afficherLigneTableau(numero, caseHumain, caseHal);
    }

    afficherSeparationTableau(CAR_T_GAUCHE, CAR_CROIX, CAR_T_DROIT);
    snprintf(caseHumain, sizeof(caseHumain), "%d pts", calculerTotal(humain, nbManchesJouees));
    snprintf(caseHal, sizeof(caseHal), "%d pts", calculerTotal(hal, nbManchesJouees));
    afficherLigneTableau("Total", caseHumain, caseHal);
    afficherSeparationTableau(CAR_COIN_BAS_GAUCHE, CAR_T_BAS, CAR_COIN_BAS_DROIT);
}

void afficherResultatFinal(const TJoueur *humain, const TJoueur *hal) {
    char message[128];
    int totalHumain = calculerTotal(humain, NB_MANCHES);
    int totalHal = calculerTotal(hal, NB_MANCHES);

    switch (determinerGagnant(totalHumain, totalHal)) {
        case GAGNANT_HUMAIN:
            snprintf(message, sizeof(message), "Victoire de %s : %d points contre %d !",
                     humain->nom, totalHumain, totalHal);
            break;
        case GAGNANT_HAL:
            snprintf(message, sizeof(message), "%s gagne : %d points contre %d. Revanche ?",
                     hal->nom, totalHal, totalHumain);
            break;
        default:
            snprintf(message, sizeof(message), "Égalité parfaite : %d points partout !", totalHumain);
    }

    printf("\n");
    afficherCadre(message);
}
