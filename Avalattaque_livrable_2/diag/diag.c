#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// decommenter pour passer en mode DEBUG
// #define __DEBUG__

#include "../include/avalam.h"
#include "../include/topologie.h"

#define MAX_NOTE 1000

int main(int argc, char *argv[])
{
    // declaration des variables
    T_Position plateau;
    char numDiag;
    char fen[NBCASES + 5];
    char note[MAX_NOTE];
    int i = 0, skip, bonus = 0, nbchar = 0, restant = 0;
    FILE *fichier;
    char nomFichier[100] = "../web/data/diag.js";
    char caractere = 'N';

    // ----- INIT -----
    plateau.evolution.malusJ = UNKNOWN;
    plateau.evolution.malusR = UNKNOWN;
    plateau.evolution.bonusJ = UNKNOWN;
    plateau.evolution.bonusR = UNKNOWN;

    // ----- NUMDIAG -----
    switch (argc)
    {
    case 3:
        // Vérification de numDiag
        for (i = 0; argv[1][i] != '\0'; i++)
            if (!isdigit(argv[1][i]))
            {
                printf("Le caractère '%c' à la position %d n'est pas un entier.\n", argv[1][i], i + 1);
                return 1;
            }

        // Convertir numDiag en entier
        numDiag = atoi(argv[1]);

        // Stockage du FEN
        strcpy(fen, argv[2]);
        break;

    default:
        printf("diag <numDiag> <fen>\n");
        return 1;
        break;
    }

    // ----- FICHIER DE DESTINATION
    printf("Le fichier de destination par défaut est : %s\nVoulez-vous le modifier ? (N,o): ", nomFichier);
    caractere = getchar();
    if (caractere == 'o')
    {
        printf("Renseignez le chemin vers le fichier de destination : ");
        scanf("%s", nomFichier);
    }

    // ----- FEN -----
    i = 0;
    while (fen[i] != ' ')
    {
        switch (fen[i])
        {
        // JAUNE
        case 'u':
            plateau.cols[i - bonus + restant].couleur = JAU;
            plateau.cols[i - bonus + restant].nb = 1;
            break;

        case 'd':
            plateau.cols[i - bonus + restant].couleur = JAU;
            plateau.cols[i - bonus + restant].nb = 2;
            break;

        case 't':
            plateau.cols[i - bonus + restant].couleur = JAU;
            plateau.cols[i - bonus + restant].nb = 3;
            break;

        case 'q':
            plateau.cols[i - bonus + restant].couleur = JAU;
            plateau.cols[i - bonus + restant].nb = 4;
            break;

        case 'c':
            plateau.cols[i - bonus + restant].couleur = JAU;
            plateau.cols[i - bonus + restant].nb = 5;
            break;

        // ROUGE
        case 'U':
            plateau.cols[i - bonus + restant].couleur = ROU;
            plateau.cols[i - bonus + restant].nb = 1;
            break;

        case 'D':
            plateau.cols[i - bonus + restant].couleur = ROU;
            plateau.cols[i - bonus + restant].nb = 2;
            break;

        case 'T':
            plateau.cols[i - bonus + restant].couleur = ROU;
            plateau.cols[i - bonus + restant].nb = 3;
            break;

        case 'Q':
            plateau.cols[i - bonus + restant].couleur = ROU;
            plateau.cols[i - bonus + restant].nb = 4;
            break;

        case 'C':
            plateau.cols[i - bonus + restant].couleur = ROU;
            plateau.cols[i - bonus + restant].nb = 5;
            break;

        // MALUS
        case 'm':
            bonus++;
            if (plateau.evolution.malusJ == UNKNOWN)
                plateau.evolution.malusJ = i - bonus;
            else
            {
                printf("il y a trop de malus/bonus !\n");
                return 1;
            }
            break;

        case 'M':
            bonus++;
            if (plateau.evolution.malusR == UNKNOWN)
                plateau.evolution.malusR = i - bonus;
            else
            {
                printf("il y a trop de malus/bonus !\n");
                return 1;
            }
            break;

        // BONUS
        case 'b':
            bonus++;
            if (plateau.evolution.bonusJ == UNKNOWN)
                plateau.evolution.bonusJ = i - bonus;
            else
            {
                printf("il y a trop de malus/bonus !\n");
                return 1;
            }
            break;

        case 'B':
            bonus++;
            if (plateau.evolution.bonusR == UNKNOWN)
                plateau.evolution.bonusR = i - bonus;
            else
            {
                printf("il y a trop de malus/bonus !\n");
                return 1;
            }
            break;

        // Cases à sauter
        default:
            if (isdigit(fen[i])) // Si le caractère est un chiffre
            {
                if (isdigit(fen[i + 1])) // Si le caractère suivant est un chiffre
                {
                    // On saute le nombre de cases indiqué par les deux chiffres
                    skip = (fen[i] - '0') * 10 + (fen[i + 1] - '0') - 2; 
                    i++;
                }
                else
                    // Si le caractère suivant n'est pas un chiffre
                    skip = fen[i] - '0' - 1;
                restant += skip;
            }
            else
            {
                printf("Le caractère '%c' à la position %d n'est pas valide !\n", fen[i], i);
                return 1;
            }
            break;
        }

        printf("FEN[i] = %c\n", fen[i]);
        printf("restant=%d\n", restant);
        i++;
    }

    i++;

    if (fen[i] == 'r')
        plateau.trait = ROU;
    else if (fen[i] == 'j')
        plateau.trait = JAU;
    else
    {
        printf("Le caractère '%c' à la position %d n'est pas valide\n", fen[i], i);
        return 1;
    }

    // Vérification longueur FEN
    fen[strlen(fen) - 2] = '\0';
    if (strlen(fen) > NBCASES + bonus + restant)
    {
        printf("\nLe FEN renseigné est trop grand !\n");
        return 1;
    }

    // DESCRIPTION
    printf("Renseignez une description (max. %d caractères) :\n", MAX_NOTE);
    while ((caractere = getchar()) != EOF)
    {
        if (caractere != '\n')
            note[nbchar] = caractere;
        else
            note[nbchar] = ' ';

        nbchar++;
    }

    note[nbchar] = '\0';

    if (nbchar >= MAX_NOTE)
    {
        printf("\nCette description est trop grande !\n");
        return 1;
    }

    // LOGS
    fichier = fopen(nomFichier, "w+");

    if (fichier == NULL)
        perror("\nLe fichier de destination est introuvable !\n");

    fprintf(fichier, "traiterJson({\n%s:%d,\n", STR_TURN, plateau.trait);
    fprintf(fichier, "%s:%d,\n", STR_NUMDIAG, numDiag);
    fprintf(fichier, "%s:\"%s\",\n", STR_NOTES, note);
    fprintf(fichier, "%s:\"%s\",\n", STR_FEN, fen);
    fprintf(fichier, "%s:%d,\n", STR_BONUS_J, plateau.evolution.bonusJ);
    fprintf(fichier, "%s:%d,\n", STR_MALUS_J, plateau.evolution.malusJ);
    fprintf(fichier, "%s:%d,\n", STR_BONUS_R, plateau.evolution.bonusR);
    fprintf(fichier, "%s:%d,\n", STR_MALUS_R, plateau.evolution.malusR);
    fprintf(fichier, "%s:[\n", STR_COLS);
    for (i = 0; i < NBCASES; i++)
    {
        if (plateau.cols[i].nb > 0 && plateau.cols[i].nb < 6 && plateau.cols[i].couleur == ROU || plateau.cols[i].couleur == JAU)
            fprintf(fichier, "\t{%s:%hhd, %s:%hhd},\n", STR_NB, plateau.cols[i].nb, STR_COULEUR, plateau.cols[i].couleur);
        else
            fprintf(fichier, "\t{%s:0, %s:0},\n", STR_NB, STR_COULEUR);
    }
    fputs("]\n});", fichier);
    fclose(fichier);
    printf("\nRéussite !\n");
    return 0;
}