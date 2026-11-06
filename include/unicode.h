#ifndef YAMS_UNICODE_H
#define YAMS_UNICODE_H

// ============================================================================
//  Module unicode : afficher des caractères spéciaux dans la console.
//
//  Les bords des dés (┏ ━ ┓ ┃ ┗ ┛) et leurs points (●) ne font pas partie du
//  code ASCII. Chaque caractère possède un numéro Unicode (son "code") qu'il
//  faut traduire en UTF-8, l'encodage que la console sait afficher.
// ============================================================================

// Codes Unicode des caractères de dessin (bloc "Box Drawing", U+2500 à U+257F)
#define CAR_COIN_HAUT_GAUCHE 0x250F  // ┏
#define CAR_COIN_HAUT_DROIT  0x2513  // ┓
#define CAR_COIN_BAS_GAUCHE  0x2517  // ┗
#define CAR_COIN_BAS_DROIT   0x251B  // ┛
#define CAR_HORIZONTAL       0x2501  // ━
#define CAR_VERTICAL         0x2503  // ┃
#define CAR_T_HAUT           0x2533  // ┳
#define CAR_T_BAS            0x253B  // ┻
#define CAR_T_GAUCHE         0x2523  // ┣
#define CAR_T_DROIT          0x252B  // ┫
#define CAR_CROIX            0x254B  // ╋
#define CAR_POINT            0x25CF  // ● : un point sur la face d'un dé

/**
 * Rôle : convertit un code Unicode (par exemple 0x250F pour ┏) en la suite
 *        d'octets UTF-8 qui représente ce caractère.
 * But : la console n'affiche pas un numéro Unicode tel quel ; elle attend du
 *       texte UTF-8, où un caractère occupe de 1 à 4 octets selon son code.
 * Paramètres :
 *   - code : le code Unicode du caractère, entre 0 et 0x10FFFF
 *   - utf8 : tableau d'au moins 5 cases, rempli par la fonction avec les
 *            octets UTF-8 suivis d'un '\0' (affichable avec printf("%s"))
 * Retour : le nombre d'octets écrits (1 à 4), ou 0 si le code n'est pas un
 *          caractère valide (utf8 contient alors une chaîne vide)
 */
int unicodeVersUtf8(char32_t code, unsigned char utf8[5]);

/**
 * Rôle : affiche un caractère Unicode dans la console.
 * But : dessiner les dés simplement, en écrivant afficherCaractere(CAR_POINT)
 *       sans se soucier de l'encodage à chaque fois.
 * Paramètres :
 *   - code : le code Unicode du caractère à afficher
 */
void afficherCaractere(char32_t code);

/**
 * Rôle : affiche plusieurs fois de suite le même caractère Unicode.
 * But : tracer les bords horizontaux des dés et du tableau des scores.
 * Paramètres :
 *   - code   : le code Unicode du caractère à répéter
 *   - nombre : combien de fois l'afficher (rien n'est affiché si nombre <= 0)
 */
void afficherCaractereRepete(char32_t code, int nombre);

/**
 * Rôle : compte le nombre de caractères d'un texte UTF-8, et non son nombre
 *        d'octets comme strlen ("Hélène" fait 6 caractères mais 8 octets).
 * But : aligner correctement les colonnes du tableau des scores, même quand
 *       le nom du joueur contient des accents.
 * Paramètres :
 *   - texte : chaîne UTF-8 terminée par '\0'
 * Retour : le nombre de caractères, soit la largeur occupée à l'écran
 *          (chaque caractère est compté sur une colonne)
 */
int largeurAffichee(const char *texte);

/**
 * Rôle : passe la console Windows en UTF-8 (page de code 65001).
 * But : sans cela, Windows afficherait des symboles incompréhensibles à la
 *       place des bords des dés et des accents. Sous Linux et macOS, la
 *       console est déjà en UTF-8 : la fonction ne fait rien.
 */
void activerUtf8Console();

#endif
