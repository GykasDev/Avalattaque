/********* Moteur de tournoi : joueur ***************/

#include <stdio.h>
#include <stdlib.h>
#include "avalam.h"
#include "moteur.h"

int placerPionEvolution(T_Position p, T_ListeCoups listeCoups);
int eviterPionMalus(T_Position p, T_ListeCoups listeCoups);
int pileVoisine(T_Position p, T_ListeCoups listeCoups);
int contre_isolement(T_Position p, T_ListeCoups l);
int isolement(T_Position p, T_ListeCoups l);
int empilement(T_Position p, T_ListeCoups l);
int rechercheIndice(int origine, int destination, T_ListeCoups listeCoups);

// Fonction qui choisit le coup à jouer
void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups)
{
	int coup; // indice du coup à jouer

	// Si on est au début de la partie, on place les pions evolution
	if (currentPosition.numCoup < 4)
	{
		printf("PLACER PION EVOLUTION\n");
		coup = placerPionEvolution(currentPosition, listeCoups);
		ecrireIndexCoup(coup);
		return;
	}
	// Priorités des stratégies:
	// 1 - Éviter pion malus
	coup = eviterPionMalus(currentPosition, listeCoups);
	if (coup != -1)
	{
		printf("EVITER PION MALUS\n");
		ecrireIndexCoup(coup);
		return;
	}
	// 2 - Pile voisine
	coup = pileVoisine(currentPosition, listeCoups);
	if (coup != -1)
	{
		printf("PILES VOISINES\n");
		ecrireIndexCoup(coup);
		return;
	}
	// 3 - Contre isolement
	coup = contre_isolement(currentPosition, listeCoups);
	if (coup != -1)
	{
		printf("CONTRE ISOLEMENT\n");
		ecrireIndexCoup(coup);
		return;
	}
	// 4 - Isolement
	coup = isolement(currentPosition, listeCoups);
	if (coup != -1)
	{
		printf("ISOLEMENT\n");
		ecrireIndexCoup(coup);
		return;
	}
	// 5 - Empilement
	coup = empilement(currentPosition, listeCoups);
	if (coup != -1)
	{
		printf("EMPILEMENT\n");
		ecrireIndexCoup(coup);
		return;
	}
	// au cas ou aucune stratégie n'est possible mais normalement on ne devrait jamais arriver ici
	ecrireIndexCoup(0);
	return;
}

// Fonction qui place les pions evolution
int placerPionEvolution(T_Position p, T_ListeCoups listeCoups)
{
	int coup;				// indice du coup à jouer
	int bonusPos;			// Position du pion bonus
	int voisinPos;			// Position d'un voisin du pion bonus
	T_Voisins voisinsBonus; // Voisins du pion bonus

	if (p.numCoup > 1)
	{
		printf("PLACER PION MALUS\n");
		// Sélectionne la position du pion malus en fonction de la couleur du joueur
		bonusPos = (p.trait == JAU) ? p.evolution.bonusR : p.evolution.bonusJ;
		// Obtient les voisins de la position du pion malus
		voisinsBonus = getVoisins(bonusPos);
		// Parcourt les voisins du pion malus
		for (int i = 0; i < voisinsBonus.nb; i++)
		{
			voisinPos = voisinsBonus.cases[i];
			// Recherche un coup dans la liste des coups disponibles pour le voisin du pion malus
			coup = rechercheIndice(voisinPos, voisinPos, listeCoups);
			if (coup != -1)
				return coup; // Renvoie le coup trouvé
		}
	}

	printf("PLACER PION BONUS\n");
	// Place le pion bonus en fonction de la couleur du joueur
	return (p.trait == JAU) ? 14 : 23; // Renvoie la position du pion bonus
}

// Fonction qui permet d'éviter les pions malus
int eviterPionMalus(T_Position p, T_ListeCoups listeCoups)
{
	int pos;	   // Position d'un voisin du pion bonus
	int voisMalus; // Position d'un voisin du pion malus
	T_Voisins v;   // Voisins du pion bonus
	T_Voisins m;   // Voisins du pion malus

	// Parcourir toutes les cases du plateau
	for (int i = 0; i < NBCASES; i++)
	{
		// Vérifier si la case courante correspond à la position du pion bonus
		if (p.evolution.bonusJ == i || p.evolution.bonusR == i)
		{
			v = getVoisins(i); // Récupérer les voisins du pion bonus
			// Parcourir les voisins du pion bonus
			for (int j = 0; j < v.nb; j++)
			{
				pos = v.cases[j]; // Enregistrer la position du voisin
				// Vérifier si le voisin est un pion malus
				if (p.evolution.malusR == v.cases[j] || p.evolution.malusJ == v.cases[j])
				{
					m = getVoisins(pos); // Récupérer les voisins du pion malus
					// Parcourir les voisins du pion malus
					for (int k = 0; k < m.nb; k++)
					{
						voisMalus = m.cases[k]; // Enregistrer la position du voisin du malus
						// Vérifier si le voisin est un pion
						if (p.cols[voisMalus].couleur != p.trait)
							return rechercheIndice(pos, voisMalus, listeCoups);
					}
				}
			}
		}
	}

	return -1; // Si aucun coup n'a été trouvé, renvoyer -1
}

// Fonction qui permet de finir une pile
int pileVoisine(T_Position p, T_ListeCoups listeCoups)
{
	int i, j;		   // Variables de boucle
	int voi;		   // Position d'un voisin
	T_Voisins voisins; // Voisins d'une case
	for (i = 0; i < NBCASES; i++)
	{
		voisins = getVoisins(i);
		for (j = 0; j < voisins.nb; j++)
		{
			voi = voisins.cases[j];
			if (p.cols[i].nb + p.cols[voi].nb == 5) // Si la somme des pions des deux piles voisines est égale à 5
			{
				if ((p.cols[i].couleur == p.trait) && (p.cols[i].nb != 0))		 // Si la pile d'origine est de la couleur du joueur en cours et n'est pas vide
					if (p.cols[voi].couleur != p.trait && (p.cols[voi].nb != 0)) // Si la pile voisine est d'une couleur différente du joueur en cours et n'est pas vide
					{
						if (((p.evolution.bonusR != voi) && (p.evolution.bonusJ != voi)) & ((p.evolution.malusJ == voi) || (p.evolution.malusR == voi)))
							// Si la pile voisine n'est pas le bonus du joueur en cours et est le malus du joueur adverse
							return rechercheIndice(voi, i, listeCoups); // Jouer le coup pour inverser les piles
						else
							return rechercheIndice(i, voi, listeCoups); // Jouer le coup pour inverser les piles
					}
			}
		}
	}
	return -1; // Aucun coup possible pour finir une pile voisine
}

// Fonction qui permet de jouer un coup pour éviter l'isolement d'un pion adverse
int contre_isolement(T_Position p, T_ListeCoups l)
{
	int i, j;		   // Variables de boucle
	int voisin;		   // Position d'un voisin
	int nbVoisin = 0;  // Nombre de voisins
	int amis = 1;	   // Nombre de pions amis
	T_Voisins Voisins; // Voisins d'une case

	// Parcourir toutes les cases du plateau
	for (i = NBCASES - 1; i >= 0; i--)
	{
		Voisins = getVoisins(i);
		// Compter les voisins de la case courante
		for (j = 0; j < Voisins.nb; j++)
		{
			voisin = Voisins.cases[j];
			if ((p.cols[voisin].nb != 0) && (p.cols[voisin].nb != 5))
				nbVoisin++;
		}
		// Vérifier si la case courante est un pion adverse isolé
		if ((nbVoisin <= 2) & (p.cols[i].couleur != p.trait) && (p.cols[i].nb != 0) & (p.cols[i].nb != 5))
		{
			if (nbVoisin == 1)
			{
				// Trouver le voisin unique
				for (j = 0; j < Voisins.nb; j++)
				{
					if ((p.cols[Voisins.cases[j]].nb != 0) && (p.cols[Voisins.cases[j]].nb != 5))
						voisin = Voisins.cases[j];
				}

				// Jouer le coup pour éviter l'isolement
				if ((p.cols[voisin].couleur == p.trait) && (p.cols[voisin].nb + p.cols[voisin].nb < 6))
					return rechercheIndice(voisin, i, l);
				else if (p.cols[i].nb + p.cols[voisin].nb < 6)
					return rechercheIndice(i, voisin, l);
			}
			else
			{
				amis = 1;
				// Vérifier si tous les voisins sont des alliés
				for (j = 0; j < Voisins.nb; j++)
				{
					if ((p.cols[Voisins.cases[j]].nb != 0) && (p.cols[Voisins.cases[j]].couleur != p.trait))
						amis = 0;
					else if ((p.cols[Voisins.cases[j]].nb != 0))
						voisin = Voisins.cases[j];
				}
				// Jouer le coup pour éviter l'isolement
				if ((amis) && (p.cols[voisin].nb + p.cols[i].nb < 6))
					return rechercheIndice(voisin, i, l);
			}
		}
		nbVoisin = 0;
	}
	return -1;
}

// Fonction qui permet d'isoler un pion
int isolement(T_Position p, T_ListeCoups l)
{
	int i, j;			// Variables de boucle
	int indice;			// Position d'un voisin
	int adv = -1;		// Position d'un pion adverse
	int adv2 = -1;		// Position d'un pion adverse
	int ami;			// Position d'un pion allié
	int voisin;			// Position d'un voisin
	int nbVoisin;		// Nombre de voisins
	int nbVoisinMin;	// Nombre de voisins minimum
	int priorite = -1;	// Priorité de la case à isoler
	T_Voisins Voisins;	// Voisins d'une case
	T_Voisins Voisins2; // Voisins d'une case

	// Parcours des cases dans l'ordre inverse pour trouver la case avec le moins de voisins non vides
	for (i = NBCASES - 1; i >= 0; i--)
	{
		Voisins = getVoisins(i);
		for (j = 0; j < Voisins.nb; j++)
		{
			voisin = Voisins.cases[j];
			if ((p.cols[voisin].nb != 0) && (p.cols[voisin].nb != 5))
				nbVoisin++;
		}
		// Vérifie si la case i est de la couleur du joueur en cours, a moins de 5 pions et a entre 1 et 4 voisins non vides
		if ((nbVoisin > 0) && (nbVoisin <= 4) && (p.cols[i].couleur == p.trait) && (p.cols[i].nb != 5) && (p.cols[i].nb != 0))
		{
			if (nbVoisin < nbVoisinMin)
			{
				nbVoisinMin = nbVoisin;
				priorite = i;
			}
		}
		nbVoisin = 0;
	}
	// Recherche des adversaires de la case prioritaire
	Voisins = getVoisins(priorite);
	for (i = 0; i < Voisins.nb; i++)
	{
		indice = Voisins.cases[i];
		if ((p.cols[indice].couleur != p.trait) && (p.cols[indice].nb != 0) && (p.cols[indice].nb != 5))
			adv = indice;
	}
	if (adv != -1)
	{
		// Recherche des adversaires du premier adversaire
		Voisins2 = getVoisins(adv);
		for (i = 0; i < Voisins2.nb; i++)
		{
			indice = Voisins2.cases[i];
			if ((p.cols[indice].couleur != p.trait) && (p.cols[indice].nb != 0) && (p.cols[indice].nb != 5))
				adv2 = indice;
		}
		if (adv2 != -1)
			return rechercheIndice(adv, adv2, l); // Retourne l'indice du coup correspondant
		else
		{
			// Recherche d'une case voisine pour jouer un coup avec la case prioritaire
			for (i = 0; i < Voisins2.nb; i++)
			{
				indice = Voisins2.cases[i];
				if ((indice != priorite) && (p.cols[indice].nb + p.cols[adv].nb < 6))
					return rechercheIndice(indice, adv, l); // Retourne l'indice du coup correspondant
			}
		}
		adv2 = -1;
	}
	else
	{
		// Recherche d'un allié parmi les voisins de la case prioritaire
		for (i = 0; i < Voisins.nb; i++)
		{
			indice = Voisins.cases[i];
			if ((indice != priorite) && (p.cols[indice].nb != 0))
				ami = indice;
		}
		Voisins2 = getVoisins(ami);
		for (i = 0; i < Voisins2.nb; i++)
		{
			indice = Voisins2.cases[i];
			if ((p.cols[indice].couleur != p.trait) && (p.cols[indice].nb != 0))
				adv = indice;
		}
		if ((adv != -1) && (p.cols[ami].nb + p.cols[adv].nb < 5))
			return rechercheIndice(ami, adv, l); // Retourne l'indice du coup correspondant
		else
		{
			// Recherche d'une autre case voisine pour jouer un coup avec l'allié
			for (i = 0; i < Voisins2.nb; i++)
			{
				indice = Voisins2.cases[i];
				if ((indice != priorite) && (p.cols[indice].nb != 0))
					adv = indice;
			}
			if (p.cols[ami].nb + p.cols[adv].nb < 6)
				return rechercheIndice(ami, adv, l); // Retourne l'indice du coup correspondant
		}
	}
	return -1; // Aucun coup possible trouvé, retourne -1
}

// Fonction qui permet de jouer un coup pour empiler 2 pions
int empilement(T_Position p, T_ListeCoups l)
{
	int origine;
	int dest;
	int i, j;
	T_Voisins voisins;

	// On parcours les cases de la position
	for (i = NBCASES - 1; i >= 0; i--)
	{
		// On cherche une case de notre couleur non vide
		if ((p.cols[i].couleur != p.trait) & (p.cols[i].nb != 0))
		{
			origine = i;
			voisins = getVoisins(origine);
			// On cherche une case de l'adversaire non vide
			for (j = 0; j < voisins.nb; j++)
			{
				dest = voisins.cases[j];
				if ((p.cols[dest].couleur != p.trait) & (p.cols[dest].nb != 0) & (p.cols[dest].nb + p.cols[origine].nb < 6))
				{
					// On a trouvé : on retourne l'indice
					return rechercheIndice(origine, dest, l);
				}
			}
			// On cherche une case de l'adversaire vide
			for (j = 0; j < voisins.nb; j++)
			{
				dest = voisins.cases[j];
				if ((p.cols[dest].nb != 0) & (dest != origine) & (p.cols[dest].nb + p.cols[origine].nb < 6))
				{
					// On a trouvé : on retourne l'indice
					return rechercheIndice(origine, dest, l);
				}
			}
		}
	}
	return -1;
}

// Fonction qui permet de trouver le coup voulue dans la liste des coups
int rechercheIndice(int origine, int destination, T_ListeCoups listeCoups)
{
	for (int i = 0; i < listeCoups.nb; i++)
	{
		if ((listeCoups.coups[i].origine == origine) && (listeCoups.coups[i].destination == destination))
			return i;
	}
	return -1;
}