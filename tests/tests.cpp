// ============================================================================
//  Tests automatiques du Yam's.
//
//  Chaque fonction "tester..." vérifie un module du jeu. Le programme affiche
//  [OK] ou [ECHEC] pour chaque vérification, puis renvoie 0 si tout est
//  correct et 1 sinon (c'est ce code de retour que lit la commande ctest).
// ============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "unicode.h"
#include "des.h"
#include "score.h"
#include "partie.h"

#define NB_LANCERS_TEST 100000  // assez de lancers pour que les moyennes soient fiables

static int nbTests = 0;
static int nbEchecs = 0;

/**
 * Rôle : enregistre le résultat d'une vérification et l'affiche.
 * But : écrire chaque test en une ligne lisible.
 * Paramètres :
 *   - condition   : vraie (non nulle) si le comportement attendu est observé
 *   - description : ce qui est vérifié, en français
 */
static void verifier(int condition, const char *description) {
    nbTests++;
    if (condition) {
        printf("[OK]     %s\n", description);
    } else {
        nbEchecs++;
        printf("[ECHEC]  %s\n", description);
    }
}

/**
 * Rôle : vérifie la règle de calcul des points sur des cas connus.
 * But : s'assurer que les bonus de double sont correctement appliqués.
 */
static void testerCalculerPoints() {
    printf("\n-- calculerPoints --\n");
    verifier(calculerPoints(3, 5) == 8, "3 et 5 valent 8 points");
    verifier(calculerPoints(2, 1) == 3, "2 et 1 valent 3 points (pas de bonus sans double)");
    verifier(calculerPoints(4, 4) == 28, "un double 4 vaut 4 + 4 + 20 = 28 points");
    verifier(calculerPoints(6, 6) == 32, "un double 6 vaut 6 + 6 + 20 = 32 points");
    verifier(calculerPoints(1, 1) == 42, "un double 1 vaut 1 + 1 + 40 = 42 points");
}

/**
 * Rôle : vérifie l'addition des points sur plusieurs manches.
 * But : s'assurer que le score en cours et le total final sont justes.
 */
static void testerCalculerTotal() {
    TJoueur joueur;
    int points[NB_MANCHES] = {8, 28, 3, 42, 10};

    printf("\n-- calculerTotal --\n");
    initialiserJoueur(&joueur, "Test", 0);
    for (int i = 0; i < NB_MANCHES; i++) {
        joueur.manches[i].points = points[i];
    }
    verifier(calculerTotal(&joueur, 0) == 0, "total nul avant la première manche");
    verifier(calculerTotal(&joueur, 2) == 36, "total après 2 manches : 8 + 28 = 36");
    verifier(calculerTotal(&joueur, NB_MANCHES) == 91, "total des 5 manches : 91");
}

/**
 * Rôle : vérifie la désignation du gagnant.
 * But : couvrir les trois issues possibles, égalité comprise.
 */
static void testerDeterminerGagnant() {
    printf("\n-- determinerGagnant --\n");
    verifier(determinerGagnant(30, 20) == GAGNANT_HUMAIN, "30 contre 20 : l'humain gagne");
    verifier(determinerGagnant(20, 30) == GAGNANT_HAL, "20 contre 30 : Hal gagne");
    verifier(determinerGagnant(25, 25) == EGALITE, "25 contre 25 : égalité");
}

/**
 * Rôle : vérifie la conversion Unicode -> UTF-8 sur un caractère de chaque
 *        longueur (1, 2, 3 et 4 octets) et sur des codes invalides.
 * But : garantir que les dés s'affichent correctement dans la console.
 *       Les octets attendus sont ceux de la norme UTF-8.
 */
static void testerUnicodeVersUtf8() {
    unsigned char t[5];

    printf("\n-- unicodeVersUtf8 --\n");
    verifier(unicodeVersUtf8('A', t) == 1 && t[0] == 0x41 && t[1] == '\0',
             "'A' (U+0041) donne 1 octet : 41");
    verifier(unicodeVersUtf8(0xE9, t) == 2 && t[0] == 0xC3 && t[1] == 0xA9 && t[2] == '\0',
             "'é' (U+00E9) donne 2 octets : C3 A9");
    verifier(unicodeVersUtf8(CAR_COIN_HAUT_GAUCHE, t) == 3 && t[0] == 0xE2 && t[1] == 0x94 && t[2] == 0x8F,
             "'┏' (U+250F) donne 3 octets : E2 94 8F");
    verifier(unicodeVersUtf8(0x1F3B2, t) == 4 && t[0] == 0xF0 && t[1] == 0x9F && t[2] == 0x8E && t[3] == 0xB2,
             "l'emoji dé (U+1F3B2) donne 4 octets : F0 9F 8E B2");
    verifier(unicodeVersUtf8(0x110000, t) == 0 && t[0] == '\0', "un code au-delà de U+10FFFF est refusé");
    verifier(unicodeVersUtf8(0xD800, t) == 0 && t[0] == '\0', "un code réservé à l'UTF-16 (U+D800) est refusé");
}

/**
 * Rôle : vérifie le comptage des caractères d'un texte UTF-8.
 * But : garantir l'alignement du tableau des scores avec des noms accentués.
 */
static void testerLargeurAffichee() {
    printf("\n-- largeurAffichee --\n");
    verifier(largeurAffichee("") == 0, "un texte vide a une largeur de 0");
    verifier(largeurAffichee("Hal") == 3, "\"Hal\" a une largeur de 3");
    verifier(largeurAffichee("Hélène") == 6 && strlen("Hélène") == 8,
             "\"Hélène\" : 6 caractères à l'écran pour 8 octets");
    verifier(largeurAffichee("┏━┓") == 3, "\"┏━┓\" : 3 caractères pour 9 octets");
}

/**
 * Rôle : lance le dé équilibré un grand nombre de fois et analyse les résultats.
 * But : vérifier que chaque face sort environ une fois sur six, sans valeur
 *       impossible.
 */
static void testerDeEquilibre() {
    int compteur[NB_FACES + 1] = {0};
    int horsBornes = 0;
    long somme = 0;
    int facesEquilibrees = 1;

    printf("\n-- lancerDeEquilibre --\n");
    for (int i = 0; i < NB_LANCERS_TEST; i++) {
        int de = lancerDeEquilibre();
        if (de < 1 || de > NB_FACES) {
            horsBornes++;
        } else {
            compteur[de]++;
        }
        somme += de;
    }

    double moyenne = (double) somme / NB_LANCERS_TEST;
    for (int face = 1; face <= NB_FACES; face++) {
        double frequence = (double) compteur[face] / NB_LANCERS_TEST;
        if (frequence < 0.16 || frequence > 0.173) {
            facesEquilibrees = 0;
        }
    }

    verifier(horsBornes == 0, "le dé donne toujours une valeur entre 1 et 6");
    verifier(moyenne > 3.45 && moyenne < 3.55, "la moyenne des lancers est proche de 3,5");
    verifier(facesEquilibrees, "chaque face sort environ une fois sur six");
}

/**
 * Rôle : lance le dé truqué de Hal un grand nombre de fois et analyse les résultats.
 * But : vérifier qu'il reste entre 1 et 6 et qu'il favorise bien les grandes valeurs.
 */
static void testerDeTruque() {
    int compteur[NB_FACES + 1] = {0};
    int horsBornes = 0;
    long somme = 0;

    printf("\n-- lancerDeTruque --\n");
    for (int i = 0; i < NB_LANCERS_TEST; i++) {
        int de = lancerDeTruque();
        if (de < 1 || de > NB_FACES) {
            horsBornes++;
        } else {
            compteur[de]++;
        }
        somme += de;
    }

    double moyenne = (double) somme / NB_LANCERS_TEST;
    verifier(horsBornes == 0, "le dé de Hal donne toujours une valeur entre 1 et 6");
    verifier(moyenne > 3.9 && moyenne < 4.1, "la moyenne du dé de Hal est proche de 4 (il triche)");
    verifier(compteur[4] > 3 * compteur[2], "le 4 sort bien plus souvent que le 2");
}

/**
 * Rôle : vérifie les relances : seuls les dés demandés changent, et les
 *        points sont recalculés avec la règle complète (bonus compris).
 * But : éviter qu'un double obtenu en relançant soit compté sans son bonus.
 */
static void testerRelancerDes() {
    TManche manche;
    int de2Intact = 1;
    int de1Intact = 1;
    int pointsCorrects = 1;

    printf("\n-- relancerDes --\n");
    for (int i = 0; i < 1000; i++) {
        manche.des[0] = 2;
        manche.des[1] = 5;
        relancerDes(&manche, CHOIX_RELANCER_DE1, 0);
        if (manche.des[1] != 5) {
            de2Intact = 0;
        }
        if (manche.points != calculerPoints(manche.des[0], manche.des[1])) {
            pointsCorrects = 0;
        }

        manche.des[0] = 2;
        manche.des[1] = 5;
        relancerDes(&manche, CHOIX_RELANCER_DE2, 0);
        if (manche.des[0] != 2) {
            de1Intact = 0;
        }
        if (manche.points != calculerPoints(manche.des[0], manche.des[1])) {
            pointsCorrects = 0;
        }
    }

    manche.des[0] = 3;
    manche.des[1] = 3;
    relancerDes(&manche, CHOIX_GARDER, 0);

    verifier(de2Intact, "relancer le dé 1 ne modifie pas le dé 2");
    verifier(de1Intact, "relancer le dé 2 ne modifie pas le dé 1");
    verifier(pointsCorrects, "les points (bonus compris) sont recalculés après chaque relance");
    verifier(manche.des[0] == 3 && manche.des[1] == 3 && manche.points == 26,
             "garder ses dés ne change pas les valeurs (double 3 = 26 points)");
}

/**
 * Rôle : vérifie l'initialisation d'un joueur, avec un nom trop long.
 * But : s'assurer que le nom est coupé proprement et que les scores partent de zéro.
 */
static void testerInitialiserJoueur() {
    TJoueur joueur;
    int toutAZero = 1;

    printf("\n-- initialiserJoueur --\n");
    initialiserJoueur(&joueur, "UnNomBeaucoupTropLongPourLeJeu", 0);
    for (int i = 0; i < NB_MANCHES; i++) {
        if (joueur.manches[i].points != 0) {
            toutAZero = 0;
        }
    }
    verifier(strlen(joueur.nom) == TAILLE_NOM, "un nom trop long est coupé à 20 octets");
    verifier(toutAZero && calculerTotal(&joueur, NB_MANCHES) == 0, "les points partent de zéro");
    verifier(joueur.estHal == 0, "le joueur humain n'utilise pas les dés truqués");
}

/**
 * Rôle : lance tous les tests et affiche le bilan.
 * Retour : 0 si tous les tests réussissent, 1 sinon
 */
int main() {
    activerUtf8Console();

    // Graine fixe : les tirages, donc les résultats des tests, sont identiques à chaque exécution
    srand(2026);

    testerCalculerPoints();
    testerCalculerTotal();
    testerDeterminerGagnant();
    testerUnicodeVersUtf8();
    testerLargeurAffichee();
    testerDeEquilibre();
    testerDeTruque();
    testerRelancerDes();
    testerInitialiserJoueur();

    printf("\n%d vérifications, %d échec(s)\n", nbTests, nbEchecs);
    if (nbEchecs > 0) {
        return 1;
    }
    return 0;
}
