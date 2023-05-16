/********* Moteur de tournoi : joueur ***************/
#include <stdio.h>
#include <stdlib.h>
#include "avalam.h"
#include "moteur.h"

int placerPionEvolution(T_Position p);
int eviterPionMalus(T_Position p, T_ListeCoups listeCoups);
int pileVoisine(T_Position p, T_ListeCoups listeCoups);
int contre_isolement(T_Position p, T_ListeCoups l);
int isolement(T_Position p, T_ListeCoups l);
int ecrasement(T_Position p, T_ListeCoups l);
int rechercheIndice(int origine, int destination, T_ListeCoups listeCoups);

void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups)
{
	// Cette fonction peut appeler la fonction ecrireIndexCoup(coupChoisi);
	// Pour sélectionner l'index d'un coup à jouer dans la liste l
	int i, o, d;
	int coup;
	
	// Priorités des stratégies:
	coup = placerPionEvolution(currentPosition);
	if (coup != -1)
	{
		printf("PLACER PION EVOLUTION\n"); // 1 - Placer pion evolution
		ecrireIndexCoup(coup);
		return;
	}
	coup = eviterPionMalus(currentPosition, listeCoups);
	if (coup != -1)
	{
		printf("EVITER PION MALUS\n"); // 2 - Eviter pion malus
		ecrireIndexCoup(coup);
		return;
	}
	coup = pileVoisine(currentPosition, listeCoups);
	if (coup != -1)
	{
		printf("PILES VOISINES\n"); // 3 - Piles voisines
		ecrireIndexCoup(coup);
		return;
	}
	coup = contre_isolement(currentPosition, listeCoups);
	if (coup != -1)
	{
		printf("CONTRE ISOLEMENT\n"); // 4 - Contre Isolement
		ecrireIndexCoup(coup);
		return;
	}
	coup = isolement(currentPosition, listeCoups);
	if (coup != -1)
	{
		printf("ISOLEMENT\n"); // 5 - Isolement
		ecrireIndexCoup(coup);
		return;
	}
	coup = ecrasement(currentPosition, listeCoups);
	if (coup != -1)
	{
		printf("ECRASEMENT\n"); // 6 - Ecrasement
		ecrireIndexCoup(coup);
		return;
	}
	ecrireIndexCoup(0);
}

//------------------->STRATEGIE PLACER PION EVOLUTION<-------------------
int placerPionEvolution(T_Position p)
{
	int i;
	for (i = 0; i < NBCASES; i++)
	{
		if (p.trait == 1)
		{
			if (p.evolution.malusJ == i) // Si on est jaune et que le malus jaune a déjà ete placé, on change de stratégie
				return -1;
		}
		else
		{
			if (p.evolution.malusR == i) // Même chose pour le joueur rouge
				return -1;
		}
	}
	for (i = 0; i < NBCASES; i++)
	{
		if (p.evolution.bonusR == i) // Si un pion évolution a déjà été placé, on ne place plus le pion bonus mais le pion malus
		{
			if (p.trait == 1)
				return 13; // Position choisie pour les jaunes: 26
			else
				return 6; // Position choisie pour les rouges: 14
		}
	}
	if (p.trait == 1)
		return 14; // Si on place le bonus jaune on le place en position 28 (coup 14)
	else
		return 23; // Si on place le bonus rouge, on choisit le coup 23
}

//------------------->STRATEGIE EVITER PION MALUS<-------------------
int eviterPionMalus(T_Position p, T_ListeCoups listeCoups)
{
	int i, j, k, l;
	int o, d;	   // Origine et destination
	int pos;	   // Enregistre la position d'un voisin du pion bonus
	int voisMalus; // Entregistre la position d'un voisin du pion malus
	int voisBonus;
	int indice;
	T_Voisins v; // Voisins du bonus
	T_Voisins m; // Voisins du pion malus
	for (i = 0; i < NBCASES; i++)
	{
		if (p.trait == 1) // JOUEUR JAUNE
		{
			if (p.evolution.bonusJ == i)
			{
				v = getVoisins(i); // On récupère les voisins du pion étudié
				for (j = 0; j < v.nb; j++)
				{
					pos = v.cases[j];					  // On enregistre la position des voisins dans pos
					if (p.evolution.malusR == v.cases[j]) // Si un des pion voisin est un malus rouge
					{
						m = getVoisins(pos);	   // On récupère les voisins du malus en question
						for (k = 0; k < m.nb; k++) // On recherche un voisin au malus rouge
						{
							voisMalus = m.cases[k];				// On enregistre la position du voisin dans posMalus
							if (p.cols[voisMalus].couleur == 2) // Le pion malus a un voisin rouge
							{
								o = pos;
								d = voisMalus;
								indice = rechercheIndice(o, d, listeCoups);
								return indice;
							}
						}
					}
				}
			}
		}
		else // JOUEUR ROUGE
		{
			if (p.evolution.bonusR == i)
			{
				v = getVoisins(i); // On récupère les voisins du pion étudié
				for (j = 0; j < v.nb; j++)
				{
					pos = v.cases[j];					  // On enregistre la position des voisins dans pos
					if (p.evolution.malusJ == v.cases[j]) // Si un des pion voisin est un malus rouge
					{
						m = getVoisins(pos);	   // On récupère les voisins du malus en question
						for (k = 0; k < m.nb; k++) // On recherche un voisin au malus rouge
						{
							voisMalus = m.cases[k];				// On enregistre la position du voisin dans posMalus
							if (p.cols[voisMalus].couleur == 1) // Le pion malus a un voisin rouge
							{
								o = pos;
								d = voisMalus;
								indice = rechercheIndice(o, d, listeCoups);
								return indice;
							}
						}
					}
				}
			}
		}
	}
	return -1;
}

//------------------->STRATEGIE PILES VOISINES<-------------------
int pileVoisine(T_Position p, T_ListeCoups listeCoups)
{
	int i, j;
	int voi;
	T_Voisins voisins;
	for (i = 0; i < NBCASES; i++)
	{
		voisins = getVoisins(i); // Récupère les voisins de
								 // chaques cases
		for (j = 0; j < voisins.nb; j++) // Pour chaque voisins de
		{								 // chaques case
			voi = voisins.cases[j];
			if (p.cols[i].nb + p.cols[voi].nb == 5)
			{
				if ((p.cols[i].couleur == p.trait) & (p.cols[i].nb != 0))																				// Une des deux piles est une
				{																																		// pile amie
					if (p.cols[voi].couleur != p.trait & (p.cols[voi].nb != 0))																			// L'autre pile est une pile
					{																																	// ennemie
						if (((p.evolution.bonusR != voi) & (p.evolution.bonusJ != voi)) & ((p.evolution.malusJ == voi) || (p.evolution.malusR == voi))) // Une des deux pile a un
																																						// malus et pas de bonus pour
																																						// compenser
						{
							printf("On joue le coup %d sur %d\n", voi, i);
							return rechercheIndice(voi, i, listeCoups);
						}
						else // Aucune des deux pile n'a de
						{	 // malus
							printf("On joue le coup %d sur %d\n", i, voi);
							return rechercheIndice(i, voi, listeCoups);
						}
					}
				}
			}
		}
	}
	return -1;
}

//------------------->STRATEGIE CONTRE ISOLEMENT<-------------------
int contre_isolement(T_Position p, T_ListeCoups l)
{
	int i, j;
	int voisin;
	int nbVoisin = 0;
	int amis = 1;
	T_Voisins Voisins;
	for (i = NBCASES - 1; i >= 0; i--)
	{
		Voisins = getVoisins(i);
		for (j = 0; j < Voisins.nb; j++)
		{
			voisin = Voisins.cases[j];
			if ((p.cols[voisin].nb != 0) & (p.cols[voisin].nb != 5))
				nbVoisin++;
		}
		if (i == 11)
			printf("TEST PION 11: %d\n", nbVoisin);
		if ((nbVoisin <= 2) & (p.cols[i].couleur != p.trait) & (p.cols[i].nb != 0) & (p.cols[i].nb != 5)) // Un pion adverse a moins de 3 voisins
		{
			if (nbVoisin == 1) // Est ce que le pion adverse n'a qu'un voisin ?
			{
				for (j = 0; j < Voisins.nb; j++)
				{
					if ((p.cols[Voisins.cases[j]].nb != 0) & (p.cols[Voisins.cases[j]].nb != 5))
						voisin = Voisins.cases[j];
				}
				if ((p.cols[voisin].couleur == p.trait) & (p.cols[voisin].nb + p.cols[voisin].nb < 6)) // Est ce que ce pion est un pion ami ?
				{
					printf("Cas 1:: On joue le coup %d sur %d\n", voisin, i);
					return rechercheIndice(voisin, i, l); // Ecraser le pion presque isolé par notre pion
				}
				else if (p.cols[i].nb + p.cols[voisin].nb < 6) // Le pion voisin est un pion ennemi
				{
					printf("Cas 2 :: On joue le coup %d sur %d\n", i, voisin);
					return rechercheIndice(i, voisin, l); // Rapprocher le pion presque isolé
				}
			}
			else // Le pion adverse a plus d'un voisin
			{
				for (j = 0; j < Voisins.nb; j++)
				{
					if ((p.cols[Voisins.cases[j]].nb != 0) & (p.cols[Voisins.cases[j]].couleur != p.trait))
						amis = 0;
					else if ((p.cols[Voisins.cases[j]].nb != 0))
						voisin = Voisins.cases[j];
				}
				if ((amis) & (p.cols[voisin].nb + p.cols[i].nb < 6)) // Les deux pions voisins sont des pions amis
				{
					printf("Cas 3 :: On joue le coup %d sur %d\n", voisin, i);
					return rechercheIndice(voisin, i, l); // Ecraser le pion presque isolé par un de nos pion
				}
				amis = 1;
			}
		}
		nbVoisin = 0;
	}
	return -1;
}

//------------------->STRATEGIE ISOLEMENT<------------------
int isolement(T_Position p, T_ListeCoups l)
{
	int i, j;
	int indice;
	int indice2;
	int adv = -1;
	int adv2 = -1;
	int ami;
	int voisin;
	int nbVoisin; // Nombre de voisins du pion
	int nbVoisinMin;
	int priorite = -1; // Indice du pion que l'on choisit d'isoler
	T_Voisins Voisins;
	T_Voisins Voisins2;
	printf("----------Appel à la fonction isolement\n");
	for (i = NBCASES - 1; i >= 0; i--)
	{
		Voisins = getVoisins(i);
		for (j = 0; j < Voisins.nb; j++) // Compte le nombre de voisin
		{
			voisin = Voisins.cases[j];
			if ((p.cols[voisin].nb != 0) & (p.cols[voisin].nb != 5))
				nbVoisin++;
		}
		if ((nbVoisin > 0) & (nbVoisin <= 4) & (p.cols[i].couleur == p.trait) & (p.cols[i].nb != 5) & (p.cols[i].nb != 0))
		{
			if (nbVoisin < nbVoisinMin)
			{
				nbVoisinMin = nbVoisin;
				priorite = i;
			}
		}
		nbVoisin = 0;
	}
	printf("On va isoler le pion %d\n", priorite);
	Voisins = getVoisins(priorite);
	for (i = 0; i < Voisins.nb; i++)
	{
		indice = Voisins.cases[i];
		if ((p.cols[indice].couleur != p.trait) & (p.cols[indice].nb != 0) & (p.cols[indice].nb != 5)) // Si notre pion a un voisin adverse;
		{
			adv = indice;
			printf("Le pion %d est un pion voisin et ennemi du pion %d", adv, priorite);
		}
	}
	indice2 = indice;
	if (adv != -1)
	{
		Voisins2 = getVoisins(adv);
		for (i = 0; i < Voisins2.nb; i++)
		{
			indice = Voisins2.cases[i];
			if ((p.cols[indice].couleur != p.trait) & (p.cols[indice].nb != 0) & (p.cols[indice].nb != 5)) // Si notre pion a un voisin adverse;
			{
				adv2 = indice;
				// printf("----->Le pion %d est un voisin ami du pion %d (col %d)\n",adv2,
				// adv,
			} // p.cols[adv].couleur);
		}
		if (adv2 != -1)
		{
			printf("Cas 0 :: On joue le coup %d sur %d\n", adv, adv2);
			return rechercheIndice(adv, adv2, l); // On écrase le pion adverse par un autre pion adverse
		}
		else // Un pion ennemi voisin du pion ennemi à isolé n'a pas de voisin ennemi
		{
			// printf("On va poser le pion  %d ennemi sur un de nos pions\n", adv);
			for (i = 0; i < Voisins2.nb; i++)
			{
				indice = Voisins2.cases[i];
				if ((indice != priorite) & (p.cols[indice].nb + p.cols[adv].nb < 6))
				{
					printf("On joue le coup %d sur %d\n", indice, adv);
					return rechercheIndice(indice, adv, l);
				}
			}
		}
		adv2 = -1;
	}
	else // Le pion à isoler n'a pas de voisin adverses
	{
		for (i = 0; i < Voisins.nb; i++)
		{
			indice = Voisins.cases[i];
			if ((indice != priorite) & (p.cols[indice].nb != 0))
				ami = indice;
		}
		Voisins2 = getVoisins(ami);
		for (i = 0; i < Voisins2.nb; i++)
		{
			indice = Voisins2.cases[i];
			if ((p.cols[indice].couleur != p.trait) & (p.cols[indice].nb != 0))
				adv = indice;
		}
		if ((adv != -1) & (p.cols[ami].nb + p.cols[adv].nb < 5)) // Le voisin du pion à isoler à un voisin adverse
		{
			printf("On va jouer le coup %d sur %d\n", ami, adv);
			return rechercheIndice(ami, adv, l);
		}
		else // Le pion voisin du pion à isoler n'a pas de voisin adverse
		{
			for (i = 0; i < Voisins2.nb; i++)
			{
				indice = Voisins2.cases[i];
				if ((indice != priorite) & (p.cols[indice].nb != 0))
					adv = indice;
			}
			if (p.cols[ami].nb + p.cols[adv].nb < 6)
			{
				printf("On joue le coup %d sur %d\n", ami, adv);
				return rechercheIndice(ami, adv, l);
			}
		}
	}
	adv = -1;
	return -1;
}

//------------------->STRATEGIE ECRASEMENT//-------------------
int ecrasement(T_Position p, T_ListeCoups l)
{
	int origine;
	int dest;
	int i, j;
	T_Voisins voisins;
	for (i = NBCASES - 1; i >= 0; i--)
	{
		if ((p.cols[i].couleur != p.trait) & (p.cols[i].nb != 0)) // Si un trouve un pion adverse
		{
			origine = i;
			voisins = getVoisins(origine);
			for (j = 0; j < voisins.nb; j++)
			{
				dest = voisins.cases[j];
				if ((p.cols[dest].couleur != p.trait) & (p.cols[dest].nb != 0) & (p.cols[dest].nb + p.cols[origine].nb < 6)) // Et que ce pion a un voisin adverse
				{
					printf("on joue le coup %d sur %d\n", origine, dest);
					return rechercheIndice(origine, dest, l);
				}
			}
			for (j = 0; j < voisins.nb; j++) // Le pion n'a aucun voisin adverse
			{
				dest = voisins.cases[j];
				if ((p.cols[dest].nb != 0) & (dest != origine) & (p.cols[dest].nb + p.cols[origine].nb < 6)) // Le pion a un voisin ami
				{
					printf("On joue le coup %d sur %d\n", dest, origine);
					return rechercheIndice(dest, origine, l);
				}
			}
		}
	}
	return -1;
}

//------------------->FONCTION RECHERCHE INDICE<-------------------
int rechercheIndice(int origine, int destination, T_ListeCoups listeCoups)
{
	int i;
	int o, d;
	printf("Appel à la fonction recherche Indice\n");
	for (i = 0; i < listeCoups.nb; i++)
	{
		o = listeCoups.coups[i].origine;
		d = listeCoups.coups[i].destination;
		if ((o == origine) && (d == destination))
			return i;
	}
	return -1;
}
