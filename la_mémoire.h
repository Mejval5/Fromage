#ifndef fromage_la_mémoire_h
#define fromage_la_mémoire_h

#include "fourre_tout.h"

#define UneDemandeDeTailleDePool 4 * 1024 * 1024

#define ALIGNMENT 8
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))
#define SIZE_T_NULL ((size_t) -1)

typedef struct
{
    size_t lePointeurDeDébut;
    size_t lePointeurDuBlocPrécédent;
    size_t lePointeurDuBlocSuivant;
    size_t lePointeurDesDébutDeDonnées;
    size_t laTailleDesDonnées;
} LeBlocAlloué;

#define AugmenterLaCapacité(capacité) \
((capacité) < 8 ? 8 : (capacité) * 2)

#define AugmenterLeTableau(genre, lePointeur, vieilleCapacité, nouvelleCapacité) \
Réaffecter(lePointeur, sizeof(genre) * (vieilleCapacité), \
sizeof(genre) * (nouvelleCapacité))

#define LibérerLeTableau(genre, lePointeur, vieilleCapacité) \
Réaffecter(lePointeur, sizeof(genre) * vieilleCapacité, 0)

size_t Réaffecter(size_t lePointeur, size_t vieilleCapacité, size_t nouvelleCapacité);
void* ValeurDuPointeur(size_t décalage);

#define Pointeur(genre, décalage) \
    ((genre*)ValeurDuPointeur(décalage))

#endif
