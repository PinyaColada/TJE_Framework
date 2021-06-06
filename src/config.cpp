#include "config.h"
#include <iostream>
#include <string.h>

// crear la llista
cfgGeneric* cfgTable[SIZEOFCFG];
strNameCfg namesCfg[SIZEOFCFG] = {
    {"dinamicsBox",dinamicsBox},
    {"dinamicsPlayer",dinamicsPlayer},
    {"player",player},
    {"physicsBox",physicsBox},
    {"physicsPlayer",physicsPlayer},
};

// ---------- class: cfgPhysics ----------
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
    char* pch;

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

// ---------- class: cfgDinamic ----------
cfgDinamic::cfgDinamic(eType name)
{
    type = name;

    // valors per defecte
    radius = 3;
    margen = 0.1;
    dead_y = -200;
}

void cfgDinamic::ReadCfg(FILE* f)
{
        // per cada linea
    char line[100];
    char* pch;

    while (feof(f) == 0)
    {
        fgets(line, 100, f);

        // radius
        if((pch = strstr(line, "radius:")) != NULL)
        {
            radius = atof(pch+7);
        }
        // margen
        else if((pch = strstr(line, "margen:")) != NULL)
        {
            margen = atof(pch+7);
        }
        // dead_y
        else if((pch = strstr(line, "dead_y:")) != NULL)
        {
            dead_y = atof(pch+7);
        }

        // tag fin
        else if(strstr(line, ">>>") != NULL) 
            break;

        else if (strlen(line) > 1)
            std::cout << "Line error: \"" << line << "\""<< std::endl;
    }
}

// ---------- class: cfgPlayer ----------
cfgPlayer::cfgPlayer(eType name)
{
    type = name;

    // valors per defecte
    maxSpeed = 130;
    minSpeed = 60;
    altura = Vector3(0, 70, 0);
}

void cfgPlayer::ReadCfg(FILE* f)
{
        // per cada linea
    char line[100];
    char* pch;

    while (feof(f) == 0)
    {
        fgets(line, 100, f);

        // maxSpeed
        if((pch = strstr(line, "maxSpeed:")) != NULL)
        {
            maxSpeed = atof(pch+9);
        }
        // minSpeed
        else if((pch = strstr(line, "minSpeed:")) != NULL)
        {
            minSpeed = atof(pch+9);
        }
        // altura
        else if((pch = strstr(line, "altura:")) != NULL)
        {
            // per agafar el valors separats per ","
            char* token = strtok(pch+7, ",");
            for (int i = 0; i < 3 && token != NULL; i++)
            {
                altura[i] = atof(token);
                token = strtok(NULL, ",");
            }
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
    std::cout << " * Create data" << std::endl;
    for (int i = 0; i < SIZEOFCFG; i++)
    {
        eType name = (eType) i;
        // crea cada element en default
        switch (name)
        {
            case dinamicsBox:
            case dinamicsPlayer:
            {
                cfgDinamic* cfg = new cfgDinamic(name);
                cfgTable[name] = cfg;
                break;
            }
            case player:
            {
                cfgPlayer* cfg = new cfgPlayer(name);
                cfgTable[name] = cfg;
                break;
            }
            case physicsBox:
            case physicsPlayer:
            {
                cfgPhysics* cfg = new cfgPhysics(name);
                cfgTable[name] = cfg;
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
    std::cout << " * Load data" << std::endl;

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
    char* pch;
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
