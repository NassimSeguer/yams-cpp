// ============================================================================
//  Module unicode : conversion Unicode -> UTF-8 et affichage des caractères.
//  La documentation de chaque fonction se trouve dans include/unicode.h.
// ============================================================================

#include <stdio.h>
#include <stdlib.h>
#include "unicode.h"

int unicodeVersUtf8(char32_t code, unsigned char utf8[5]) {
    int nbOctets;

    // En UTF-8, le premier octet indique la longueur du caractère (0xxxxxxx,
    // 110xxxxx, 1110xxxx ou 11110xxx) et chaque octet suivant commence par 10.
    // Les bits du code Unicode sont répartis dans les "x", du poids fort au poids faible.
    if (code >= 0xD800 && code <= 0xDFFF) {
        // Plage réservée par Unicode au fonctionnement de l'UTF-16 : pas un caractère
        nbOctets = 0;
    } else if (code <= 0x7F) {
        // 1 octet : 0xxxxxxx -> c'est le code ASCII, recopié tel quel
        utf8[0] = (unsigned char) code;
        nbOctets = 1;
    } else if (code <= 0x7FF) {
        // 2 octets : 110xxxxx 10xxxxxx -> 11 bits à répartir (5 + 6)
        utf8[0] = (unsigned char) (0xC0 | (code >> 6));
        utf8[1] = (unsigned char) (0x80 | (code & 0x3F));
        nbOctets = 2;
    } else if (code <= 0xFFFF) {
        // 3 octets : 1110xxxx 10xxxxxx 10xxxxxx -> 16 bits (4 + 6 + 6)
        // C'est le cas des caractères qui dessinent les dés.
        utf8[0] = (unsigned char) (0xE0 | (code >> 12));
        utf8[1] = (unsigned char) (0x80 | ((code >> 6) & 0x3F));
        utf8[2] = (unsigned char) (0x80 | (code & 0x3F));
        nbOctets = 3;
    } else if (code <= 0x10FFFF) {
        // 4 octets : 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx -> 21 bits (3 + 6 + 6 + 6)
        utf8[0] = (unsigned char) (0xF0 | (code >> 18));
        utf8[1] = (unsigned char) (0x80 | ((code >> 12) & 0x3F));
        utf8[2] = (unsigned char) (0x80 | ((code >> 6) & 0x3F));
        utf8[3] = (unsigned char) (0x80 | (code & 0x3F));
        nbOctets = 4;
    } else {
        // Au-delà de 0x10FFFF, aucun caractère Unicode n'existe
        nbOctets = 0;
    }

    // & 0x3F garde les 6 bits de poids faible, | 0x80 ajoute le préfixe 10.
    // On termine par '\0' pour obtenir une chaîne affichable avec printf.
    utf8[nbOctets] = '\0';
    return nbOctets;
}

void afficherCaractere(char32_t code) {
    unsigned char utf8[5];

    unicodeVersUtf8(code, utf8);
    printf("%s", (const char *) utf8);
}

void afficherCaractereRepete(char32_t code, int nombre) {
    unsigned char utf8[5];

    // On encode le caractère une seule fois, puis on l'affiche autant de fois que demandé.
    unicodeVersUtf8(code, utf8);
    for (int i = 0; i < nombre; i++) {
        printf("%s", (const char *) utf8);
    }
}

int largeurAffichee(const char *texte) {
    int largeur = 0;

    for (int i = 0; texte[i] != '\0'; i++) {
        // Les octets de suite d'un caractère UTF-8 s'écrivent 10xxxxxx :
        // on ne compte que les autres, donc un seul octet par caractère.
        if (((unsigned char) texte[i] & 0xC0) != 0x80) {
            largeur++;
        }
    }
    return largeur;
}

void activerUtf8Console() {
#ifdef _WIN32
    // "chcp 65001" choisit l'UTF-8 comme page de code ; "> nul" masque le message de confirmation.
    system("chcp 65001 > nul");
#endif
}
