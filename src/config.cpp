#include "config.h"
#include <iostream>
#include <string.h>

// crear la llista
cfgGeneric* cfgTable[SIZEOFCFG];
strNameCfg namesCfg[SIZEOFCFG] = {
    {"dinamicsBox",dinamicsBox},
    {"dinamicsPlayer",dinamicsPlayer},
    {"player",player},
    {"box",box},
    {"saw",saw},
    {"physicsBox",physicsBox},
    {"physicsPlayer",physicsPlayer},
};

// guarda un float
bool setFloat(float &var, char* line, const char* par, int dim)
{
    char* pch;
    if((pch = strstr(line, par)) != NULL)
    {
        var = atof(pch + dim);
        return true;
    }
    else
        return false;
}

// guarda un Vector3
bool setVector3(Vector3 &var, char* line, const char* par, int dim)
{
    char* pch;
    if((pch = strstr(line, par)) != NULL)
    {
        // per agafar el valors separats per ","
        char* token = strtok(pch + dim, ",");
        for (int i = 0; i < 3 && token != NULL; i++)
        {
            var[i] = atof(token);
            token = strtok(NULL, ",");
        }
        return true;
    }
    else
        return false;
}

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

    while (feof(f) == 0)
    {
        fgets(line, 100, f);

        // set parametres
        if (setFloat( g, line, "g:", 2)) {}
        else if (setFloat( g_jump, line, "g_jump:", 7)) {}
        else if (setFloat( v_jump, line, "v_jump:", 7)) {}
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

    while (feof(f) == 0)
    {
        fgets(line, 100, f);

        // set parametres
        if (setFloat( radius, line, "radius:", 7)) {}
        else if (setFloat( margen, line, "margen:", 7)) {}
        else if (setFloat( dead_y, line, "dead_y:", 7)) {}
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

    while (feof(f) == 0)
    {
        fgets(line, 100, f);

        // set parametres
        if (setFloat( maxSpeed, line, "maxSpeed:", 9)) {}
        else if (setFloat( minSpeed, line, "minSpeed:", 9)) {}
        else if (setVector3( altura, line, "altura:", 7)) {}
        // tag fin
        else if(strstr(line, ">>>") != NULL) 
            break;
        else if (strlen(line) > 1)
            std::cout << "Line error: \"" << line << "\""<< std::endl;
    }
}

// ---------- class: cfgBox ----------
cfgBox::cfgBox(eType name)
{
    type = name;

    // valors per defecte
    max_h = 100;
    min_h = 0;
    distPicked = 50;
}

void cfgBox::ReadCfg(FILE* f)
{
        // per cada linea
    char line[100];

    while (feof(f) == 0)
    {
        fgets(line, 100, f);

        // set parametres
        if (setFloat( max_h, line, "max_h:", 6)) {}
        else if (setFloat( min_h, line, "min_h:", 6)) {}
        else if (setFloat( distPicked, line, "distPicked:", 11)) {}
        // tag fin
        else if(strstr(line, ">>>") != NULL) 
            break;
        else if (strlen(line) > 1)
            std::cout << "Line error: \"" << line << "\""<< std::endl;
    }
}

// ---------- class: cfgSaw ----------
cfgSaw::cfgSaw(eType name)
{
    type = name;

    // valors per defecte
    vr = 0.1;
}

void cfgSaw::ReadCfg(FILE* f)
{
        // per cada linea
    char line[100];

    while (feof(f) == 0)
    {
        fgets(line, 100, f);

        // set parametres
        if (setFloat( vr, line, "vr:", 3)) {}
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
            case box:
            {
                cfgBox* cfg = new cfgBox(name);
                cfgTable[name] = cfg;
                break;
            }
            case saw:
            {
                cfgSaw* cfg = new cfgSaw(name);
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
