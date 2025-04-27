#include "le_débogage.h"
#include "la_valeur.h"

void DisassembléLeMorceau(const LeMorceau* leMorceau, const wchar_t* leNom)
{
    wprintf(L"== %ls ==\n", leNom);

    uint32_t leDécalage = 0;
    
    while (leDécalage < leMorceau->laTaille)
    {
        uint32_t vielleDécalage = leDécalage;
        leDécalage = DisassembléLaInstruction(leMorceau, leDécalage);
    }
}

uint32_t ObtenirLaLigneParDécalage(const LeMorceau* leMorceau, const uint32_t leDécalageCourte)
{
    uint32_t laTaille = 0;
    uint32_t leIndice = 0;
    uint32_t laLigne = 0;
    while (laTaille <= leDécalageCourte)
    {
        LesInformationsDesLignes laLigneInformation = Pointeur(LesInformationsDesLignes, leMorceau->lesLignes)[leIndice];
        leIndice++;
        laTaille += laLigneInformation.lesRépétitions;
        laLigne = laLigneInformation.laLigne;
    }
    return laLigne;
}

uint32_t DisassembléLaInstruction(const LeMorceau* leMorceau, uint32_t leDécalage)
{
    wprintf(L"%8u    - ", leDécalage);
    if (leDécalage > 0 && ObtenirLaLigneParDécalage(leMorceau, leDécalage) == ObtenirLaLigneParDécalage(leMorceau, leDécalage - 1))
    {
        wprintf(L"       |    ");
    }
    else
    {
        wprintf(L"%8u    ", ObtenirLaLigneParDécalage(leMorceau, leDécalage));
    }

    const uint8_t laInstruction = Pointeur(uint8_t, leMorceau->leCode)[leDécalage];
    leDécalage++;
    
    switch (laInstruction)
    {
        case LeRetourOp:
            return LaInstructionSimple(L"Une Opération de Retour", leDécalage);
        case LaConstanteOp:
        case LaConstanteLongueOp:
            return LaInstructionConstante(L"Une Opération de Constante", laInstruction, leMorceau, leDécalage);
        default:
            wprintf(L"[DisassembléLaInstruction] Une Instruction inconnue\n");
            return leDécalage;
    }
}

static uint32_t LaInstructionSimple(const wchar_t* leNom, uint32_t leDécalage)
{
    wprintf(L"%ls\n", leNom);
    return leDécalage;
}

bool EstLaInstructionLongue(const CodeOp laInstruction)
{
    switch (laInstruction)
    {
        case LaConstanteOp:
            return false;
        case LaConstanteLongueOp:
            return true;
        case LeRetourOp:
            return false;
        default:
            wprintf(L"[EstLaInstructionLongue] La Instruction inconnue\n");
            return false;
    }
}

static uint32_t LaInstructionConstante(const wchar_t* leNom, const uint8_t laInstruction, const LeMorceau* leMorceau, uint32_t leDécalage)
{
    uint32_t laConstante;
    if (EstLaInstructionLongue(laInstruction))
    {
        const uint8_t byte1 = Pointeur(uint8_t, leMorceau->leCode)[leDécalage];
        leDécalage++;
        const uint8_t byte2 = Pointeur(uint8_t, leMorceau->leCode)[leDécalage];
        leDécalage++;
        const uint8_t byte3 = Pointeur(uint8_t, leMorceau->leCode)[leDécalage];
        leDécalage++;

        laConstante = byte1 | (byte2 << 8) | (byte3 << 16);
    }
    else
    {
        laConstante = Pointeur(uint8_t, leMorceau->leCode)[leDécalage];
        leDécalage++;
    }
    
    wprintf(L"%-24ls %8u '", leNom, laConstante);
    LaValeur* laValeurPointeur = Pointeur(LaValeur, leMorceau->lesConstantes.lesValeurs);
    LaValeur laValeur = laValeurPointeur[laConstante];
    AffichéLaValeur(laValeur);
    
    wprintf(L"'\n");
    return leDécalage;
}
