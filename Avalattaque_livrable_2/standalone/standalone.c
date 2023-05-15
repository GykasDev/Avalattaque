#include <stdio.h>
#include <string.h>

// decommenter pour passer en mode DEBUG
// #define __DEBUG__

#include "../include/avalam.h"
#include "../include/topologie.h"

int main(int argc, char *argv[])
{
    // declaration des variables
    T_Position plateau;
    T_ListeCoups listeCoups;
    T_Score score;
    octet origine, destination;
    FILE *fichier;
    char nomFichier[100] = "../web/data/refresh.js";
    int FLAG = VRAI, tours = 0;

    switch (argc)
    {
    case 1:
        // si il n'y a pas d'argument
        printf("Fichier par defaut --> '%s'\n", nomFichier);
        break;

    case 2:
        // si il y a un argument
        strcpy(nomFichier, argv[1]);
        printf("Fichier --> '%s'\n", nomFichier);
        break;

    default:
        printf("standalone [chemin]\n");
        return 1;
        break;
    }

    do
    {
        // si c'est le debut du jeu
        if (FLAG)
        {
            FLAG = FAUX;
            plateau = getPositionInitiale();

            // Placement des bonus jaunes
            do
            {
                if (FLAG)
                    printf("Vous ne pouvez pas jouer ici !\n\n");
                FLAG = VRAI;
                printf("\tBonus Jaune :");
                scanf("%hhd", &plateau.evolution.bonusJ);
            } while (plateau.evolution.bonusJ < 0 || plateau.evolution.bonusJ > 47 || plateau.cols[plateau.evolution.bonusJ].couleur == ROU);
            FLAG = FAUX;

            // Placement des bonus rouges
            do
            {
                if (FLAG)
                    printf("Vous ne pouvez pas jouer ici !\n\n");
                FLAG = VRAI;
                printf("\tBonus Rouge :");
                scanf("%hhd", &plateau.evolution.bonusR);
            } while (plateau.evolution.bonusR < 0 || plateau.evolution.bonusR > 47 || plateau.cols[plateau.evolution.bonusR].couleur == JAU);
            FLAG = FAUX;

            // Placement des malus jaunes
            do
            {
                if (FLAG)
                    printf("Vous ne pouvez pas jouer ici !\n\n");
                FLAG = VRAI;
                printf("\tMalus Jaune :");
                scanf("%hhd", &plateau.evolution.malusJ);
            } while (plateau.evolution.malusJ < 0 || plateau.evolution.malusJ > 47 || plateau.cols[plateau.evolution.malusJ].couleur == ROU || plateau.evolution.malusJ == plateau.evolution.bonusJ);
            FLAG = FAUX;

            // Placement des malus rouges
            do
            {
                if (FLAG)
                    printf("Vous ne pouvez pas jouer ici !\n\n");
                FLAG = VRAI;
                printf("\tMalus Rouge: ");
                scanf("%hhd", &plateau.evolution.malusR);
            } while (plateau.evolution.malusR < 0 || plateau.evolution.malusR > 47 || plateau.cols[plateau.evolution.malusR].couleur == JAU || plateau.evolution.malusR == plateau.evolution.bonusR);
            FLAG = FAUX;

            printf0("Fin de l'initialisation du jeu et début de la partie.\n");
        }
        // si le jeu a deja demarre 
        else
        {
            printf("\nCoup %d\n\t", tours);
            afficherScore(score);
            printf("\tTrait au %s :\n", COLNAME(plateau.trait));
            printf("\t\tPile d'origine :");
            scanf("%hhd", &origine);
            printf("\t\tPile de destination :");
            scanf("%hhd", &destination);

            printf0("Fin de la demande de mouvement et mise à jour du plateau.\n");

            plateau = jouerCoup(plateau, origine, destination);
        }

        printf0("Mise a jour du score.\n");

        score = evaluerScore(plateau);

        // ecriture du fichier

        printf0("Ouverture du fichier...\n");

        fichier = fopen(nomFichier, "w+");

        printf0("Insertion du texte dans le fichier...\n");

        fprintf(fichier, "traiterJson({%s:%d,", STR_TURN, plateau.trait);
        fprintf(fichier, "%s:%d,", STR_SCORE_J, score.nbJ);
        fprintf(fichier, "%s:%d,", STR_SCORE_J5, score.nbJ5);
        fprintf(fichier, "%s:%d,", STR_SCORE_R, score.nbR);
        fprintf(fichier, "%s:%d,", STR_SCORE_R5, score.nbR5);
        fprintf(fichier, "%s:%d,", STR_BONUS_J, plateau.evolution.bonusJ);
        fprintf(fichier, "%s:%d,", STR_MALUS_J, plateau.evolution.malusJ);
        fprintf(fichier, "%s:%d,", STR_BONUS_R, plateau.evolution.bonusR);
        fprintf(fichier, "%s:%d,", STR_MALUS_R, plateau.evolution.malusR);
        fprintf(fichier, "%s:[", STR_COLS);
        for (int i = 0; i < NBCASES; i++)
            fprintf(fichier, "{%s:%hhd, %s:%hhd},", STR_NB, plateau.cols[i].nb, STR_COULEUR, plateau.cols[i].couleur);
        fputs("]});", fichier);
        fclose(fichier);

        printf0("Fermeture du fichier.\n");
        printf0("Mise a jour des coups.\n");

        listeCoups = getCoupsLegaux(plateau);

        printf1("Fin du tours de boucle n°%d.\n", tours);

        tours++;

    } while (listeCoups.nb);

    printf0("Fin de la boucle principale (du jeu).\n");
    printf0("Annoncment des resultats.\n");

    printf("\n\t-----------------\n");
    printf("\t| FIN DE PARTIE |\n");
    printf("\t-----------------\n\n");

    if (score.nbJ > score.nbR)
        printf("Victoire des jaunes !\n\t%d a %d\n", score.nbJ, score.nbR);
    else if (score.nbJ < score.nbR)
        printf("Victoire des rouges !\n\t%d a %d\n", score.nbR, score.nbJ);
    else if (score.nbJ5 > score.nbR5)
        printf("Victoire des jaunes sur les piles de 5 !\n\t%d a %d\n", score.nbJ5, score.nbR5);
    else if (score.nbJ5 < score.nbR5)
        printf("Victoire des rouges sur les piles de 5 !\n\t%d a %d\n", score.nbR5, score.nbJ5);
    else
        printf("EGALITE\n");

    return 0;
}
