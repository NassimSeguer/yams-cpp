#ifndef YAMS_SCORE_H
#define YAMS_SCORE_H

// ============================================================================
//  Module score : règles de calcul des points et désignation du gagnant.
// ============================================================================

#include "types.h"

#define BONUS_DOUBLE    20  // bonus pour deux dés identiques
#define BONUS_DOUBLE_UN 40  // bonus pour deux "1" : le meilleur coup du jeu

// Valeurs renvoyées par determinerGagnant
#define EGALITE        0
#define GAGNANT_HUMAIN 1
#define GAGNANT_HAL    2

/**
 * Rôle : calcule les points rapportés par un lancer de deux dés.
 *        Règle : somme des deux dés, + BONUS_DOUBLE pour un double,
 *        + BONUS_DOUBLE_UN à la place pour un double 1.
 *        Exemples : 3 et 5 -> 8 ; 4 et 4 -> 28 ; 1 et 1 -> 42.
 * But : regrouper la règle de score en un seul endroit, utilisé après le
 *       premier lancer comme après une relance.
 * Paramètres :
 *   - de1 : valeur du premier dé (1 à NB_FACES)
 *   - de2 : valeur du second dé (1 à NB_FACES)
 * Retour : le nombre de points obtenus
 */
int calculerPoints(int de1, int de2);

/**
 * Rôle : additionne les points d'un joueur sur ses premières manches.
 * But : afficher le score en cours de partie et le total final.
 * Paramètres :
 *   - joueur          : le joueur dont on veut le total
 *   - nbManchesJouees : nombre de manches à additionner (0 à NB_MANCHES)
 * Retour : la somme des points de ces manches
 */
int calculerTotal(const TJoueur *joueur, int nbManchesJouees);

/**
 * Rôle : compare deux scores et indique qui l'emporte.
 * But : utilisée à la fois pour chaque manche et pour la partie entière.
 * Paramètres :
 *   - pointsHumain : score du joueur humain
 *   - pointsHal    : score de Hal
 * Retour : GAGNANT_HUMAIN, GAGNANT_HAL ou EGALITE
 */
int determinerGagnant(int pointsHumain, int pointsHal);

#endif
