#ifndef fromage_la_valeur_h
#define fromage_la_valeur_h

#include <stdint.h>
#include "la_mémoire.h"

typedef double LaValeur;

typedef struct
{
    uint32_t laTaille;
    uint32_t laCapacité;
    size_t lesValeurs;
} LeTableauDeValeurs;


void InitialiséLeTableauDeValeurs(LeTableauDeValeurs* leTableau);
void ÉcritLeTableauDeValeurs(LeTableauDeValeurs* leTableau, LaValeur laValeur);
void LibéréLeTableauDeValeurs(LeTableauDeValeurs* leTableau);
void AffichéLaValeur(LaValeur laValeur);

#endif