#ifndef fromage_le_débogage_h
#define fromage_le_débogage_h

#include "le_morceau.h"

void DisassembléLeMorceau(const LeMorceau* leMorceau, const wchar_t* leNom);
uint32_t DisassembléLaInstruction(const LeMorceau* leMorceau, uint32_t leDécalage);
static uint32_t LaInstructionSimple(const wchar_t* leNom, uint32_t leDécalage);
static uint32_t LaInstructionConstante(const wchar_t* leNom, const uint8_t laInstruction, const LeMorceau* leMorceau, uint32_t leDécalage);
#endif
