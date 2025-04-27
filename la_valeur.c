
#include "la_valeur.h"
#include "la_mémoire.h"

void InitialiséLeTableauDeValeurs(LeTableauDeValeurs* leTableau)
{
    leTableau->lesValeurs = SIZE_T_NULL;
    leTableau->laCapacité = 0;
    leTableau->laTaille = 0;
}

void ÉcritLeTableauDeValeurs(LeTableauDeValeurs* leTableau, LaValeur laValeur)
{
    if (leTableau->laCapacité < leTableau->laTaille + 1)
    {
        uint32_t vieilleCapacité = leTableau->laCapacité;
        leTableau->laCapacité = AugmenterLaCapacité(vieilleCapacité);
        leTableau->lesValeurs = AugmenterLeTableau(LaValeur, leTableau->lesValeurs,
            vieilleCapacité, leTableau->laCapacité);
    }

    Pointeur(LaValeur, leTableau->lesValeurs)[leTableau->laTaille] = laValeur;
    leTableau->laTaille++;
}

void LibéréLeTableauDeValeurs(LeTableauDeValeurs* leTableau)
{
    LibérerLeTableau(LaValeur, leTableau->lesValeurs, leTableau->laCapacité);
    InitialiséLeTableauDeValeurs(leTableau);
}

void AffichéLaValeur(LaValeur laValeur)
{
    wprintf(L"%g", laValeur);
}