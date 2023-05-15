#include <stdio.h>
#include <stdlib.h>
#include "../include/avalam.h"
#include "../include/moteur.h"
#include <unistd.h>

T_Voisins getVoisinsLegauxD1(T_ListeCoups listeCoups, octet pion)
{
    // Déclaration et initialisation des variables
    T_Voisins Voisins = getVoisins(pion);
    T_Voisins VoisinsLegaux;
    VoisinsLegaux.nb = 0;
    // On vérifie pour chaque voisin s'il fait partie des coups légaux
    for (int i = 0; i < Voisins.nb; i++)
    {
        for (int j = 0; j < listeCoups.nb; j++)
        {
            if (pion == listeCoups.coups[j].origine && Voisins.cases[i] == listeCoups.coups[j].destination)
            {
                // Si oui, on l'ajoute dans le tableau des voisins légaux
                VoisinsLegaux.cases[VoisinsLegaux.nb] = Voisins.cases[i];
                VoisinsLegaux.nb++;
            }
        }
    }
    // On renvoie les voisins légaux
    return VoisinsLegaux;
}

int evaluationplateau(T_Position currentPosition)
{
    // Déclaration et initialisation des variables
    int actualscore = 0, i, u;
    octet myColor = currentPosition.trait;
    T_Voisins VoisinsLegaux;
    T_Voisins VoisinsIllegaux;
    T_ListeCoups listeCoups;

    // On récupère la liste des coups légaux
    listeCoups = getCoupsLegaux(currentPosition);

    // On parcourt les colonnes
    for (i = 0; i < NBCASES - 1; i++)
    {
        // On récupère les voisins légaux et illégaux
        VoisinsLegaux = getVoisinsLegauxD1(listeCoups, i);
        VoisinsIllegaux = getVoisins(i);

        // On évalue la colonne en fonction de son nombre de pions, ici 1
        if (currentPosition.cols[i].nb == 1)
        {
            // Si la colonne n'a pas de voisins
            if (!nbVoisins(i))
            {
                if (currentPosition.cols[i].couleur == myColor)
                    actualscore = actualscore + 80;
                else
                    actualscore = actualscore - 80;
            }
            // Si la colonne a des voisins
            else
            {
                if (currentPosition.cols[i].couleur == myColor)
                    actualscore = actualscore + 5;
                else
                    actualscore = actualscore - 5;
            }

            // On évalue la colonne en fonction de ses voisins
            for (u = 0; u < 8; u++)
            {
                // Si le voisin est de la même couleur et qu'il a 4 pions
                if (currentPosition.cols[VoisinsLegaux.cases[u]].couleur == myColor && currentPosition.cols[VoisinsLegaux.cases[u]].nb == 4)
                {
                    // Si la colonne est de la même couleur
                    if (currentPosition.cols[i].couleur == myColor)
                        actualscore = actualscore + 15;
                    // Si la colonne n'est pas de la même couleur
                    else
                        actualscore = actualscore - 100;
                }
                // Si le voisin est d'une couleur différente et qu'il a 4 pions
                if (currentPosition.cols[VoisinsLegaux.cases[u]].couleur != myColor && currentPosition.cols[VoisinsLegaux.cases[u]].nb == 4)
                {
                    // Si la colonne est de la même couleur
                    if (currentPosition.cols[i].couleur == myColor)
                        actualscore = actualscore - 100;
                    // Si la colonne n'est pas de la même couleur
                    else
                        actualscore = actualscore - 100;
                }
            }
        }

        // On évalue la colonne en fonction de son nombre de pions, ici 2
        if (currentPosition.cols[i].nb == 2)
        {
            // Si la colonne n'a pas de voisins
            if (!nbVoisins(i))
            {
                if (currentPosition.cols[i].couleur == myColor)
                    actualscore = actualscore + 75;
                else
                    actualscore = actualscore - 75;
            }
            // Si la colonne a des voisins
            else
            {
                if (currentPosition.cols[i].couleur == myColor)
                    actualscore = actualscore + 12;
                else
                    actualscore = actualscore - 12;
            }

            // On évalue la colonne en fonction de ses voisins
            for (u = 0; u < 8; u++)
            {
                // Si le voisin est de la même couleur et qu'il a 2 pions
                if (currentPosition.cols[VoisinsLegaux.cases[u]].couleur == myColor && currentPosition.cols[VoisinsLegaux.cases[u]].nb == 2)
                {
                    // Si la colonne est de la même couleur
                    if (currentPosition.cols[i].couleur == myColor)
                        actualscore = actualscore + 5;
                    // Si la colonne n'est pas de la même couleur
                    else
                        actualscore = actualscore - 10;
                }
                // Si le voisin est d'une couleur différente et qu'il a 2 pions
                if (currentPosition.cols[VoisinsLegaux.cases[u]].couleur != myColor && currentPosition.cols[VoisinsLegaux.cases[u]].nb == 2)
                {
                    // Si la colonne est de la même couleur
                    if (currentPosition.cols[i].couleur == myColor)
                        actualscore = actualscore + 10;
                    // Si la colonne n'est pas de la même couleur
                    else
                        actualscore = actualscore - 5;
                }
                // Si le voisin est de la même couleur et qu'il a 3 pions
                if (currentPosition.cols[VoisinsLegaux.cases[u]].couleur == myColor && currentPosition.cols[VoisinsLegaux.cases[u]].nb == 3)
                {
                    // Si la colonne est de la même couleur
                    if (currentPosition.cols[i].couleur == myColor)
                        actualscore = actualscore + 10;
                    // Si la colonne n'est pas de la même couleur
                    else
                        actualscore = actualscore - 100;
                }
                // Si le voisin est d'une couleur différente et qu'il a 3 pions
                if (currentPosition.cols[VoisinsLegaux.cases[u]].couleur != myColor && currentPosition.cols[VoisinsLegaux.cases[u]].nb == 3)
                {
                    // Si la colonne est de la même couleur
                    if (currentPosition.cols[i].couleur == myColor)
                        actualscore = actualscore - 100;
                    // Si la colonne n'est pas de la même couleur
                    else
                        actualscore = actualscore - 100;
                }
                // Si le voisin est de la même couleur
                if (currentPosition.cols[VoisinsLegaux.cases[u]].couleur == myColor)
                {
                    // Si la colonne est de la même couleur
                    if (currentPosition.cols[i].couleur == myColor)
                        actualscore = actualscore + 5;
                    // Si la colonne n'est pas de la même couleur
                    actualscore = actualscore - 5;
                }
                // Si le voisin est d'une couleur différente
                if (currentPosition.cols[VoisinsLegaux.cases[u]].couleur != myColor)
                {
                    // Si la colonne est de la même couleur
                    if (currentPosition.cols[i].couleur == myColor)
                        actualscore = actualscore - 5;
                    // Si la colonne n'est pas de la même couleur
                    else
                        actualscore = actualscore + 5;
                }
                // Si pas de voisin 
                if (!currentPosition.cols[VoisinsIllegaux.cases[u]].nb)
                {
                    // Si la colonne est de la même couleur
                    if (currentPosition.cols[i].couleur == myColor)
                        actualscore = actualscore + 10;
                    // Si la colonne n'est pas de la même couleur
                    else
                        actualscore = actualscore - 10;
                }
            }
        }

        // On évalue la colonne en fonction de son nombre de pions, ici 3
        if (currentPosition.cols[i].nb == 3)
        {
            // Si la colonne n'a pas de voisins
            if (!nbVoisins(i))
            {
                // Si la colonne est de la même couleur
                if (currentPosition.cols[i].couleur == myColor)
                    actualscore = actualscore + 70;
                // Si la colonne n'est pas de la même couleur
                else
                    actualscore = actualscore - 50;
            }
            // Si la colonne a des voisins
            else
            {
                // Si la colonne est de la même couleur
                if (currentPosition.cols[i].couleur == myColor)
                    actualscore = actualscore + 14;
                // Si la colonne n'est pas de la même couleur
                else
                    actualscore = actualscore - 14;
            }

            // On évalue la colonne en fonction de ses voisins
            for (u = 0; u < 8; u++)
            {
                // Si le voisin est de la même couleur et qu'il a 2 pions
                if (currentPosition.cols[VoisinsLegaux.cases[u]].couleur == myColor && currentPosition.cols[VoisinsLegaux.cases[u]].nb == 2)
                {
                    // Si la colonne est de la même couleur
                    if (currentPosition.cols[i].couleur == myColor)
                        actualscore = actualscore + 15;
                    // Si la colonne n'est pas de la même couleur
                    else
                        actualscore = actualscore - 70;
                }
                // Si le voisin est d'une couleur différente et qu'il a 2 pions
                if (currentPosition.cols[VoisinsLegaux.cases[u]].couleur != myColor && currentPosition.cols[VoisinsLegaux.cases[u]].nb == 2)
                {
                    // Si la colonne est de la même couleur
                    if (currentPosition.cols[i].couleur == myColor)
                        actualscore = actualscore - 70;
                    // Si la colonne n'est pas de la même couleur
                    else
                        actualscore = actualscore - 70;
                }
                // Si le voisin est de la même couleur
                if (currentPosition.cols[VoisinsLegaux.cases[u]].couleur == myColor)
                {
                    // Si la colonne est de la même couleur
                    if (currentPosition.cols[i].couleur == myColor)
                        actualscore = actualscore + 5;
                    // Si la colonne n'est pas de la même couleur
                    else
                        actualscore = actualscore - 5;
                }
                // Si le voisin est d'une couleur différente
                if (currentPosition.cols[VoisinsLegaux.cases[u]].couleur != myColor)
                {
                    // Si la colonne est de la même couleur
                    if (currentPosition.cols[i].couleur == myColor)
                        actualscore = actualscore - 5;
                    // Si la colonne n'est pas de la même couleur
                    else
                        actualscore = actualscore + 5;
                }
                // Si pas de voisin
                if (!currentPosition.cols[VoisinsIllegaux.cases[u]].nb)
                {
                    // Si la colonne est de la même couleur
                    if (currentPosition.cols[i].couleur == myColor)
                        actualscore = actualscore + 10;
                    // Si la colonne n'est pas de la même couleur
                    else
                        actualscore = actualscore - 10;
                }
            }
        }

        // On évalue la colonne en fonction de son nombre de pions, ici 4
        if (currentPosition.cols[i].nb == 4)
        {
            // Si la colonne n'a pas de voisins
            if (!nbVoisins(i))
            {
                // Si la colonne est de la même couleur
                if (currentPosition.cols[i].couleur == myColor)
                    actualscore = actualscore + 65;
                // Si la colonne n'est pas de la même couleur
                else
                    actualscore = actualscore - 65;
            }
            // Si la colonne a des voisins
            else
            {
                // Si la colonne est de la même couleur
                if (currentPosition.cols[i].couleur == myColor)
                    actualscore = actualscore + 7;
                // Si la colonne n'est pas de la même couleur
                else
                    actualscore = actualscore - 7;
            }

            // On évalue la colonne en fonction de ses voisins
            for (u = 0; u < 8; u++)
            {
                // Si le voisin est de la même couleur
                if (currentPosition.cols[VoisinsLegaux.cases[u]].couleur == myColor)
                {
                    // Si la colonne est de la même couleur
                    if (currentPosition.cols[i].couleur == myColor)
                        actualscore = actualscore + 5;
                    // Si la colonne n'est pas de la même couleur
                    else
                        actualscore = actualscore - 5;
                }

                // Si le voisin est d'une couleur différente
                if (currentPosition.cols[VoisinsLegaux.cases[u]].couleur != myColor)
                {
                    // Si la colonne est de la même couleur
                    if (currentPosition.cols[i].couleur == myColor)
                        actualscore = actualscore - 5;
                    // Si la colonne n'est pas de la même couleur
                    else
                        actualscore = actualscore + 5;
                }

                // Si pas de voisin
                if (!currentPosition.cols[VoisinsIllegaux.cases[u]].nb)
                {
                    // Si la colonne est de la même couleur
                    if (currentPosition.cols[i].couleur == myColor)
                        actualscore = actualscore + 10;
                    // Si la colonne n'est pas de la même couleur
                    else
                        actualscore = actualscore - 10;
                }
            }
        }

        // On évalue la colonne en fonction de son nombre de pions, ici 5
        if (currentPosition.cols[i].nb == 5)
        {
            // Si la colonne n'a pas de voisins
            if (currentPosition.cols[i].couleur == myColor)
                actualscore = actualscore + 200;
            // Si la colonne a des voisins
            else
                actualscore = actualscore - 90;
        }
    }
    // On retourne le score
    return -actualscore;
}

int rechercheCoup(T_Position plateau, T_ListeCoups listeCoups, octet orig, octet dest)
{
    int d;
    listeCoups = getCoupsLegaux(plateau);
    for (d = 0; d < listeCoups.nb; d++)
        if (orig == listeCoups.coups[d].origine && dest == listeCoups.coups[d].destination)
            return d;
    return 1;
}

void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups)
{
    int bscore = -2000, score = -2000, v, aux = 0, bestp;
    int nvo, nvd, o, d, besto, bestd, bestoancien, bestdancien;
    T_Voisins listemalus, listemalus2;
    T_Position copieplateau;
    listemalus = getVoisins(currentPosition.evolution.bonusR);
    listemalus2 = getVoisins(currentPosition.evolution.malusJ);
    if (currentPosition.trait == 1)
    {
        if (!currentPosition.numCoup)
        {
            ecrireIndexCoup(rechercheCoup(currentPosition, listeCoups, 3, 3));
            return;
        }
        if (currentPosition.numCoup == 2)
        {

            {
                ecrireIndexCoup(rechercheCoup(currentPosition, listeCoups, listemalus.cases[2], listemalus.cases[2]));
                ecrireIndexCoup(rechercheCoup(currentPosition, listeCoups, listemalus.cases[3], listemalus.cases[3]));
                return;
            }
        }
    }
    if (currentPosition.trait == 2)
    {
        if (currentPosition.numCoup == 1)
        {
            if (currentPosition.evolution.bonusJ == 20)
            {
                ecrireIndexCoup(rechercheCoup(currentPosition, listeCoups, 7, 7));
                return;
            }
            ecrireIndexCoup(rechercheCoup(currentPosition, listeCoups, 20, 20));
            return;
        }

        if (currentPosition.numCoup == 3)
        {
            if (currentPosition.trait == 2)
            {
                for (int y = 0; y < 8; y++)
                {
                    for (int u = 0; u < 8; u++)
                    {
                        if (listemalus2.cases[y] == listemalus.cases[u])
                            aux++;
                    }
                }
                if (aux == 0)
                {
                    ecrireIndexCoup(rechercheCoup(currentPosition, listeCoups, listemalus.cases[2], listemalus.cases[2]));
                    ecrireIndexCoup(rechercheCoup(currentPosition, listeCoups, listemalus.cases[3], listemalus.cases[3]));
                }

                if (aux != 0)
                {
                    ecrireIndexCoup(rechercheCoup(currentPosition, listeCoups, listemalus2.cases[2], listemalus2.cases[2]));
                    ecrireIndexCoup(rechercheCoup(currentPosition, listeCoups, listemalus2.cases[3], listemalus2.cases[3]));
                }
            }
        }
    }

    if (currentPosition.numCoup == 4)
    {
        ecrireIndexCoup(rechercheCoup(currentPosition, listeCoups, 1, 4));
    }

    if (currentPosition.numCoup == 6)
    {
        ecrireIndexCoup(rechercheCoup(currentPosition, listeCoups, 13, 7));
    }

    if (currentPosition.numCoup == 5)
    {
        ecrireIndexCoup(rechercheCoup(currentPosition, listeCoups, 22, 21));
    }

    if (currentPosition.numCoup == 5)
    {
        ecrireIndexCoup(rechercheCoup(currentPosition, listeCoups, 29, 28));
    }

    T_ListeCoups listeref = getCoupsLegaux(currentPosition);

    for (int p = 0; p < listeref.nb; p++)
    {

        copieplateau.trait = currentPosition.trait;

        copieplateau.numCoup = currentPosition.numCoup;
        copieplateau.evolution.bonusJ = currentPosition.evolution.bonusJ;
        copieplateau.evolution.bonusR = currentPosition.evolution.bonusR;
        copieplateau.evolution.malusJ = currentPosition.evolution.malusJ;
        copieplateau.evolution.malusR = currentPosition.evolution.malusR;

        for (int q = 0; q < NBCASES; q++)
        {
            copieplateau.cols[q].nb = currentPosition.cols[q].nb;
            copieplateau.cols[q].couleur = currentPosition.cols[q].couleur;
        }

        listeCoups = getCoupsLegaux(currentPosition);

        o = listeCoups.coups[p].origine;
        d = listeCoups.coups[p].destination;

        if (estValide(copieplateau, o, d))
        {
            score = evaluationplateau(jouerCoup(copieplateau, o, d));

            if (score > bscore)
            {
                bscore = score;
                bestoancien = besto;
                besto = o;
                bestdancien = bestd;
                bestd = d;
                bestp = p;
                ecrireIndexCoup(rechercheCoup(copieplateau, listeCoups, o, d));
            }
        }
    }

    for (int m = 0; m < 8 * NBCASES; m++)
    {
        copieplateau.trait = currentPosition.trait;

        copieplateau.numCoup = currentPosition.numCoup;
        copieplateau.evolution.bonusJ = currentPosition.evolution.bonusJ;
        copieplateau.evolution.bonusR = currentPosition.evolution.bonusR;
        copieplateau.evolution.malusJ = currentPosition.evolution.malusJ;
        copieplateau.evolution.malusR = currentPosition.evolution.malusR;

        for (int q = 0; q < NBCASES; q++)
        {
            copieplateau.cols[q].nb = currentPosition.cols[q].nb;
            copieplateau.cols[q].couleur = currentPosition.cols[q].couleur;
        }
        nvo = listeCoups.coups[m].origine;
        nvd = listeCoups.coups[m].destination;
        jouerCoup(copieplateau, nvo, nvd);

        for (int n = 0; n < 8 * NBCASES; n++)
        {
            copieplateau.trait = currentPosition.trait;

            copieplateau.numCoup = currentPosition.numCoup;
            copieplateau.evolution.bonusJ = currentPosition.evolution.bonusJ;
            copieplateau.evolution.bonusR = currentPosition.evolution.bonusR;
            copieplateau.evolution.malusJ = currentPosition.evolution.malusJ;
            copieplateau.evolution.malusR = currentPosition.evolution.malusR;

            for (int w = 0; w < NBCASES; w++)
            {
                copieplateau.cols[w].nb = currentPosition.cols[w].nb;
                copieplateau.cols[w].couleur = currentPosition.cols[w].couleur;
            }
            listeCoups = getCoupsLegaux(copieplateau);
            o = listeCoups.coups[n].origine;
            d = listeCoups.coups[n].destination;
            score = evaluationplateau(jouerCoup(copieplateau, o, d));
            if (score > bscore + 150 && m == bestp)
                ecrireIndexCoup(rechercheCoup(currentPosition, listeCoups, bestoancien, bestdancien));
        }
    }
    for (int g = 0; g < 8 * NBCASES; g++)
    {
        copieplateau.trait = currentPosition.trait;
        copieplateau.numCoup = currentPosition.numCoup;
        copieplateau.evolution.bonusJ = currentPosition.evolution.bonusJ;
        copieplateau.evolution.bonusR = currentPosition.evolution.bonusR;
        copieplateau.evolution.malusJ = currentPosition.evolution.malusJ;
        copieplateau.evolution.malusR = currentPosition.evolution.malusR;
        for (int q = 0; q < NBCASES; q++)
        {
            copieplateau.cols[q].nb = currentPosition.cols[q].nb;
            copieplateau.cols[q].couleur = currentPosition.cols[q].couleur;
        }
        nvo = listeCoups.coups[g].origine;
        nvd = listeCoups.coups[g].destination;
        jouerCoup(copieplateau, nvo, nvd);
        for (int h = 0; h < 8 * NBCASES; h++)
        {
            copieplateau.trait = currentPosition.trait;
            copieplateau.numCoup = currentPosition.numCoup;
            copieplateau.evolution.bonusJ = currentPosition.evolution.bonusJ;
            copieplateau.evolution.bonusR = currentPosition.evolution.bonusR;
            copieplateau.evolution.malusJ = currentPosition.evolution.malusJ;
            copieplateau.evolution.malusR = currentPosition.evolution.malusR;
            for (int b = 0; b < NBCASES; b++)
            {
                copieplateau.cols[b].nb = currentPosition.cols[b].nb;
                copieplateau.cols[b].couleur = currentPosition.cols[b].couleur;
            }
            o = listeCoups.coups[h].origine;
            d = listeCoups.coups[h].destination;
            jouerCoup(copieplateau, nvo, nvd);
            for (int k = 0; k < 8 * NBCASES; k++)
            {
                copieplateau.trait = currentPosition.trait;
                copieplateau.numCoup = currentPosition.numCoup;
                copieplateau.evolution.bonusJ = currentPosition.evolution.bonusJ;
                copieplateau.evolution.bonusR = currentPosition.evolution.bonusR;
                copieplateau.evolution.malusJ = currentPosition.evolution.malusJ;
                copieplateau.evolution.malusR = currentPosition.evolution.malusR;
                for (int z = 0; z < NBCASES; z++)
                {
                    copieplateau.cols[z].nb = currentPosition.cols[z].nb;
                    copieplateau.cols[z].couleur = currentPosition.cols[z].couleur;
                }
                listeCoups = getCoupsLegaux(copieplateau);
                o = listeCoups.coups[g].origine;
                d = listeCoups.coups[g].destination;
                score = evaluationplateau(jouerCoup(copieplateau, o, d));
                if (score > bscore + 400)
                {
                    bscore = score - 400;
                    ecrireIndexCoup(rechercheCoup(currentPosition, listeCoups, nvo, nvd));
                }
            }
        }
    }
    return;
}
