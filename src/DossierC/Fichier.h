#pragma once

#include "Gestion.h"

typedef struct doubletteFi DoubletteFi;
struct doubletteFi {
	int matriculeUn;
	int matriculeDeux;
	Categorie categorie;
};

typedef struct shiftFi ShiftFi;
struct shiftFi {
	int date;
	int heure;
	int nbDoublettes;
};

Erreur initialiserShifts(Shift * * pDebShifts);
Erreur sauverShifts(Shift * pDebShifts);