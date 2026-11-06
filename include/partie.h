#ifndef YAMS_PARTIE_H
#define YAMS_PARTIE_H

// ============================================================================
//  Module partie : déroulement du jeu (tours, manches, partie complète).
// ============================================================================

#include "types.h"

// Choix proposés au joueur après son lancer
#define CHOIX_GARDER        1
#define CHOIX_RELANCER_DE1  2
#define CHOIX_RELANCER_DE2  3
#define CHOIX_RELANCER_TOUT 4

/**
 * Rôle : prépare un joueur pour une nouvelle partie : copie son nom (coupé
 *        à TAILLE_NOM octets si besoin) et remet tous ses résultats à zéro.
 * But : partir d'une structure propre, sans valeurs aléatoires en mémoire.
 * Paramètres :
 *   - joueur : le joueur à initialiser
 *   - nom    : son nom
 *   - estHal : 1 pour l'ordinateur, 0 pour le joueur humain
 */
void initialiserJoueur(TJoueur *joueur, const char *nom, int estHal);

/**
 * Rôle : lance tous les dés d'une manche et calcule les points obtenus.
 * But : premier lancer de chaque manche, pour l'humain comme pour Hal.
 * Paramètres :
 *   - manche : la manche à remplir (dés et points)
 *   - estHal : 1 pour utiliser le dé truqué de Hal, 0 pour un dé équilibré
 */
void lancerDesManche(TManche *manche, int estHal);

/**
 * Rôle : relance le ou les dés demandés, puis recalcule les points
 *        (bonus compris) avec les nouvelles valeurs.
 * But : appliquer le choix du joueur après son premier lancer.
 * Paramètres :
 *   - manche : la manche en cours
 *   - choix  : CHOIX_RELANCER_DE1, CHOIX_RELANCER_DE2 ou CHOIX_RELANCER_TOUT
 *              (avec CHOIX_GARDER, les dés ne changent pas)
 *   - estHal : type de dé à utiliser pour la relance
 */
void relancerDes(TManche *manche, int choix, int estHal);

/**
 * Rôle : fait jouer Hal pour une manche : il lance ses dés une seule fois.
 * But : Hal joue avant l'humain, qui connaît donc le score à battre.
 * Paramètres :
 *   - hal          : l'ordinateur
 *   - indiceManche : numéro de la manche (à partir de 0)
 */
void jouerTourHal(TJoueur *hal, int indiceManche);

/**
 * Rôle : fait jouer l'humain pour une manche : premier lancer, puis menu
 *        pour garder ses dés ou en relancer (NB_TIRAGES - 1 relances au plus).
 * But : la partie interactive du jeu, celle où le joueur prend ses décisions.
 * Paramètres :
 *   - humain       : le joueur humain
 *   - indiceManche : numéro de la manche (à partir de 0)
 */
void jouerTourHumain(TJoueur *humain, int indiceManche);

/**
 * Rôle : joue une manche complète : en-tête, tour de Hal, tour de l'humain,
 *        puis bilan.
 * But : découper la partie en étapes simples et identiques.
 * Paramètres :
 *   - humain       : le joueur humain
 *   - hal          : l'ordinateur
 *   - indiceManche : numéro de la manche (à partir de 0)
 */
void jouerManche(TJoueur *humain, TJoueur *hal, int indiceManche);

/**
 * Rôle : joue les NB_MANCHES manches, affiche la feuille de score puis le
 *        vainqueur.
 * But : point d'entrée d'une partie, appelé depuis main.
 * Paramètres :
 *   - humain : le joueur humain, déjà initialisé
 *   - hal    : l'ordinateur, déjà initialisé
 * Retour : GAGNANT_HUMAIN, GAGNANT_HAL ou EGALITE (voir score.h)
 */
int jouerPartie(TJoueur *humain, TJoueur *hal);

#endif
