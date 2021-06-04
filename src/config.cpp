#include "config.h"
#include <iostream>
#include <string.h>

// crear la llista
cfgGeneric* cfgTable[SIZEOFCFG];
strNameCfg namesCfg[SIZEOFCFG] = {
    {"physicsBox",physicsBox},
    {"physicsPlayer",physicsPlayer},
};

// ----- class: cfgPhysics -----
cfgPhysics::cfgPhysics(eType name)
{
    type = name;

    // valors per defecte
    g = 1000;
    g_jump = 1000;
    v_jump = 500;
}

void cfgPhysics::ReadCfg(FILE* f)
{
        // per cada linea
    char line[100];
    char * pch;

    while (feof(f) == 0)
    {
        fgets(line, 100, f);

        // g
        if((pch = strstr(line, "g:")) != NULL)
        {
            g = atof(pch+2);
        }
        // g_jump
        else if((pch = strstr(line, "g_jump:")) != NULL)
        {
            g_jump = atof(pch+7);
        }
        // v_jump
        else if((pch = strstr(line, "v_jump:")) != NULL)
        {
            v_jump = atof(pch+7);
        }

        // tag fin
        else if(strstr(line, ">>>") != NULL) 
            break;

        else if (strlen(line) > 1)
            std::cout << "Line error: \"" << line << "\""<< std::endl;
    }
}

// funcions que es criden desde fora
void InitCfg()
{
    std::cout << "--- Create data ---" << std::endl;
    for (int i = 0; i < SIZEOFCFG; i++)
    {
        eType name = (eType) i;

        // crea cada element en default
        switch (name)
        {
            case physicsBox:
            case physicsPlayer:
            {
                cfgPhysics* cfgP = new cfgPhysics(name);
                cfgTable[name] = cfgP;
                break;
            }
            default:
                std::cout << "**Error**" << std::endl;
                break;
        }
    }
}

cfgGeneric* getCfg(eType type)
{
    return cfgTable[type];
}

void LoadCfg(const char* filename)
{
    std::cout << "--- Load data ---" << std::endl;

    //obrir fitxer
    FILE* f = fopen(filename, "r");

    // comprova si es correcte
    if (f == NULL)
    {
        std::cout << "Error: File not found" << std::endl;
        return;
    }

    // per cada linea
    char line[100];
    char * pch;
    eType type;

    while (feof(f) == 0)
    {
        fgets(line, 100, f);

        // buscar "<<<"
        if((pch = strstr(line, "<<<")) == NULL) 
            continue;

        // buscar "type:"
        if((pch = strstr(pch+3, "type:")) == NULL) 
            continue;

        // buscar la type de la linea
        pch += 5;
        type = DEFAULT;

        for (int i = 0; type == DEFAULT && i < SIZEOFCFG; i++)
        {
            // si es el element
            if(strstr(pch, namesCfg[i].name) != NULL) 
                type = namesCfg[i].type;
        }

        // si no a trobat el element
        if(type == DEFAULT)
            continue;

        // carrega els parametres del objecte
        cfgTable[type]->ReadCfg(f);
    }

    // tancar fitxer
    fclose(f);
}
