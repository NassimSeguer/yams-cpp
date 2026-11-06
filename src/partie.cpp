// ============================================================================
//  Module partie : enchaînement des tours, des manches et de la partie.
//  La documentation de chaque fonction se trouve dans include/partie.h.
// ============================================================================

#include <stdio.h>
#include <string.h>
#include "types.h"
#include "des.h"
#include "score.h"
#include "saisie.h"
#include "affichage.h"
#include "partie.h"

void initialiserJoueur(TJoueur *joueur, const char *nom, int estHal) {
    // strncpy copie au plus TAILLE_NOM octets mais n'ajoute pas le '\0' si le
    // nom est trop long : on le place donc nous-mêmes dans la dernière case.
    strncpy(joueur->nom, nom, TAILLE_NOM);
    joueur->nom[TAILLE_NOM] = '\0';
    joueur->estHal = estHal;

    for (int i = 0; i < NB_MANCHES; i++) {
        for (int j = 0; j < NB_DES; j++) {
            joueur->manches[i].des[j] = 0;
        }
        joueur->manches[i].points = 0;
    }
}

void lancerDesManche(TManche *manche, int estHal) {
    for (int i = 0; i < NB_DES; i++) {
        manche->des[i] = lancerDe(estHal);
    }
    manche->points = calculerPoints(manche->des[0], manche->des[1]);
}

void relancerDes(TManche *manche, int choix, int estHal) {
    if (choix == CHOIX_RELANCER_DE1 || choix == CHOIX_RELANCER_TOUT) {
        manche->des[0] = lancerDe(estHal);
    }
    if (choix == CHOIX_RELANCER_DE2 || choix == CHOIX_RELANCER_TOUT) {
        manche->des[1] = lancerDe(estHal);
    }
    // Même règle qu'au premier lancer : un double obtenu en relançant rapporte aussi son bonus
    manche->points = calculerPoints(manche->des[0], manche->des[1]);
}

void jouerTourHal(TJoueur *hal, int indiceManche) {
    TManche *manche = &hal->manches[indiceManche];

    lancerDesManche(manche, hal->estHal);
    afficherTirage(hal->nom, manche);
}

void jouerTourHumain(TJoueur *humain, int indiceManche) {
    TManche *manche = &humain->manches[indiceManche];

    lancerDesManche(manche, humain->estHal);
    afficherTirage(humain->nom, manche);

    // Après le premier lancer, il reste NB_TIRAGES - 1 relances possibles
    for (int relancesRestantes = NB_TIRAGES - 1; relancesRestantes > 0; relancesRestantes--) {
        printf("\nQue voulez-vous faire ? (relances restantes : %d)\n", relancesRestantes);
        printf("  %d - Garder ces dés\n", CHOIX_GARDER);
        printf("  %d - Relancer le dé 1\n", CHOIX_RELANCER_DE1);
        printf("  %d - Relancer le dé 2\n", CHOIX_RELANCER_DE2);
        printf("  %d - Relancer les deux dés\n", CHOIX_RELANCER_TOUT);

        int choix = lireEntier(CHOIX_GARDER, CHOIX_RELANCER_TOUT);
        if (choix == CHOIX_GARDER) {
            break;  // le joueur est satisfait : inutile de proposer d'autres relances
        }
        relancerDes(manche, choix, humain->estHal);
        afficherTirage(humain->nom, manche);
    }
}

void jouerManche(TJoueur *humain, TJoueur *hal, int indiceManche) {
    afficherEnTeteManche(indiceManche);
    jouerTourHal(hal, indiceManche);
    jouerTourHumain(humain, indiceManche);
    afficherBilanManche(humain, hal, indiceManche);
}

int jouerPartie(TJoueur *humain, TJoueur *hal) {
    for (int i = 0; i < NB_MANCHES; i++) {
        jouerManche(humain, hal, i);
        // Pause entre deux manches, pour laisser le temps de lire le bilan
        if (i < NB_MANCHES - 1) {
            attendreEntree();
        }
    }

    afficherFeuilleDeScore(humain, hal, NB_MANCHES);
    afficherResultatFinal(humain, hal);
    return determinerGagnant(calculerTotal(humain, NB_MANCHES), calculerTotal(hal, NB_MANCHES));
}
