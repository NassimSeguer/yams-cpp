// ============================================================================
//  Module saisie : lecture robuste du clavier.
//  La documentation de chaque fonction se trouve dans include/saisie.h.
// ============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "saisie.h"

int lireLigne(char tampon[], int taille) {
    // On vide le tampon d'affichage pour que la question apparaisse bien
    // avant que le programme se mette à attendre la réponse.
    fflush(stdout);

    if (fgets(tampon, taille, stdin) == NULL) {
        tampon[0] = '\0';
        return 0;
    }

    size_t longueur = strlen(tampon);
    if (longueur > 0 && tampon[longueur - 1] == '\n') {
        // Cas normal : la ligne entière tient dans le tampon, on enlève le '\n'
        tampon[longueur - 1] = '\0';
        longueur--;
    } else {
        // Ligne trop longue : on jette le reste pour ne pas fausser la saisie suivante
        int c;
        do {
            c = getchar();
        } while (c != '\n' && c != EOF);
    }

    // Un fichier écrit sous Windows peut finir ses lignes par "\r\n" : on retire le '\r'
    if (longueur > 0 && tampon[longueur - 1] == '\r') {
        tampon[longueur - 1] = '\0';
    }
    return 1;
}

int lireEntier(int min, int max) {
    char tampon[32];
    int valeur;
    char reste;

    while (1) {
        printf("-> ");
        if (!lireLigne(tampon, (int) sizeof(tampon))) {
            printf("\nPlus aucune saisie possible : fin du programme.\n");
            exit(0);
        }
        // sscanf renvoie le nombre d'éléments lus : 1 signifie "un entier et
        // rien d'autre derrière". Ainsi "2x" ou "abc" sont refusés.
        if (sscanf(tampon, "%d %c", &valeur, &reste) == 1 && valeur >= min && valeur <= max) {
            return valeur;
        }
        printf("Choix invalide : tapez un nombre entre %d et %d.\n", min, max);
    }
}

void lireNom(char nom[], int taille) {
    do {
        printf("-> ");
        if (!lireLigne(nom, taille)) {
            // Plus rien à lire : on prend un nom par défaut plutôt que de bloquer
            strncpy(nom, "Joueur", taille - 1);
            nom[taille - 1] = '\0';
            return;
        }
        if (nom[0] == '\0') {
            printf("Le nom ne peut pas être vide.\n");
        }
    } while (nom[0] == '\0');
}

void attendreEntree() {
    char tampon[8];

    printf("\n(Appuyez sur Entrée pour continuer)");
    lireLigne(tampon, (int) sizeof(tampon));
}
