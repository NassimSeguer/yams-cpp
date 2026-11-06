#ifndef YAMS_TYPES_H
#define YAMS_TYPES_H

// ============================================================================
//  Constantes et types communs à tous les modules du jeu.
//  Modifier une constante ici suffit à changer la règle partout
//  (par exemple NB_MANCHES pour jouer des parties plus longues).
// ============================================================================

#define NB_DES      2   // dés lancés par chaque joueur (le calcul des points est prévu pour 2)
#define NB_FACES    6   // un dé classique a 6 faces
#define NB_MANCHES  5   // nombre de manches dans une partie
#define NB_TIRAGES  2   // tirages par manche : le premier lancer + 1 relance
#define TAILLE_NOM  20  // nombre maximal d'octets dans le nom d'un joueur

/**
 * Résultat d'un joueur pour une manche.
 *   - des    : la valeur de chaque dé, entre 1 et NB_FACES
 *   - points : les points marqués avec ces dés (voir calculerPoints)
 */
typedef struct {
    int des[NB_DES];
    int points;
} TManche;

/**
 * Un participant de la partie : le joueur humain ou Hal, l'ordinateur.
 *   - nom     : chaîne de caractères terminée par '\0'
 *   - manches : le résultat de chacune des manches
 *   - estHal  : 1 pour l'ordinateur (il lance des dés truqués), 0 pour l'humain
 */
typedef struct {
    char nom[TAILLE_NOM + 1];   // + 1 pour le '\0' de fin de chaîne
    TManche manches[NB_MANCHES];
    int estHal;
} TJoueur;

#endif
