#include "Lexique.h"
#include "Interface.h"

void imprimerAccueil(void); //portabilité nulle
Langues langueLue(void);
void imprimerMessageErreur(Erreur codeErreur);

void main(void) {
	Langues langue;
	Erreur codeErreur;
	Message * pLexique = NULL;
	imprimerAccueil();
	langue = langueLue();
	if (langue != STOP) {
		codeErreur = chargerLexique(langue, &pLexique);
		if (codeErreur != PAS_D_ERREUR) {
			imprimerMessageErreur(codeErreur);
		} else {
			Shift * pDebShifts;
			codeErreur = initialiserShifts(&pDebShifts);
			if (codeErreur != PAS_D_ERREUR) {
				NETTOYER_ECRAN;
				afficherMessage(pLexique, codeErreur + PREMIERE_ERREUR);
				OK;
				A_LA_LIGNE;
			} else {
				int nbMembres;
				Membre membres[NB_MEMBRES_MAX];
				Erreur codeErreur = chargementMembres(membres, &nbMembres);
				if (codeErreur != PAS_D_ERREUR) {
					NETTOYER_ECRAN;
					afficherMessage(pLexique, codeErreur + PREMIERE_ERREUR);
					OK;
					A_LA_LIGNE;
				} else {
					dialogues(pLexique, pDebShifts, membres, nbMembres);
				}
			}
		}
		if (pLexique != NULL) {
			libererLexique(pLexique);
		}
	}
}

void imprimerAccueil(void) {
	//impression d'un affichage d'accueil
	for (int i = 0; i < 10; i++) {
		A_LA_LIGNE;
	}
	printf("\t\t\t\t\t_____________________________________");
	A_LA_LIGNE;
	A_LA_LIGNE;
	changerCouleur(BLEU_SUR_NOIR);
	printf("\t\t\t\t\t\t     DOSSIER DE C");
	changerCouleur(JAUNE_SUR_NOIR);
	A_LA_LIGNE;
	printf("\t\t\t\t\t\t   DEVRESSE TINA\323L");
	A_LA_LIGNE;
	printf("\t\t\t\t\t\t  HENALLUX IESN IG1C");
	A_LA_LIGNE;
	printf("\t\t\t\t\t     etu33784@student.henallux.be");
	changerCouleur(GRIS_SUR_NOIR);
	A_LA_LIGNE;
	printf("\t\t\t\t\t_____________________________________");
	getchar();
	NETTOYER_ECRAN;
}

Langues langueLue(void) {
	Langues choix;
	bool estValide;
	do {
		puts("0 - Quitter");
		puts("1 - Francais");
		puts("2 - English");
		puts("3 - Nederlands");
		//printf("Choix : ");
		changerCouleur(BLEU_SUR_NOIR);
		printf("Choice : ");
		changerCouleur(GRIS_SUR_NOIR);
		scanf_s("%d", &choix);
		getchar();
		estValide = (choix == FRANCAIS || choix == ANGLAIS || choix == NEERLANDAIS || choix == STOP);
		if (!estValide) {
			NETTOYER_ECRAN;
			changerCouleur(ROUGE_SUR_NOIR);
			//puts("Choix invalide. Veuillez recommencer.");
			printf("Unvalid choice. Please retry.");
			changerCouleur(GRIS_SUR_NOIR);
			OK;
		}
		NETTOYER_ECRAN;
	} while (!estValide);
	return choix;
}

void imprimerMessageErreur(Erreur codeErreur) {
	NETTOYER_ECRAN;
	switch (codeErreur) {
		case ERREUR_MEMOIRE:
			changerCouleur(ROUGE_SUR_NOIR);
			//puts("Erreur - plus de place m\202moire disponible.");
			puts("Error - no more place available to memorize the vocabulary.");
			changerCouleur(GRIS_SUR_NOIR);
			OK;
			break;
		case ERREUR_OUVERTURE_FICHIER:
			changerCouleur(ROUGE_SUR_NOIR);
			//puts("Erreur - ouverture du fichier impossible.");
			puts("Error - impossible to open the vocabulary's file.");
			changerCouleur(GRIS_SUR_NOIR);
			OK;
			break;
		default: puts("Error.");
		}
}