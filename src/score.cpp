// ============================================================================
//  Module score : calcul des points et désignation du gagnant.
//  La documentation de chaque fonction se trouve dans include/score.h.
// ============================================================================

#include "score.h"

int calculerPoints(int de1, int de2) {
    int points = de1 + de2;

    if (de1 == de2) {
        // Un double 1 est le coup le plus rare pour Hal : il vaut le plus gros bonus.
        if (de1 == 1) {
            points += BONUS_DOUBLE_UN;
        } else {
            points += BONUS_DOUBLE;
        }
    }
    return points;
}

int calculerTotal(const TJoueur *joueur, int nbManchesJouees) {
    int total = 0;

    for (int i = 0; i < nbManchesJouees; i++) {
        total += joueur->manches[i].points;
    }
    return total;
}

int determinerGagnant(int pointsHumain, int pointsHal) {
    if (pointsHumain > pointsHal) {
        return GAGNANT_HUMAIN;
    }
    if (pointsHal > pointsHumain) {
        return GAGNANT_HAL;
    }
    return EGALITE;
}
