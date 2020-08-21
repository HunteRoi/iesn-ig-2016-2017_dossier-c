#include "Interface.h"

Menu choixObtenu(Message *pLexique, int debMenu);
int afficherMenu(Message *pLexique, int debMenu);
InfoMembre matriculeObtenu(Message *pLexique, Shift *pDebShifts, Membre membres[], int nbMembres);
Shift * shiftObtenu(Message *pLexique, Shift *pDebShifts);
Erreur ajouterInscription(Message * pLexique, Shift * pDebShifts, Membre membres[], int nbMembres);
Erreur supprimerInscription(Message * pLexique, Shift * pDebShifts, Membre membres[], int nbMembres);
Erreur imprimerInscriptions(Message * pLexique, Shift * pDebShifts, Membre membres[], int nbMembres);
Erreur afficherParCategories(Message * pLexique, Shift * pDebShifts, Membre membres[], int nbMembres);
Erreur deplacerDoublette(Message * pLexique, Shift * pDebShifts, Membre membres[], int nbMembre);

void dialogues(Message * pLexique, Shift * pDebShifts, Membre membres[], int nbMembres) {
	Erreur codeErreur;
	int choixMenu = choixObtenu(pLexique, MENU_PRINCIPAL);
	while (choixMenu != QUITTER) {
		switch (choixMenu) {
			case AJOUTER_INSCRIPTION:
				codeErreur = ajouterInscription(pLexique, pDebShifts, membres, nbMembres);
				break;
			case SUPPRIMER_INSCRIPTION:
				codeErreur = supprimerInscription(pLexique, pDebShifts, membres, nbMembres);
				break;
			case IMPRIMER_SHIFTS:
				codeErreur = imprimerInscriptions(pLexique, pDebShifts, membres, nbMembres);
				break;
			case IMPRIMER_CATEGORIE:
				codeErreur = afficherParCategories(pLexique, pDebShifts, membres, nbMembres);
				break;
			case DEPLACER_DOUBLETTE:
				codeErreur = deplacerDoublette(pLexique, pDebShifts, membres, nbMembres);
				break;
		}
		if (codeErreur != PAS_D_ERREUR) {
			NETTOYER_ECRAN;
			afficherMessage(pLexique, codeErreur + PREMIERE_ERREUR);
			OK;
			NETTOYER_ECRAN;
		}
		if (codeErreur != ERREUR_MEMOIRE && codeErreur != ERREUR_OUVERTURE_FICHIER) {
			if (codeErreur != PAS_D_ERREUR) {
				Choix recommencer;
				bool estValide;
				do {
					afficherMessage(pLexique, OBTENTION_RECOMMENCER);
					scanf_s("%d", &recommencer);
					getchar();
					estValide = recommencer == OUI || recommencer == NON;
					if (!estValide) {
						afficherMessage(pLexique, PREMIERE_ERREUR + MAUVAIS_CHOIX);
						OK;
					}
					NETTOYER_ECRAN;
				} while (!estValide);
				if (recommencer == OUI) {
					switch (choixMenu) {
					case AJOUTER_INSCRIPTION:
						choixMenu = AJOUTER_INSCRIPTION;
						break;
					case SUPPRIMER_INSCRIPTION:
						choixMenu = SUPPRIMER_INSCRIPTION;
						break;
					case DEPLACER_DOUBLETTE:
						choixMenu = DEPLACER_DOUBLETTE;
						break;
					}
				}
				else {
					choixMenu = choixObtenu(pLexique, MENU_PRINCIPAL);
				}
			}
			else {
				choixMenu = choixObtenu(pLexique, MENU_PRINCIPAL);
			}
		}
	}
	codeErreur = sauverShifts(pDebShifts);
	if (codeErreur == ERREUR_OUVERTURE_FICHIER) {
		afficherMessage(pLexique, PREMIERE_ERREUR + ERREUR_OUVERTURE_FICHIER);
		OK;
		A_LA_LIGNE;
	}
	if (pDebShifts != NULL) {
		libererShifts(pDebShifts);
	}
}

Erreur ajouterInscription(Message * pLexique, Shift * pDebShifts, Membre membres[], int nbMembres) {
	InfoMembre membreUn;
	InfoMembre membreDeux;
	bool dejaInscrit;
	Shift * pShift;
	Doublette * pNouvDoublette;
	Erreur codeErreur = afficherShiftsIncomplets(pLexique, pDebShifts);
	if (codeErreur == SHIFTS_COMPLETS) {
		return SHIFTS_COMPLETS;
	}
	codeErreur = nouvelleDoublette(&pNouvDoublette);
	if (codeErreur == ERREUR_MEMOIRE) {
		return ERREUR_MEMOIRE;
	}
	pShift = shiftObtenu(pLexique, pDebShifts);
	membreUn = matriculeObtenu(pLexique, pDebShifts, membres, nbMembres);
	dejaInscrit = membreDejaInscrit(pDebShifts, membreUn.matricule);
	if (!dejaInscrit) {
		membreDeux = matriculeObtenu(pLexique, pDebShifts, membres, nbMembres);
		dejaInscrit = membreDejaInscrit(pDebShifts, membreDeux.matricule);
		if (!dejaInscrit && !matriculesIdentiques(membreUn.matricule, membreDeux.matricule)) {
			Categorie cat = categorie(membreUn.moy, membreDeux.moy);
			ajouterDoubletteShift(pShift, membreUn.matricule, membreDeux.matricule, cat, pNouvDoublette);
			majNbDoublettes(pShift, PLUS);
			NETTOYER_ECRAN;
			afficherMessage(pLexique, DOUBLETTE_INSCRITE);
			OK;
			NETTOYER_ECRAN;
		}
		else {
			codeErreur = (matriculesIdentiques(membreUn.matricule, membreDeux.matricule)) ? MEMBRE_IDENTIQUE : MEMBRE_DEJA_INSCRIT;
		}
	}
	else {
		codeErreur = MEMBRE_DEJA_INSCRIT;
	}
	return codeErreur;
}

Erreur supprimerInscription(Message * pLexique, Shift * pDebShifts, Membre membres[], int nbMembres) {
	Shift * pShift;
	Doublette * pDoublette;
	Doublette * pDoubletteSauve;
	InfoMembre membreUn = matriculeObtenu(pLexique, pDebShifts, membres, nbMembres);
	InfoMembre membreDeux = matriculeObtenu(pLexique, pDebShifts, membres, nbMembres);
	bool doubletteExiste = estDoublette(pDebShifts, membreUn.matricule, membreDeux.matricule, &pShift, &pDoublette, &pDoubletteSauve);
	if (doubletteExiste) {
		supprimerDoublette(pShift, pDoublette, pDoubletteSauve);
		majNbDoublettes(pShift, MOINS);
		NETTOYER_ECRAN;
		afficherMessage(pLexique, DOUBLETTE_DESINSCRITE);
		OK;
		NETTOYER_ECRAN;
		return PAS_D_ERREUR;
	} else {
		return DOUBLETTE_INCONNUE;
	}
}

Erreur imprimerInscriptions(Message * pLexique, Shift * pDebShifts, Membre membres[], int nbMembres) {
	afficherShiftsDoublettes(pLexique, pDebShifts, membres, nbMembres);
	return PAS_D_ERREUR;
}

Erreur afficherParCategories(Message * pLexique, Shift * pDebShifts, Membre membres[], int nbMembres) {
	afficherDoublettesCategories(pLexique, pDebShifts, membres, nbMembres);
	return PAS_D_ERREUR;
}

Menu choixObtenu(Message * pLexique, int debMenu) {
	bool choixValide;
	Menu choix;
	do {
		int maxChoix = afficherMenu(pLexique, debMenu);
		afficherMessage(pLexique, OBTENTION_CHOIX);
		scanf_s("%d", &choix);
		getchar();
		choixValide = (choix >= PREMIER_CHOIX && choix <= maxChoix);
		if (!choixValide) {
			NETTOYER_ECRAN;
			afficherMessage(pLexique, PREMIERE_ERREUR + MAUVAIS_CHOIX);
			OK;
			A_LA_LIGNE;
		}
		NETTOYER_ECRAN;
	} while (!choixValide);
	return choix;
}

int afficherMenu(Message * pLexique, int debMenu) {
	Message * pDebMenu = messageTrouve(pLexique, debMenu);
	Message * pMessage;
	char texte[NB_CAR_MESSAGE_LANGUE];
	int maxChoix = nbChoixMenu(pDebMenu->pSuiv, debMenu);
	int numChoix = 1;
	strcpy_s(texte, NB_CAR_MESSAGE_LANGUE, pDebMenu->texte);
	for (int i = 0; texte[i] != '\0'; i++) {
		texte[i] = toupper(pDebMenu->texte[i]);
	}
	changerCouleur(JAUNE_SUR_NOIR);
	printf("\t\t%s",texte);
	changerCouleur(GRIS_SUR_NOIR);
	A_LA_LIGNE;
	pMessage = pDebMenu->pSuiv;
	while (pMessage != NULL && numChoix <= maxChoix) {
		printf("%d. %s", numChoix, pMessage->texte);
		A_LA_LIGNE;
		numChoix++;
		pMessage = pMessage->pSuiv;
	}
	return maxChoix;
}

Shift * shiftObtenu(Message * pLexique, Shift * pDebShifts) {
	bool shiftExiste;
	int date;
	int heure;
	Shift * pShift;
	do {
		afficherMessage(pLexique, OBTENTION_DATE);
		scanf_s("%d", &date);
		getchar();
		afficherMessage(pLexique, OBTENTION_HEURE);
		scanf_s("%d", &heure);
		getchar();
		shiftExiste = shiftTrouve(pDebShifts, date, heure, &pShift);
		if (!shiftExiste) {
			afficherMessage(pLexique, PREMIERE_ERREUR + SHIFT_INCONNU);
			OK;
			A_LA_LIGNE;
		}
	} while (!shiftExiste);
	return pShift;
}

InfoMembre matriculeObtenu(Message * pLexique, Shift * pDebShifts, Membre membres[], int nbMembres) {
	InfoMembre membre;
	bool membreExiste;
	int indMembre;
	NETTOYER_ECRAN;
	afficherMessage(pLexique, AFFICHER_MEMBRES);
	A_LA_LIGNE;
	listerMembres(membres, nbMembres);
	do {
		afficherMessage(pLexique, OBTENTION_MATRICULE);
		scanf_s("%d", &membre.matricule);
		getchar();
		membreExiste = membreTrouve(membres, nbMembres, membre.matricule, &indMembre);
		if (!membreExiste) {
			afficherMessage(pLexique, PREMIERE_ERREUR + MEMBRE_INCONNU);
			OK;
			A_LA_LIGNE;
		}
	} while (!membreExiste);
	membre.moy = membres[indMembre].moyenne;
	return membre;
}

Erreur deplacerDoublette(Message * pLexique, Shift * pDebShifts, Membre membres[], int nbMembres) {
	Shift * pShift;
	Doublette * pDoublette;
	Doublette * pDoubletteSauve;
	Shift * pShiftDeplacement;
	Erreur codeErreur;
	bool doubletteExiste;
	InfoMembre membreUn = matriculeObtenu(pLexique, pDebShifts, membres, nbMembres);
	InfoMembre membreDeux = matriculeObtenu(pLexique, pDebShifts, membres, nbMembres);
	doubletteExiste = estDoublette(pDebShifts, membreUn.matricule, membreDeux.matricule, &pShift, &pDoublette, &pDoubletteSauve);
	if (doubletteExiste) {
		codeErreur = afficherShiftsIncomplets(pLexique, pDebShifts);
		if (codeErreur == SHIFTS_COMPLETS) {
			return SHIFTS_COMPLETS;
		}
		pShiftDeplacement = shiftObtenu(pLexique, pDebShifts);
		if (pShiftDeplacement == pShift) {
			return SHIFTS_IDENTIQUES;
		}
		rattacherDoubletteAutreShift(pShiftDeplacement, pDoublette);
		detacherDoublette(pShift, pDoublette, pDoubletteSauve);
		majNbDoublettes(pShift, MOINS);
		NETTOYER_ECRAN;
		afficherMessage(pLexique, DOUBLETTE_DEPLACEE);
		OK;
		NETTOYER_ECRAN;
		codeErreur = PAS_D_ERREUR;
	} else {
		codeErreur = DOUBLETTE_INCONNUE;
	}
	return codeErreur;
}