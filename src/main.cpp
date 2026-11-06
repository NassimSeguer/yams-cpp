// ============================================================================
//  Yam's contre Hal : jeu de dés en console.
//
//  Le joueur affronte Hal, l'ordinateur, en NB_MANCHES manches de deux dés.
//  À chaque manche, il peut relancer un dé ou les deux pour améliorer son
//  score. Hal, lui, joue avec des dés truqués.
//
//  Organisation du code :
//    unicode   : affichage des caractères spéciaux (bords et points des dés)
//    des       : tirages au hasard (dé équilibré, dé truqué de Hal)
//    score     : règles de calcul des points
//    saisie    : lecture sûre du clavier
//    affichage : tout ce qui s'écrit à l'écran
//    partie    : enchaînement des tours et des manches
// ============================================================================

#include <stdio.h>
#include "types.h"
#include "unicode.h"
#include "des.h"
#include "score.h"
#include "saisie.h"
#include "affichage.h"
#include "partie.h"

/**
 * Rôle : point d'entrée du programme. Prépare la console et le hasard,
 *        demande le nom du joueur, puis enchaîne les parties tant que le
 *        joueur veut rejouer, en comptant les victoires de chacun.
 * But : relier les modules entre eux ; main ne contient aucune règle du jeu.
 * Retour : 0, le code qui signale au système que le programme s'est bien terminé
 */
int main() {
    char nom[TAILLE_NOM + 1];
    int victoiresHumain = 0;
    int victoiresHal = 0;
    int rejouer;

    activerUtf8Console();
    initialiserHasard();

    afficherTitre();
    afficherRegles();

    printf("Comment vous appelez-vous ?\n");
    lireNom(nom, TAILLE_NOM + 1);
    printf("\nBienvenue %s ! Hal vous attend...\n", nom);

    do {
        TJoueur humain;
        TJoueur hal;

        // Les joueurs sont réinitialisés à chaque partie : les scores repartent de zéro
        initialiserJoueur(&humain, nom, 0);
        initialiserJoueur(&hal, "Hal", 1);

        int gagnant = jouerPartie(&humain, &hal);
        if (gagnant == GAGNANT_HUMAIN) {
            victoiresHumain++;
        } else if (gagnant == GAGNANT_HAL) {
            victoiresHal++;
        }

        printf("\nParties gagnées : %s %d – Hal %d\n", nom, victoiresHumain, victoiresHal);
        printf("\nVoulez-vous rejouer ?\n  1 - Oui\n  2 - Non\n");
        rejouer = (lireEntier(1, 2) == 1);
    } while (rejouer);

    printf("\nMerci d'avoir joué, à bientôt !\n");
    return 0;
}
