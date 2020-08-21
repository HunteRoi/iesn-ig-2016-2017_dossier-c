#include "Fichier.h"

bool premiereFois(void); /* PP adapté au code C (correspond à detectionFichierShifts)
						 NB : je ne teste pas l'existence du fichier DatesOrgShifts.dat car ce test est fait plus bas
						 => risque de redondance */
Erreur chargementDatesOrgShifts(Shift * * pDebShifts);
Erreur chargementInscriptions(Shift * * pDebShifts);

bool premiereFois(void) {
	FILE * pFichierInscriptions;
	fopen_s(&pFichierInscriptions, "Inscriptions.dat", "rb");
	if (pFichierInscriptions == NULL) {
		return true;
	}
	fclose(pFichierInscriptions);
	return false;
}

Erreur initialiserShifts(Shift * * pDebShifts) {
	Erreur codeErreur;
	*pDebShifts = NULL;
	if (premiereFois()) {
		codeErreur = chargementDatesOrgShifts(pDebShifts);
	} else {
		codeErreur = chargementInscriptions(pDebShifts);
	}
	return codeErreur;
}

Erreur chargementDatesOrgShifts(Shift * * pDebShifts) {
	FILE * pDatesOrgShifts;
	Shift * pShiftSauve;
	Shift * pShiftNouv;
	int date;
	int heure;
	fopen_s(&pDatesOrgShifts, "DatesOrgShifts.dat", "r");
	if (pDatesOrgShifts == NULL) {
		return ERREUR_OUVERTURE_FICHIER;
	}
	fscanf_s(pDatesOrgShifts, "%d %d", &date, &heure);
	while (!feof(pDatesOrgShifts)) {
		Erreur codeErreur = nouveauShift(&pShiftNouv);
		if (codeErreur != PAS_D_ERREUR) {
			fclose(pDatesOrgShifts);
			return codeErreur;
		}
		ajouterShift(pDebShifts, date, heure, pShiftNouv, &pShiftSauve);
		fscanf_s(pDatesOrgShifts, "%d %d", &date, &heure);
	}
	fclose(pDatesOrgShifts);
	return PAS_D_ERREUR;
}

Erreur chargementInscriptions(Shift * * pDebShifts) {
	Erreur codeErreur;
	FILE * pShiftsFi;
	Shift * pShiftSauve;
	Shift * pShiftNouv;
	ShiftFi shiftLu;
	fopen_s(&pShiftsFi, "Inscriptions.dat", "rb");
	if (pShiftsFi == NULL) {
		return ERREUR_OUVERTURE_FICHIER;
	}
	fread_s(&shiftLu, sizeof(ShiftFi), sizeof(ShiftFi), 1, pShiftsFi);
	while (!feof(pShiftsFi)) {
		codeErreur = nouveauShift(&pShiftNouv);
		if (codeErreur != PAS_D_ERREUR) {
			fclose(pShiftsFi);
			return codeErreur;
		}
		ajouterShift(pDebShifts, shiftLu.date, shiftLu.heure, pShiftNouv, &pShiftSauve);
		
		if (shiftLu.nbDoublettes != 0) {
			DoubletteFi doubletteLu;
			Doublette * pDoubletteNouv;
			int nbDoublettes = 1;
			
			
			while (nbDoublettes <= shiftLu.nbDoublettes) {
				codeErreur = nouvelleDoublette(&pDoubletteNouv);
				if (codeErreur != PAS_D_ERREUR) {
					fclose(pShiftsFi);
					return codeErreur;
				}
				fread_s(&doubletteLu, sizeof(DoubletteFi), sizeof(DoubletteFi), 1, pShiftsFi);
				ajouterDoubletteShift(pShiftNouv, doubletteLu.matriculeUn, doubletteLu.matriculeDeux, doubletteLu.categorie, pDoubletteNouv);
				nbDoublettes++;		
			}
			pShiftNouv->nbDoublettes = shiftLu.nbDoublettes;
		}
		fread_s(&shiftLu, sizeof(ShiftFi), sizeof(ShiftFi), 1, pShiftsFi);
	}
	fclose(pShiftsFi);
	return PAS_D_ERREUR;
}

Erreur sauverShifts(Shift * pDebShifts) {
	FILE * pShiftFi;
	ShiftFi shift;
	Shift * pShift = pDebShifts;
	fopen_s(&pShiftFi, "Inscriptions.dat", "wb");
	if (pShiftFi == NULL) {
		return ERREUR_OUVERTURE_FICHIER;
	}
	while (pShift != NULL) {
		DoubletteFi inscription;
		Doublette * pDoublette; 
		shift.date = pShift->date;
		shift.heure = pShift->heure;
		shift.nbDoublettes = pShift->nbDoublettes;
		fwrite(&shift, sizeof(ShiftFi), 1, pShiftFi);
		pDoublette = pShift->pDebDoublettes;
		while (pDoublette != NULL) {
			inscription.matriculeUn = pDoublette->matriculeUn;
			inscription.matriculeDeux = pDoublette->matriculeDeux;
			inscription.categorie = pDoublette->categorie;
			fwrite(&inscription, sizeof(DoubletteFi), 1, pShiftFi);
			pDoublette = pDoublette->pSuiv;
		}
		pShift = pShift->pSuiv;
	}
	fclose(pShiftFi);
	return PAS_D_ERREUR;
}