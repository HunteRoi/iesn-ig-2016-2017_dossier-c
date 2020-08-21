#include "Lexique.h"

void nomFichierLangage(Langues langue, char nomFichier[]);
void modifierTexte(char texte[]);
void remplacerAccents(char texte[]);
void retirerALaLigne(char texte[]);

Erreur chargerLexique(Langues langue, Message * * pLexique) {
	char nomFichier[NB_CAR_LANGUES];
	char texte[NB_CAR_MESSAGE_LANGUE];
	int num;
	FILE * pFichier;
	Message * pLexiquePrec = NULL;
	nomFichierLangage(langue, nomFichier);
	fopen_s(&pFichier, nomFichier, "r");
	if (pFichier == NULL) {
		return ERREUR_OUVERTURE_FICHIER;
	}
	fscanf_s(pFichier, "%d", &num);
	while (!feof(pFichier)) {
		Message * pMessage = (Message *) calloc (1, sizeof(Message));
		if (pMessage == NULL) {
			fclose(pFichier);
			return ERREUR_MEMOIRE;
		}
		fgets(texte, NB_CAR_MESSAGE_LANGUE, pFichier);
		modifierTexte(texte);
		pMessage->num = num;
		strcpy_s(pMessage->texte, NB_CAR_MESSAGE_LANGUE, texte);
		//pMessage->pSuiv = NULL; non écrit car je fais un calloc
		if (*pLexique == NULL) {
			*pLexique = pMessage;
		} else {
			pLexiquePrec->pSuiv = pMessage;
		}
		pLexiquePrec = pMessage;
		fscanf_s(pFichier, "%d", &num);
	}
	fclose(pFichier);
	return PAS_D_ERREUR;
}

void nomFichierLangage(Langues langue, char nomFichier[]) {
	if (langue == FRANCAIS) {
		strcpy_s(nomFichier, NB_CAR_LANGUES, "Francais.txt");
	} else if (langue == ANGLAIS) {
		strcpy_s(nomFichier, NB_CAR_LANGUES, "English.txt");
	} else {
		strcpy_s(nomFichier, NB_CAR_LANGUES, "Nederlands.txt");
	}
}

Message * messageTrouve(Message * pLexique, NumMessage numMessage) {
	Message * pMessage = pLexique;
	while (pMessage != NULL && pMessage->num < numMessage) {
		pMessage = pMessage->pSuiv;
	}	
	return pMessage;
}

void afficherMessage(Message * pLexique, NumMessage numMessage) {
	Message * pMessage = messageTrouve(pLexique, numMessage);
	if (numMessage > PREMIERE_ERREUR) {
		changerCouleur(ROUGE_SUR_NOIR);
	}
	else if (numMessage > CHOIX_HEURE && numMessage <= OBTENTION_HEURE) {
		changerCouleur(BLEU_SUR_NOIR);
	}
	else if (numMessage >= DOUBLETTE_INSCRITE && numMessage <= DOUBLETTE_DEPLACEE) {
		changerCouleur(VERT_SUR_NOIR);
	}
	printf("%s", pMessage->texte);
	if (numMessage > PREMIERE_ERREUR || (numMessage > CHOIX_HEURE && numMessage <= OBTENTION_HEURE) || (numMessage >= DOUBLETTE_INSCRITE && numMessage <= DOUBLETTE_DEPLACEE)) {
		changerCouleur(GRIS_SUR_NOIR);
	}
}

int nbChoixMenu(Message * pMenu, NumMessage debMenu) {
	int maxChoix = 0;
	while (pMenu != NULL && pMenu->num <= debMenu + NBMAXCHOIXMENU) {
		maxChoix++;
		pMenu = pMenu->pSuiv;
	}
	return maxChoix;
}

void libererLexique(Message * pLexique) {
	Message * pMessage = pLexique;
	Message * pMessageSauve;
	while (pMessage != NULL) {
		pMessageSauve = pMessage->pSuiv;
		free(pMessage);
		pMessage = pMessageSauve;
	}
}

void modifierTexte(char texte[]) {
	remplacerAccents(texte); //remplacer les accents ("défectueux") du fichier .txt afin de les faire apparaître
	retirerALaLigne(texte);
}

void remplacerAccents(char texte[]) {
	for (int i = 0; i < NB_CAR_MESSAGE_LANGUE; i++) {
		switch (texte[i]) {
			case EAIGU:
				texte[i] = '\202';
				break;
			case AGRAVE:
				texte[i] = '\205';
				break;
			case ECIRC:
				texte[i] = '\210';
				break;
			case ETREMA:
				texte[i] = '\211';
				break;
			case EGRAVE:
				texte[i] = '\212';
				break;
			case ITREMA:
				texte[i] = '\213';
				break;
			case ICIRC:
				texte[i] = '\214';
				break;
			case EAIGUMAJ:
				texte[i] = '\265';
				break;
			case AGRAVEMAJ:
				texte[i] = '\267';
				break;
			case ECIRCMAJ:
				texte[i] = '\322';
				break;
			case ETREMAMAJ:
				texte[i] = '\323';
				break;
			case EGRAVEMAJ:
				texte[i] = '\324';
				break;
			case ICIRCMAJ:
				texte[i] = '\327';
				break;
			case ITREMAMAJ:
				texte[i] = '\330';
				break;
		}
	}
}

void retirerALaLigne(char texte[]) {
	for (int i = 0; i < NB_CAR_MESSAGE_LANGUE; i++) {
		if (texte[i] == '\n') {
			texte[i] = '\0';
		}
	}
}

void changerCouleur(int numColor) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, numColor);
}