#ifndef YAMS_DES_H
#define YAMS_DES_H

// ============================================================================
//  Module des : tirages au hasard des dés.
//  Le joueur lance un dé équilibré. Hal, l'ordinateur, lance un dé truqué
//  qui tombe plus souvent sur les grandes valeurs.
// ============================================================================

#define MOYENNE_DE_HAL    4.0  // valeur moyenne du dé de Hal (3,5 pour un dé honnête)
#define ECART_TYPE_DE_HAL 1.0  // dispersion des résultats autour de cette moyenne

/**
 * Rôle : initialise le générateur de nombres pseudo-aléatoires (srand).
 * But : obtenir des tirages différents à chaque lancement du programme.
 *       Sans cet appel, rand() produirait toujours la même suite de nombres.
 */
void initialiserHasard();

/**
 * Rôle : tire un nombre réel au hasard entre 0 (inclus) et 1 (exclu),
 *        chaque valeur ayant la même chance de sortir (loi uniforme).
 * But : brique de base des deux types de dés.
 * Retour : un réel dans l'intervalle [0, 1[
 */
double tirageUniforme();

/**
 * Rôle : simule le lancer d'un dé honnête.
 * But : dé utilisé par le joueur humain.
 * Retour : un entier entre 1 et NB_FACES, chaque face ayant une chance sur 6
 */
int lancerDeEquilibre();

/**
 * Rôle : simule le lancer du dé truqué de Hal. Le résultat suit une loi
 *        normale (courbe en cloche) de moyenne MOYENNE_DE_HAL, arrondie à
 *        l'entier le plus proche et ramenée entre 1 et NB_FACES.
 * But : donner un avantage à l'ordinateur : il obtient souvent 3, 4 ou 5,
 *       donc plus de doubles qu'un joueur honnête.
 * Retour : un entier entre 1 et NB_FACES
 */
int lancerDeTruque();

/**
 * Rôle : lance le bon type de dé selon le joueur.
 * But : éviter de répéter le test "est-ce Hal ?" partout dans le programme.
 * Paramètres :
 *   - estHal : 1 pour le dé truqué de Hal, 0 pour le dé équilibré du joueur
 * Retour : la valeur obtenue, entre 1 et NB_FACES
 */
int lancerDe(int estHal);

#endif
