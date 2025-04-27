#include <stdlib.h>
#include "la_mémoire.h"

#include <complex.h>

static size_t LaCapacitéDuTas = 0;
static size_t LePointeurDuPremierBloc = SIZE_T_NULL;
static uint8_t* LePointeurDuTas = nullptr;

void CopierLaMémoire(const uint8_t* laSource, uint8_t* laDestination, const size_t laTaille)
{
    for (size_t i = 0; i < laTaille; ++i)
    {
        laDestination[i] = laSource[i];
    }
}

void RequêteAllocation(const size_t laCapacité)
{
    wprintf(L"Requête Allocation pour la capacité: %llu\n", laCapacité);

    const size_t laCapacitéMod = laCapacité % (UneDemandeDeTailleDePool);
    const size_t laCapacitéTaille = laCapacité / (size_t)(UneDemandeDeTailleDePool);
    size_t laCapacitéNouvelle = laCapacitéTaille * UneDemandeDeTailleDePool;
    if (laCapacitéMod != 0)
    {
        laCapacitéNouvelle += UneDemandeDeTailleDePool;
    }

    wprintf(L"La Capacité Nouvelle: %llu\n", laCapacitéNouvelle);
    uint8_t* viellePointeurDuTas = LePointeurDuTas;
    LePointeurDuTas = malloc(laCapacitéNouvelle);

    if (LePointeurDuTas == nullptr)
    {
        wprintf(L"Échec d'allocation de mémoire!");
        exit(1);
    }

    if (viellePointeurDuTas != nullptr && viellePointeurDuTas != LePointeurDuTas)
    {
        CopierLaMémoire(viellePointeurDuTas, LePointeurDuTas, LaCapacitéDuTas);
        free(viellePointeurDuTas);
    }

    LaCapacitéDuTas = laCapacitéNouvelle;
}

void AssuréeAllocation(size_t laCapacité)
{
    if (LaCapacitéDuTas >= laCapacité)
    {
        return;
    }

    RequêteAllocation(laCapacité);
}

uint8_t* Pointeuriser_uint_8(size_t décalage)
{
    if (décalage == SIZE_T_NULL)
    {
        return nullptr;
    }
    
    return LePointeurDuTas + décalage;
}

LeBlocAlloué* Pointeuriser(size_t décalage)
{
    if (décalage == SIZE_T_NULL)
    {
        return nullptr;
    }
    
    return (LeBlocAlloué*) (LePointeurDuTas + décalage);
}

void* ValeurDuPointeur(size_t décalageDeDonnées)
{
    if (décalageDeDonnées == SIZE_T_NULL)
    {
        return nullptr;
    }
    
    return LePointeurDuTas + décalageDeDonnées;
}

size_t DePointeuriser_uint_8(uint8_t* leBlocAlloué)
{
    if (leBlocAlloué == nullptr)
    {
        return SIZE_T_NULL;
    }
    
    return leBlocAlloué - LePointeurDuTas;
}

size_t DePointeuriser(LeBlocAlloué* leBlocAlloué)
{
    if (leBlocAlloué == nullptr)
    {
        return SIZE_T_NULL;
    }
    
    return (uint8_t*)leBlocAlloué - LePointeurDuTas;
}

void SuppriméUnBloc(LeBlocAlloué* leBlocAlloué)
{
    if (leBlocAlloué == nullptr)
    {
        return;
    }
    
    LeBlocAlloué* leBlocPrécédent = Pointeuriser(leBlocAlloué->lePointeurDuBlocPrécédent);
    LeBlocAlloué* leBlocSuivant = Pointeuriser(leBlocAlloué->lePointeurDuBlocSuivant);
    if (leBlocPrécédent != nullptr)
    {
        leBlocPrécédent->lePointeurDuBlocSuivant = leBlocAlloué->lePointeurDuBlocSuivant;
    }

    if (leBlocSuivant != nullptr)
    {
        leBlocSuivant->lePointeurDuBlocPrécédent = leBlocAlloué->lePointeurDuBlocPrécédent;
    }

    if (leBlocAlloué == Pointeuriser(LePointeurDuPremierBloc))
    {
        LePointeurDuPremierBloc = leBlocAlloué->lePointeurDuBlocSuivant;
    }
}

LeBlocAlloué* InitialiséLeBloc(LeBlocAlloué* leBloc)
{
    if (leBloc == nullptr)
    {
        wprintf(L"Null bloc ne peut pas être alloué!");
        exit(1);
    }

    leBloc->lePointeurDeDébut = SIZE_T_NULL;
    leBloc->lePointeurDuBlocPrécédent = SIZE_T_NULL;
    leBloc->lePointeurDuBlocSuivant = SIZE_T_NULL;
    leBloc->lePointeurDesDébutDeDonnées = SIZE_T_NULL;
    leBloc->laTailleDesDonnées = 0;

    return leBloc;
}

size_t InséréLeBloc(size_t laCapacité, size_t leEspaceNécessaire)
{
    size_t leBlocPrécédent = DePointeuriser(nullptr);
    size_t leBlocSuivant = LePointeurDuPremierBloc;

    size_t leBlocNouvellePointeur = DePointeuriser_uint_8(LePointeurDuTas);

    while (leBlocSuivant != SIZE_T_NULL)
    {
        const size_t leEspaceDisponible = leBlocSuivant - leBlocNouvellePointeur;

        if (leBlocSuivant > leBlocNouvellePointeur && leEspaceNécessaire < leEspaceDisponible)
        {
            LeBlocAlloué* leBlocNouvelle = Pointeuriser(leBlocNouvellePointeur);
            leBlocNouvelle = InitialiséLeBloc(leBlocNouvelle);
            leBlocNouvelle->laTailleDesDonnées = laCapacité;
            leBlocNouvelle->lePointeurDeDébut = DePointeuriser(leBlocNouvelle);
            leBlocNouvelle->lePointeurDuBlocPrécédent = leBlocPrécédent;
            leBlocNouvelle->lePointeurDuBlocSuivant = leBlocSuivant;
            leBlocNouvelle->lePointeurDesDébutDeDonnées = DePointeuriser(leBlocNouvelle) + ALIGN(sizeof(LeBlocAlloué));
            
            if (leBlocPrécédent == SIZE_T_NULL)
            {
                LePointeurDuPremierBloc = DePointeuriser(leBlocNouvelle);                
            }
            else
            {
                Pointeuriser(leBlocPrécédent)->lePointeurDuBlocSuivant = DePointeuriser(leBlocNouvelle);
            }

            if (leBlocSuivant != SIZE_T_NULL)
            {
                Pointeuriser(leBlocSuivant)->lePointeurDuBlocPrécédent = DePointeuriser(leBlocNouvelle);
            }

            return leBlocNouvellePointeur;
        }

        leBlocPrécédent = leBlocSuivant;
        LeBlocAlloué* leBlocPrécédent_t = Pointeuriser(leBlocPrécédent);
        leBlocNouvellePointeur = leBlocPrécédent_t->lePointeurDesDébutDeDonnées + ALIGN(leBlocPrécédent_t->laTailleDesDonnées);
        
        leBlocSuivant = leBlocPrécédent_t->lePointeurDuBlocSuivant;
    }
    
    AssuréeAllocation(leBlocNouvellePointeur + leEspaceNécessaire);
    LeBlocAlloué* leBlocNouvelle = InitialiséLeBloc(Pointeuriser(leBlocNouvellePointeur));
    leBlocNouvelle->lePointeurDeDébut = leBlocNouvellePointeur;
    leBlocNouvelle->lePointeurDuBlocSuivant = SIZE_T_NULL;
    leBlocNouvelle->lePointeurDesDébutDeDonnées = leBlocNouvellePointeur + ALIGN(sizeof(LeBlocAlloué));
    leBlocNouvelle->laTailleDesDonnées = laCapacité;
    
    if (leBlocPrécédent == SIZE_T_NULL)
    {
        LePointeurDuPremierBloc = leBlocNouvellePointeur;
    }
    else
    {
        leBlocNouvelle->lePointeurDuBlocPrécédent = leBlocPrécédent;
        Pointeuriser(leBlocNouvelle->lePointeurDuBlocPrécédent)->lePointeurDuBlocSuivant = leBlocNouvellePointeur;
    }

    return leBlocNouvellePointeur;
}

size_t CrééLaMémoire(const size_t laCapacité)
{
    const size_t leEspaceNécessaire = ALIGN(laCapacité) + ALIGN(sizeof(LeBlocAlloué));
    AssuréeAllocation(leEspaceNécessaire);

    size_t leBlocAlloué = InséréLeBloc(laCapacité, leEspaceNécessaire);

    return leBlocAlloué;
}

bool PeutRedimensionnerLaMémoire(LeBlocAlloué* leBlocAlloué, size_t nouvelleCapacité)
{
    size_t leBlocAllouéSuivant = leBlocAlloué->lePointeurDuBlocSuivant;
    size_t nouvellePointeurDeFin = leBlocAlloué->lePointeurDesDébutDeDonnées + ALIGN(nouvelleCapacité);

    if (leBlocAllouéSuivant == SIZE_T_NULL)
    {
        return nouvellePointeurDeFin <= LaCapacitéDuTas;
    }

    return nouvellePointeurDeFin <= leBlocAllouéSuivant;
}

LeBlocAlloué* RedimensionnerLaMémoire(LeBlocAlloué* leBlocAlloué, size_t nouvelleCapacité)
{
    leBlocAlloué->laTailleDesDonnées = nouvelleCapacité;
    return leBlocAlloué;
}

size_t Réaffecter(size_t lePointeur, size_t vieilleCapacité, const size_t nouvelleCapacité)
{
    if (nouvelleCapacité == 0)
    {
        if (lePointeur == SIZE_T_NULL)
        {
            return SIZE_T_NULL;
        }
        
        SuppriméUnBloc(Pointeuriser(lePointeur - ALIGN(sizeof(LeBlocAlloué))));
        return SIZE_T_NULL;
    }

    if (lePointeur == SIZE_T_NULL)
    {
        size_t nouvelleBlocAlloué = CrééLaMémoire(nouvelleCapacité);
        return Pointeuriser(nouvelleBlocAlloué)->lePointeurDesDébutDeDonnées;
    }

    size_t lePointeurDeVieuxBloc = lePointeur - ALIGN(sizeof(LeBlocAlloué));
    LeBlocAlloué* leBlocAlloué = Pointeuriser(lePointeurDeVieuxBloc);
    
    if (PeutRedimensionnerLaMémoire(leBlocAlloué, nouvelleCapacité))
    {
        return RedimensionnerLaMémoire(leBlocAlloué, nouvelleCapacité)->lePointeurDesDébutDeDonnées;
    }

    size_t nouvelleBlocAlloué = CrééLaMémoire(nouvelleCapacité);
    LeBlocAlloué* nouvelleBlocAllouéPointeur = Pointeuriser(nouvelleBlocAlloué);
    LeBlocAlloué* vielleBlocAllouéPointeur = Pointeuriser(lePointeurDeVieuxBloc);
    CopierLaMémoire(Pointeuriser_uint_8(vielleBlocAllouéPointeur->lePointeurDesDébutDeDonnées),
                    Pointeuriser_uint_8(nouvelleBlocAllouéPointeur->lePointeurDesDébutDeDonnées),
                    vielleBlocAllouéPointeur->laTailleDesDonnées);

    SuppriméUnBloc(vielleBlocAllouéPointeur);
    return Pointeuriser(nouvelleBlocAlloué)->lePointeurDesDébutDeDonnées;
}
