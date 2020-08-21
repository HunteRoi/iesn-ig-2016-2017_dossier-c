#pragma once

#include "Constantes.h"
#include "Lexique.h"

typedef struct membre Membre;
struct membre {
	int matricule;
	char nom[NB_CAR_NOM];
	char prenom[NB_CAR_PRENOM];
	int moyenne;
};

typedef struct doublette Doublette;
struct doublette {
	int matriculeUn;
	int matriculeDeux;
	Categorie categorie;
	Doublette * pSuiv;
};

typedef struct shift Shift;
struct shift {
	int date;
	int heure;
	int nbDoublettes;
	Doublette * pDebDoublettes;
	Shift * pSuiv;
};

Erreur nouveauShift(Shift * * pShiftNouv);
void ajouterShift(Shift * * pDebShifts, int date, int heure, Shift * pShiftNouv, Shift * * pShiftSauve);
Erreur afficherShiftsIncomplets(Message * pLexique, Shift * pDebShifts);
Erreur nouvelleDoublette(Doublette * * pNouvDoublette);
bool shiftTrouve(Shift * pDebShifts, int date, int heure, Shift * * pShift);
Erreur chargementMembres(Membre membres[], int * nbMembres);
void listerMembres(Membre membres[], int nbMembres);
bool membreTrouve(Membre membres[], int nbMembres, int matricule, int * indMembre);
bool membreDejaInscrit(Shift * pDebShifts, int matricule);
Categorie categorie(int moy1, int moy2);
void ajouterDoubletteShift(Shift * pShift, int matriculeUn, int matriculeDeux, Categorie categorie, Doublette * pNouvDoublette);
void majNbDoublettes(Shift * pShift, Maj choix);
bool matriculesIdentiques(int matriculeUn, int matriculeDeux);
void supprimerDoublette(Shift * pShift, Doublette * pDoublette, Doublette * pDoubletteSauve);
void afficherShiftsDoublettes(Message * pLexique, Shift * pDebShifts, Membre membre[], int nbMembres);
void libererShifts(Shift * pDebShifts);
void afficherDoublettesCategories(Message * pLexique, Shift * pDebShifts, Membre membres[], int nbMembres);
void rattacherDoubletteAutreShift(Shift * pShiftDeplacement, Doublette * pDoublette);
void detacherDoublette(Shift * pShift, Doublette * pDoublette, Doublette * pDoubletteSauve);
bool estDoublette(Shift * pDebShifts, int matriculeUn, int matriculeDeux, Shift * * pShift, Doublette * * pDoublette, Doublette * * pDoubletteSauve);