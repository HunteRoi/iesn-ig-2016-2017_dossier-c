#pragma once

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <Windows.h> //mauvaise portabilité, j'en suis conscient ; mais l'affichage est plus clean avec le system("cls");

#define NB_CAR_LANGUES 15
#define NB_CAR_MESSAGE_LANGUE 150
#define NBMAXCHOIXMENU 6
#define NB_CAR_NOM 80
#define NB_CAR_PRENOM 30
#define NB_MAX_DOUBLETTES_PAR_SHIFT 3
#define NB_MEMBRES_MAX 50
#define TAILLE_DATE 11
#define TAILLE_HEURE 6
#define PREMIER_CHOIX 1
#define NB_MEMBRES_DOUBLETTES_PAR_CAT 100

#define JAUNE_SUR_NOIR 14
#define ROUGE_SUR_NOIR 12
#define BLEU_SUR_NOIR 9
#define GRIS_SUR_NOIR 7
#define MAUVE_SUR_NOIR 5
#define VERT_SUR_NOIR 2

#define A_LA_LIGNE printf("\n")
#define NETTOYER_ECRAN system("cls")
#define OK getchar()

typedef enum numMessage NumMessage;
enum numMessage {
	MENU_PRINCIPAL = 1001,
	CHOIX_DATE = 2001,
	CHOIX_HEURE, //2002
	OBTENTION_MATRICULE, //2003
	OBTENTION_RECOMMENCER, //2004
	OBTENTION_CHOIX, //2005
	OBTENTION_DATE, //2006
	OBTENTION_HEURE, //2007
	DOUBLETTE_INSCRITE, //2008
	DOUBLETTE_DESINSCRITE, //2009
	DOUBLETTE_DEPLACEE, //2010
	DOUBLETTE_NUMERO, //2011
	AFFICHER_PLACES_PRISES, //2012
	AFFICHER_MEMBRES = 3001,
	AFFICHER_DOUBLETTE_CAT, //3002
	AFFICHER_CATEGORIES = 4001,
	PREMIERE_ERREUR = 5000
};

typedef enum erreur Erreur;
enum erreur { 
	PAS_D_ERREUR = 0,
	MAUVAIS_CHOIX, //1
	MEMBRE_INCONNU, //2
	MEMBRE_DEJA_INSCRIT, //3
	SHIFTS_COMPLETS, //4
	AUCUN_SHIFT_PROPOSE, //5
	SHIFT_INCONNU, //6
	DOUBLETTE_INCONNUE, //7
	ERREUR_MEMOIRE,  //8
	ERREUR_OUVERTURE_FICHIER, //9
	MEMBRE_IDENTIQUE, //10
	LISTE_INSCRIPTIONS_VIDE, //11
	CATEGORIE_VIDE, //12
	SHIFTS_IDENTIQUES //13
};

typedef enum choix Choix;
enum choix {
	NON, //0
	OUI //1
};

typedef enum maj Maj;
enum maj { 
	PLUS = +1,
	MOINS = -1
};

typedef enum categorie Categorie;
enum categorie {
	CAT1 = 1,
	CAT2, //2
	CAT3, //3
	CAT4, //4
	CAT5, //5
	CAT6 //6
};

typedef enum langues Langues;
enum langues {
	STOP = 0,
	FRANCAIS, //1
	ANGLAIS, //2
	NEERLANDAIS //3
};

typedef enum menu Menu;
enum menu {
	AJOUTER_INSCRIPTION = 1,
	SUPPRIMER_INSCRIPTION, //2
	IMPRIMER_SHIFTS, //3
	IMPRIMER_CATEGORIE, //4
	DEPLACER_DOUBLETTE, //5
	QUITTER //6
};

typedef enum accents Accents;
enum accents {
	EAIGU = 'é',
	AGRAVE = 'à',
	ECIRC = 'ê',
	ETREMA = 'ë',
	EGRAVE = 'è',
	ITREMA = 'ï',
	ICIRC = 'î',
	EAIGUMAJ = 'É',
	AGRAVEMAJ = 'À',
	ECIRCMAJ = 'Ê',
	ETREMAMAJ = 'Ë',
	EGRAVEMAJ = 'È',
	ITREMAMAJ = 'Ï',
	ICIRCMAJ = 'Î'
};