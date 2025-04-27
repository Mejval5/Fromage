#ifndef fromage_le_morceau_h
#define fromage_le_morceau_h

#include "fourre_tout.h"
#include "la_valeur.h"

typedef enum
{
  LaConstanteOp,
  LaConstanteLongueOp,
  LeRetourOp,
} CodeOp;

typedef struct
{
  uint32_t laLigne;
  uint32_t lesRépétitions;
} LesInformationsDesLignes;

typedef struct
{
  uint32_t laTaille;
  uint32_t laCapacité;
  size_t leCode;
  
  uint32_t laTailleDesLignes;
  uint32_t laCapacitéDesLignes;
  size_t lesLignes;
  
  LeTableauDeValeurs lesConstantes;
} LeMorceau;

void InitialiséLeMorceau(LeMorceau* leMorceau);
void ÉcritLeMorceauUnaire(LeMorceau* leMorceau, uint8_t laOperation, uint32_t laLigne);
void ÉcritLeMorceauBinaire(LeMorceau* leMorceau, uint8_t laInstruction, uint32_t leByte, uint32_t laLigne);
uint32_t AjoutéLaConstante(LeMorceau* leMorceau, LaValeur laValeur);
void LibéréLeMorceau(LeMorceau* leMorceau);

#endif