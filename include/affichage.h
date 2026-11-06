#ifndef YAMS_AFFICHAGE_H
#define YAMS_AFFICHAGE_H

// ============================================================================
//  Module affichage : tout ce qui s'écrit à l'écran (titre, règles, dés,
//  bilans et feuille de score). Aucune règle du jeu n'est décidée ici.
// ============================================================================

#include "types.h"

/**
 * Rôle : affiche une ligne de texte entourée d'un cadre (┏━━┓ ┃ ┃ ┗━━┛).
 * But : mettre en valeur le titre du jeu et le résultat final.
 * Paramètres :
 *   - texte : le texte à encadrer (UTF-8, accents acceptés)
 */
void afficherCadre(const char *texte);

/**
 * Rôle : affiche le titre du jeu dans un cadre.
 * But : écran d'accueil.
 */
void afficherTitre();

/**
 * Rôle : affiche les règles du jeu. Les valeurs (bonus, nombre de manches...)
 *        sont lues dans les constantes, donc restent toujours à jour.
 * But : expliquer le jeu au joueur avant la première manche.
 */
void afficherRegles();

/**
 * Rôle : dessine des dés côte à côte, avec leurs points, en caractères
 *        Unicode, puis écrit "Dé 1", "Dé 2"... sous chacun d'eux.
 * But : montrer le résultat d'un lancer de façon visuelle.
 * Paramètres :
 *   - des   : valeurs des dés, chacune entre 1 et NB_FACES
 *   - nbDes : nombre de dés à dessiner
 */
void afficherDes(const int des[], int nbDes);

/**
 * Rôle : affiche les dés d'un joueur et le détail du calcul de ses points
 *        (par exemple "Double 4 : 4 + 4 + 20 = 28 points").
 * But : informer le joueur après chaque lancer ou relance.
 * Paramètres :
 *   - nom    : nom du joueur qui vient de lancer
 *   - manche : ses dés et ses points pour la manche en cours
 */
void afficherTirage(const char *nom, const TManche *manche);

/**
 * Rôle : affiche le bandeau de début de manche ("Manche 2 / 5").
 * But : séparer visuellement les manches.
 * Paramètres :
 *   - indiceManche : numéro de la manche en partant de 0
 */
void afficherEnTeteManche(int indiceManche);

/**
 * Rôle : annonce le gagnant de la manche et le score total en cours.
 * But : permettre au joueur de suivre l'évolution de la partie.
 * Paramètres :
 *   - humain       : le joueur humain
 *   - hal          : l'ordinateur
 *   - indiceManche : numéro de la manche qui vient de se terminer (à partir de 0)
 */
void afficherBilanManche(const TJoueur *humain, const TJoueur *hal, int indiceManche);

/**
 * Rôle : affiche un tableau récapitulatif : les dés et les points de chaque
 *        joueur pour chaque manche, puis les totaux.
 * But : résumer toute la partie avant l'annonce du vainqueur.
 * Paramètres :
 *   - humain          : le joueur humain
 *   - hal             : l'ordinateur
 *   - nbManchesJouees : nombre de manches à afficher
 */
void afficherFeuilleDeScore(const TJoueur *humain, const TJoueur *hal, int nbManchesJouees);

/**
 * Rôle : annonce dans un cadre le vainqueur de la partie (ou l'égalité).
 * But : conclure la partie.
 * Paramètres :
 *   - humain : le joueur humain
 *   - hal    : l'ordinateur
 */
void afficherResultatFinal(const TJoueur *humain, const TJoueur *hal);

#endif
