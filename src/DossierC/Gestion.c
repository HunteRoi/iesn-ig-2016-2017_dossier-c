#include "Gestion.h"

void garnirShiftNouv(int date, int heure, Shift * pShiftNouv);
void garnirDoubletteNouv(int matriculeUn, int matriculeDeux, Categorie categorie, Doublette * pNouvDoublette);
void afficherShift(Message * pLexique, Shift * pShiftLu);
void modifierDate(char dateAffichee[], int date);
void modifierHeure(char heureAffichee[], int heure);
bool doubletteTrouvee(Shift * pDebShifts, int matriculeUn, int matriculeDeux, Shift * * pShift, Doublette * * pDoublette, Doublette * * pDoubletteSauve);
void afficherDoublette(Message * pLexique, Doublette * pDoublette, Membre membres[], int iMembreUn, int iMembreDeux);
int indiceMembre(Membre membres[], int nbMembres, int matricule);


Erreur nouveauShift(Shift * * pShiftNouv) {
	*pShiftNouv = (Shift *)malloc(sizeof(Shift));
	if (pShiftNouv == NULL) return ERREUR_MEMOIRE;
	return PAS_D_ERREUR;
}

void ajouterShift(Shift * * pDebShifts, int date, int heure, Shift * pShiftNouv, Shift * * pShiftSauve) {
	garnirShiftNouv(date, heure, pShiftNouv);
	if (*pDebShifts == NULL) {
		*pDebShifts = pShiftNouv;
	} else {
		(*pShiftSauve)->pSuiv = pShiftNouv;
	}
	(*pShiftSauve) = pShiftNouv;
}

void garnirShiftNouv(int date, int heure, Shift * pShiftNouv) {
	pShiftNouv->date = date;
	pShiftNouv->heure = heure;
	pShiftNouv->nbDoublettes = 0;
	pShiftNouv->pDebDoublettes = NULL;
	pShiftNouv->pSuiv = NULL;
}

Erreur afficherShiftsIncomplets(Message * pLexique, Shift * pDebShifts) {
	Shift * pShiftLu = pDebShifts;
	Erreur codeErreur = SHIFTS_COMPLETS;
	NETTOYER_ECRAN;
	while (pShiftLu != NULL) {
		if (pShiftLu->nbDoublettes < NB_MAX_DOUBLETTES_PAR_SHIFT) {
			codeErreur = PAS_D_ERREUR;
			afficherShift(pLexique, pShiftLu);
			afficherMessage(pLexique, AFFICHER_PLACES_PRISES);
			printf("%d/%d", NB_MAX_DOUBLETTES_PAR_SHIFT-pShiftLu->nbDoublettes, NB_MAX_DOUBLETTES_PAR_SHIFT);
			A_LA_LIGNE;
			A_LA_LIGNE;
		}
		pShiftLu = pShiftLu->pSuiv;
	}
	return codeErreur;
}

void afficherShift(Message * pLexique, Shift * pShiftLu) {
	char dateAffichee[TAILLE_DATE];
	char heureAffichee[TAILLE_HEURE];
	modifierDate(dateAffichee, pShiftLu->date);
	modifierHeure(heureAffichee, pShiftLu->heure);
	changerCouleur(MAUVE_SUR_NOIR);
	afficherMessage(pLexique, CHOIX_DATE);
	puts(dateAffichee);
	changerCouleur(GRIS_SUR_NOIR);
	changerCouleur(MAUVE_SUR_NOIR);
	afficherMessage(pLexique, CHOIX_HEURE);
	puts(heureAffichee);
	changerCouleur(GRIS_SUR_NOIR);
}

Erreur chargementMembres(Membre membres[], int * nbMembres) {
	FILE * pMembres;
	Membre membreLu;
	*nbMembres = 0;
	fopen_s(&pMembres, "MembresFSBB.dat", "rb"); //fichier créé par le biais d'un programme externe au Dossier
	if (pMembres == NULL) {
		return ERREUR_OUVERTURE_FICHIER;
	}
	fread_s(&membreLu, sizeof(Membre), sizeof(Membre), 1, pMembres);
	while (!feof(pMembres) && *nbMembres <= NB_MEMBRES_MAX) {
		membres[*nbMembres] = membreLu;
		++(*nbMembres);
		fread_s(&membreLu, sizeof(Membre), sizeof(Membre), 1, pMembres);
	}
	return PAS_D_ERREUR;
}

Erreur nouvelleDoublette(Doublette * * pNouvDoublette) {
	*pNouvDoublette = (Doublette *)malloc(sizeof(Doublette));
	if (*pNouvDoublette == NULL) return ERREUR_MEMOIRE;
	return PAS_D_ERREUR;
}

bool shiftTrouve(Shift * pDebShifts, int date, int heure, Shift * * pShift) {
	*pShift = pDebShifts;
	while (*pShift != NULL && (*pShift)->date < date) {
		*pShift = (*pShift)->pSuiv;
	}
	while (*pShift != NULL && (*pShift)->date == date && (*pShift)->heure < heure) {
		*pShift = (*pShift)->pSuiv;
	}
	return (*pShift != NULL && (*pShift)->date == date && (*pShift)->heure == heure);
}

void listerMembres(Membre membres[], int nbMembres) {
	for (int i = 0; i < nbMembres; i++) {
		printf("%d %s %s %d", membres[i].matricule, membres[i].nom, membres[i].prenom, membres[i].moyenne);
		A_LA_LIGNE;
	}
}

bool membreTrouve(Membre membres[], int nbMembres, int matricule, int * indMembre) {
	bool trouve = false;
	*indMembre = 0;
	while (*indMembre < nbMembres && membres[*indMembre].matricule != matricule) {
		(*indMembre)++;
	}
	if (membres[*indMembre].matricule == matricule) {
		trouve = true;
	}
	return trouve;
}

bool membreDejaInscrit(Shift * pDebShifts, int matricule) {
	Shift * shiftLu = pDebShifts;
	while (shiftLu != NULL) {
		Doublette * inscriptionLue = shiftLu->pDebDoublettes;
		while (inscriptionLue != NULL) {
			if (inscriptionLue->matriculeUn == matricule || inscriptionLue->matriculeDeux == matricule) {
				return true;
			}
			inscriptionLue = inscriptionLue->pSuiv;
		}
		shiftLu = shiftLu->pSuiv;
	}
	return false;
}

Categorie categorie(int moy1, int moy2) {
	int moyTot = (int)((moy1 + moy2) / 2.);
	if (moyTot >= 200) {
		return CAT1;
	} else if (moyTot >= 190) {
		return CAT2;
	} else if (moyTot >= 180) {
		return CAT3;
	} else if (moyTot >= 170) {
		return CAT4;
	} else {
		return (moyTot >= 160) ? CAT5 : CAT6;
	}
}

void ajouterDoubletteShift(Shift * pShift, int matriculeUn, int matriculeDeux, Categorie categorie, Doublette * pNouvDoublette) {
	Doublette * p = pShift->pDebDoublettes;
	Doublette * pSauve = NULL;
	while (p != NULL) {
		pSauve = p;
		p = p->pSuiv;
	}
	garnirDoubletteNouv(matriculeUn, matriculeDeux, categorie, pNouvDoublette);
	if (p == pShift->pDebDoublettes) {
		pShift->pDebDoublettes = pNouvDoublette;
	} else {
		pSauve->pSuiv = pNouvDoublette;
	}
}

void garnirDoubletteNouv(int matriculeUn, int matriculeDeux, Categorie categorie, Doublette * pNouvDoublette) {
	pNouvDoublette->matriculeUn = matriculeUn;
	pNouvDoublette->matriculeDeux = matriculeDeux;
	pNouvDoublette->categorie = categorie;
	pNouvDoublette->pSuiv = NULL;
}

void majNbDoublettes(Shift * pShift, Maj choix) {
	pShift->nbDoublettes += choix;
}

void modifierDate(char dateAffichee[], int date) {
	dateAffichee[0] = ((int)(date / 10E6)) + '0';
	dateAffichee[1] = ((int)(date / 10E5) % 10) + '0';
	dateAffichee[2] = ((int)(date / 10E4) % 10) + '0';
	dateAffichee[3] = ((int)(date / 10E3) % 10) + '0';
	dateAffichee[4] = '/';
	dateAffichee[5] = ((int)(date / 10E2) % 10) + '0';
	dateAffichee[6] = ((int)(date / 100) % 10) + '0';
	dateAffichee[7] = '/';
	dateAffichee[8] = ((int)(date / 10) % 10) + '0';
	dateAffichee[9] = ((int)(date % 10)) + '0';
	dateAffichee[10] = '\0';
}

void modifierHeure(char heureAffichee[], int heure) {
	heureAffichee[0] = ((int) (heure / 1000)) + '0';
	heureAffichee[1] = ((int) (heure / 100) % 10) + '0';
	heureAffichee[2] = ':';
	heureAffichee[3] = ((int) (heure / 10) % 10) + '0';
	heureAffichee[4] = ((int) (heure % 10)) + '0';
	heureAffichee[5] = '\0';
}

bool doubletteTrouvee(Shift * pDebShifts, int matriculeUn, int matriculeDeux, Shift * * pShift, Doublette * * pDoublette, Doublette * * pDoubletteSauve) {
	*pShift = pDebShifts;
	while (*pShift != NULL) {
		*pDoublette = (*pShift)->pDebDoublettes;
		while (*pShift != NULL && *pDoublette != NULL) {
			if (matriculesIdentiques((*pDoublette)->matriculeUn,matriculeUn) && matriculesIdentiques((*pDoublette)->matriculeDeux,matriculeDeux)) {
				return true;
			}
			*pDoubletteSauve = *pDoublette;
			*pDoublette = (*pDoublette)->pSuiv;
		}
		*pShift = (*pShift)->pSuiv;
	}
	return false;
}

bool matriculesIdentiques(int matriculeMembreA, int matriculeMembreB) {
	return (matriculeMembreA == matriculeMembreB);
}

void supprimerDoublette(Shift * pShift, Doublette * pDoublette, Doublette * pDoubletteSauve) {
	detacherDoublette(pShift, pDoublette, pDoubletteSauve);
	free(pDoublette);
}

void afficherShiftsDoublettes(Message * pLexique, Shift * pDebShifts, Membre membres[], int nbMembres) {
	Shift * pShiftLu = pDebShifts;
	NETTOYER_ECRAN;
	while (pShiftLu != NULL) {
		Doublette * pDoublette;
		int nbDoublettes = 0;
		afficherShift(pLexique, pShiftLu);
		pDoublette = pShiftLu->pDebDoublettes;
		while (pDoublette != NULL && nbDoublettes <= pShiftLu->nbDoublettes) {
			int iMembreUn = indiceMembre(membres, nbMembres, pDoublette->matriculeUn);
			int iMembreDeux = indiceMembre(membres, nbMembres, pDoublette->matriculeDeux);
			nbDoublettes++;
			printf("\t\t");
			afficherMessage(pLexique, DOUBLETTE_NUMERO);
			printf("%d -", nbDoublettes);
			A_LA_LIGNE;
			afficherDoublette(pLexique, pDoublette, membres, iMembreUn, iMembreDeux);
			afficherMessage(pLexique, AFFICHER_DOUBLETTE_CAT);
			printf("%d", pDoublette->categorie);
			A_LA_LIGNE;
			A_LA_LIGNE;
			pDoublette = pDoublette->pSuiv;
		}
		if (pDoublette == pShiftLu->pDebDoublettes && nbDoublettes == 0) {
			afficherMessage(pLexique, PREMIERE_ERREUR + LISTE_INSCRIPTIONS_VIDE);
			A_LA_LIGNE;
			A_LA_LIGNE;
		}
		pShiftLu = pShiftLu->pSuiv;
	}
	OK;
	NETTOYER_ECRAN;
}

int indiceMembre(Membre membres[], int nbMembres, int matricule) {
	int i = 0;
	while (i < nbMembres && membres[i].matricule < matricule) { 
		i++;
	}
	return i;
}

void afficherDoublette(Message * pLexique, Doublette * pDoublette, Membre membres[], int iMembreUn, int iMembreDeux) {
	printf("--> %s %s et %s %s", membres[iMembreUn].nom, membres[iMembreUn].prenom, membres[iMembreDeux].nom, membres[iMembreDeux].prenom);
	A_LA_LIGNE;
}

void libererShifts(Shift * pDebShifts) {
	Shift * pShift = pDebShifts;
	Shift * pShiftSauve;
	while (pShift != NULL) {
		pShiftSauve = pShift->pSuiv;
		free(pShift);
		pShift = pShiftSauve;
	}
}

void afficherDoublettesCategories(Message * pLexique, Shift * pDebShifts, Membre membres[], int nbMembres) {
	Categorie cpt = CAT1;
	NETTOYER_ECRAN;
	while (cpt <= CAT6) {
		Shift * pShiftLu = pDebShifts;
		printf("\t\t");
		changerCouleur(MAUVE_SUR_NOIR);
		afficherMessage(pLexique, AFFICHER_CATEGORIES);
		printf("%d -", cpt);
		changerCouleur(GRIS_SUR_NOIR);
		A_LA_LIGNE;
		bool doubletteImprimee = false;
		while (pShiftLu != NULL) {
			Doublette * pDoublette = pShiftLu->pDebDoublettes;
			while (pDoublette != NULL) {
				if (pDoublette->categorie == cpt) {
					int iMembreUn = indiceMembre(membres, nbMembres, pDoublette->matriculeUn);
					int iMembreDeux = indiceMembre(membres, nbMembres, pDoublette->matriculeDeux);
					afficherDoublette(pLexique, pDoublette, membres, iMembreUn, iMembreDeux);
					doubletteImprimee = true;
				}
				pDoublette = pDoublette->pSuiv;
			}
			pShiftLu = pShiftLu->pSuiv;
		}
		if (!doubletteImprimee) {
			afficherMessage(pLexique, PREMIERE_ERREUR + CATEGORIE_VIDE);
			A_LA_LIGNE;
		}
		cpt++;
	}
	OK;
	NETTOYER_ECRAN;
}

void rattacherDoubletteAutreShift(Shift * pShiftDeplacement, Doublette * pDoublette) {
	Doublette * p = pShiftDeplacement->pDebDoublettes;
	Doublette * pSauve = NULL;
	while (p != NULL) {
		pSauve = p;
		p = p->pSuiv;
	}
	if (p == pShiftDeplacement->pDebDoublettes) {
		pShiftDeplacement->pDebDoublettes = pDoublette;
	} else {
		p = pDoublette;
		pSauve->pSuiv = p;
	}
	majNbDoublettes(pShiftDeplacement, PLUS);
}

void detacherDoublette(Shift * pShift, Doublette * pDoublette, Doublette * pDoubletteSauve) {
	if (pShift->pDebDoublettes == pDoublette) {
		pShift->pDebDoublettes = pDoublette->pSuiv;
	} else {
		pDoubletteSauve->pSuiv = pDoublette->pSuiv;
	}
}

bool estDoublette(Shift * pDebShifts, int matriculeUn, int matriculeDeux, Shift * * pShift, Doublette * * pDoublette, Doublette * * pDoubletteSauve) {
	bool estDoublette = doubletteTrouvee(pDebShifts, matriculeUn, matriculeDeux, pShift, pDoublette, pDoubletteSauve);
	if (!estDoublette) {
		estDoublette = doubletteTrouvee(pDebShifts, matriculeDeux, matriculeUn, pShift, pDoublette, pDoubletteSauve);
	}
	return estDoublette;
}