#include <locale.h>
#include <fcntl.h>
#include <io.h>

#include "fourre_tout.h"
#include "le_débogage.h"
#include "le_morceau.h"
#include "stdlib.h"

int main(int argc, const char* argv[]) 
{
    _setmode(_fileno(stdout), _O_U16TEXT);
    
    LeMorceau leMorceau;
    InitialiséLeMorceau(&leMorceau);
    
    srand(0);

    uint32_t laLigne = 0;
    for (int i = 0; i < 52000 ; i++)
    {
        if (rand() % 5 == 0)
        {
            laLigne++;
        }

        const uint32_t laConstante = AjoutéLaConstante(&leMorceau, i + 0.1);
        ÉcritLeMorceauBinaire(&leMorceau, LaConstanteOp, laConstante, laLigne);
    }
    
    ÉcritLeMorceauUnaire(&leMorceau, LeRetourOp, laLigne + 1);
    DisassembléLeMorceau(&leMorceau, L"Le Morceau de test");
    LibéréLeMorceau(&leMorceau);

    return 0;
}
