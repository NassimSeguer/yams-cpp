#ifndef YAMS_SAISIE_H
#define YAMS_SAISIE_H

// ============================================================================
//  Module saisie : lire ce que tape le joueur, sans jamais planter.
//  Toutes les lectures passent par lireLigne : on lit une ligne entière,
//  puis on l'analyse. Une saisie incorrecte ("abc", "7", "2x"...) est
//  refusée et la question est reposée.
// ============================================================================

/**
 * Rôle : lit une ligne tapée au clavier et retire le retour à la ligne final.
 *        Si la ligne est trop longue pour le tableau, la fin est ignorée.
 * But : base de toutes les saisies. Lire ligne par ligne évite les pièges
 *       de scanf (caractères laissés en attente pour la question suivante).
 * Paramètres :
 *   - tampon : tableau qui reçoit le texte lu, terminé par '\0'
 *   - taille : nombre de cases du tableau tampon
 * Retour : 1 si une ligne a été lue, 0 s'il n'y a plus rien à lire
 *          (fin de fichier, par exemple si l'entrée est redirigée)
 */
int lireLigne(char tampon[], int taille);

/**
 * Rôle : demande un nombre entier jusqu'à obtenir une valeur entre min et max.
 * But : lire les choix des menus (relancer un dé, rejouer...) de façon sûre.
 *       Si plus aucune saisie n'est possible, le programme s'arrête proprement.
 * Paramètres :
 *   - min : plus petite valeur acceptée
 *   - max : plus grande valeur acceptée
 * Retour : l'entier choisi par le joueur, compris entre min et max
 */
int lireEntier(int min, int max);

/**
 * Rôle : demande le nom du joueur, en refusant un nom vide.
 * But : personnaliser les messages et la feuille de score.
 *       Un nom trop long est coupé ; sans saisie possible, le nom "Joueur"
 *       est utilisé.
 * Paramètres :
 *   - nom    : tableau qui reçoit le nom, terminé par '\0'
 *   - taille : nombre de cases du tableau nom
 */
void lireNom(char nom[], int taille);

/**
 * Rôle : met le jeu en pause jusqu'à ce que le joueur appuie sur Entrée.
 * But : laisser le temps de lire le résultat d'une manche avant la suivante.
 */
void attendreEntree();

#endif
