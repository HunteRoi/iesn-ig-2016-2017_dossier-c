#pragma once

#include "Gestion.h"
#include "Fichier.h"
#include "Lexique.h"

typedef struct infoMembre InfoMembre;
struct infoMembre {
	int matricule;
	int moy;
};

void dialogues(Message *pLexique, Shift *pDebShifts, Membre membres[], int nbMembres);