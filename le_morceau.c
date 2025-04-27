#include "le_morceau.h"
#include "la_mémoire.h"

void InitialiséLeMorceau(LeMorceau* leMorceau)
{
    leMorceau->laTaille = 0;
    leMorceau->laCapacité = 0;
    leMorceau->leCode = SIZE_T_NULL;
    
    leMorceau->laTailleDesLignes = 0;
    leMorceau->laCapacitéDesLignes = 0;
    leMorceau->lesLignes = SIZE_T_NULL;
    
    InitialiséLeTableauDeValeurs(&leMorceau->lesConstantes);
}

void ÉcritLaLigne(LeMorceau* leMorceau, uint32_t laLigne)
{
    bool besoinDePlusDesLignes = leMorceau->laTailleDesLignes == 0
        || Pointeur(LesInformationsDesLignes, leMorceau->lesLignes)[leMorceau->laTailleDesLignes - 1].laLigne != laLigne;
    if (besoinDePlusDesLignes)
    {
        leMorceau->laTailleDesLignes++;
        
        if (leMorceau->laTailleDesLignes > leMorceau->laCapacitéDesLignes)
        {
            const uint32_t vieilleCapacitéDesLignes = leMorceau->laCapacitéDesLignes;
            leMorceau->laCapacitéDesLignes = AugmenterLaCapacité(vieilleCapacitéDesLignes);
            leMorceau->lesLignes = AugmenterLeTableau(LesInformationsDesLignes, leMorceau->lesLignes,
                                                      vieilleCapacitéDesLignes, leMorceau->laCapacitéDesLignes);            
        }

        LesInformationsDesLignes* laInformationDeligne = &Pointeur(LesInformationsDesLignes, leMorceau->lesLignes)[leMorceau->laTailleDesLignes - 1];
        laInformationDeligne->laLigne = laLigne;
        laInformationDeligne->lesRépétitions = 0;    
    }

    Pointeur(LesInformationsDesLignes, leMorceau->lesLignes)[leMorceau->laTailleDesLignes - 1].lesRépétitions++;
}

void ÉcritUneOperation(LeMorceau* leMorceau, uint8_t laOperation)
{
    leMorceau->laTaille++;
    bool besoinDePlusDeCode = leMorceau->laTaille > leMorceau->laCapacité;
    
    if (besoinDePlusDeCode)
    {
        const uint32_t vieilleCapacité = leMorceau->laCapacité;
        leMorceau->laCapacité = AugmenterLaCapacité(vieilleCapacité);
        leMorceau->leCode = AugmenterLeTableau(uint8_t, leMorceau->leCode,
                                               vieilleCapacité, leMorceau->laCapacité);
    }

    Pointeur(uint8_t, leMorceau->leCode)[leMorceau->laTaille - 1] = laOperation;
}

void ÉcritLeMorceauUnaire(LeMorceau* leMorceau, uint8_t laOperation, uint32_t laLigne)
{
    ÉcritUneOperation(leMorceau, laOperation);
    
    ÉcritLaLigne(leMorceau, laLigne);
}

uint8_t TransformerLaOperationEnLongue(uint8_t laInstruction)
{
    switch (laInstruction)
    {
        case LaConstanteOp:
            return LaConstanteLongueOp;
        default:
            wprintf(L"Une Instruction inconnue!");
            return LaConstanteLongueOp;
    }
}

void ÉcritLeMorceauBinaire(LeMorceau* leMorceau, uint8_t laInstruction, uint32_t leByte, uint32_t laLigne)
{
    if (leByte <= 255)
    {
        ÉcritLeMorceauUnaire(leMorceau, laInstruction, laLigne);
        ÉcritLeMorceauUnaire(leMorceau, leByte, laLigne);
    }
    else if (leByte < (1 << 24))
    {
        ÉcritLeMorceauUnaire(leMorceau, TransformerLaOperationEnLongue(laInstruction), laLigne);
        
        uint8_t laPremièreValeur = leByte & 0xFF;
        ÉcritLeMorceauUnaire(leMorceau, laPremièreValeur, laLigne);
        uint8_t laDeuxièmeValeur = leByte >> 8 & 0xFF;
        ÉcritLeMorceauUnaire(leMorceau, laDeuxièmeValeur, laLigne);
        uint8_t laTroisièmeValeur = leByte >> 16 & 0xFF;
        ÉcritLeMorceauUnaire(leMorceau, laTroisièmeValeur, laLigne);
    }
    else
    {
        wprintf(L"La Ligne %u: Le Byte %u est trop long", laLigne, leByte);
    }
}

uint32_t AjoutéLaConstante(LeMorceau* leMorceau, LaValeur laValeur)
{
    ÉcritLeTableauDeValeurs(&leMorceau->lesConstantes, laValeur);
    return leMorceau->lesConstantes.laTaille - 1;
}

void LibéréLeMorceau(LeMorceau* leMorceau)
{
    LibérerLeTableau(uint8_t, leMorceau->leCode, leMorceau->laCapacité);
    LibérerLeTableau(LesInformationsDesLignes, leMorceau->lesLignes, leMorceau->laCapacitéDesLignes);
    LibéréLeTableauDeValeurs(&leMorceau->lesConstantes);
    InitialiséLeMorceau(leMorceau);
}
