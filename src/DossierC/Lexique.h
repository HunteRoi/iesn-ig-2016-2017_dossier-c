#pragma once

#include "Constantes.h"

typedef struct message Message;
struct message {
	int num;
	char texte[NB_CAR_MESSAGE_LANGUE];
	Message * pSuiv;
};

Erreur chargerLexique(Langues langue, Message * * pLexique);
Message * messageTrouve(Message * pLexique, NumMessage numMessage);
void afficherMessage(Message * pLexique, NumMessage numMessage);
int nbChoixMenu(Message * pMenu, NumMessage debMenu);
void libererLexique(Message * pLexique);
void changerCouleur(int numColor); /*fonction supplémentaire afin d'ajouter de la couleur à la console
								   /!\ portabilité nulle car ça demande des fonctions de la librairie Windows.h*/