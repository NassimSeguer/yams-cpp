// ============================================================================
//  Module des : tirages au hasard (dé équilibré et dé truqué de Hal).
//  La documentation de chaque fonction se trouve dans include/des.h.
// ============================================================================

#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "types.h"
#include "des.h"

static const double PI = 3.14159265358979323846;

void initialiserHasard() {
    // time(NULL) donne l'heure actuelle en secondes : la "graine" change à
    // chaque lancement, donc la suite de nombres produite par rand() aussi.
    srand((unsigned int) time(NULL));
}

double tirageUniforme() {
    // rand() renvoie un entier entre 0 et RAND_MAX (inclus). En divisant par
    // RAND_MAX + 1, le résultat reste strictement inférieur à 1.
    return rand() / (RAND_MAX + 1.0);
}

int lancerDeEquilibre() {
    // tirageUniforme() * 6 est dans [0, 6[ : sa partie entière vaut 0, 1, ..., 5
    // avec la même probabilité. On ajoute 1 pour obtenir une face de 1 à 6.
    return 1 + (int) (tirageUniforme() * NB_FACES);
}

int lancerDeTruque() {
    // Méthode de Box-Muller : à partir de deux tirages uniformes u1 et u2,
    // z = racine(-2 ln(1 - u1)) * cos(2 pi u2) suit une loi normale de
    // moyenne 0 et d'écart-type 1.
    // On utilise 1 - u1, qui est dans ]0, 1], car ln(0) n'existe pas.
    double u1 = tirageUniforme();
    double u2 = tirageUniforme();
    double z = sqrt(-2.0 * log(1.0 - u1)) * cos(2.0 * PI * u2);

    // On recentre la courbe sur la moyenne voulue (4 au lieu de 0).
    double valeur = MOYENNE_DE_HAL + ECART_TYPE_DE_HAL * z;

    // On arrondit à l'entier le plus proche, puis on ramène le résultat
    // entre 1 et 6, car une loi normale peut donner -1 ou 8 (rarement).
    int resultat = (int) lround(valeur);
    if (resultat < 1) {
        resultat = 1;
    }
    if (resultat > NB_FACES) {
        resultat = NB_FACES;
    }
    return resultat;
}

int lancerDe(int estHal) {
    if (estHal) {
        return lancerDeTruque();
    }
    return lancerDeEquilibre();
}
