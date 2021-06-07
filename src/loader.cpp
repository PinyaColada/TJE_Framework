#include "loader.h"
#include "framework.h"
#include <string.h>

strObjType nameType[SIZEOFOT] = {
    {"ePlayer",ePlayer},
    {"eFloor",eFloor},
    {"eBLarge",eBLarge},
    {"eBLong",eBLong},
    {"eBUnit",eBUnit},
    {"eBox",eBox},
    {"eJewel",eJewel},
    {"eMushroom",eMushroom},
    {"eRock",eRock},
    {"eWeed",eWeed}
};

// funcions que es criden desde fora
void SaveLevel(Level* level)
{
    FILE* f = fopen("data/Levels/NewLevel.txt","w");

    // comprova si es correcte
    if (f == NULL)
    {
        std::cout << "Error: File not found" << std::endl;
        return;
    }

    // nom del nivell
    fprintf(f,"### %s\n\n", level->name);

    // Skybox
    if (level->Skybox.name == eSkybox)
    {
        fprintf(f,"<<< %s\n", "eSkybox");
        fprintf(f,"tex: %s\n", level->Skybox.texture);
        fprintf(f,">>>\n\n");
    }
    else
        std::cout << "Skybox not found" << std::endl;

    // Player
    if (level->player.type == ePlayer)
    {
        fprintf(f,"<<< %s\n", "ePlayer");
        fprintf(f,"p: "); level->player.pos.exportVector(f);
        fprintf(f,">>>\n\n");
    }
    else
        std::cout << "Player not found" << std::endl;

    // llista Static
    fprintf(f,"<<< %s\n", "LStatics");

    StaticObj sobj;
    for(int i = 0; i < level->numSObj; i++)
    {
        sobj = level->sObjs[i];
        if(sobj.type == eFloor)
            continue;

        fprintf(f,"--- %s\n", nameType[sobj.type].name);
        fprintf(f,"p: "); sobj.pos.exportVector(f);
        fprintf(f,"r: "); sobj.rot.exportVector(f);
    }

    fprintf(f,">>>\n\n");

    // llista dinamics
    fprintf(f,"<<< %s\n", "LDinamics");

    DinamicObj dobj;
    for(int i = 0; i < level->numDObj; i++)
    {
        dobj = level->dObjs[i];
        if(dobj.type == eFloor)
            continue;
        
        fprintf(f,"--- %s\n", nameType[dobj.type].name);
        fprintf(f,"p: "); dobj.pos.exportVector(f);
    }

    fprintf(f,">>>\n\n");

    // tancar fitxer
    fclose(f);
    std::cout << " + Level Saved" << std::endl;
}

char name[40];
char tex[40];

Level* LoadLevel(const char* filename)
{
    FILE* f = fopen(filename, "r");

    // comprova si es correcte
    if (f == NULL)
    {
        std::cout << "Error: File not found" << std::endl;
        return NULL;
    } 

    // per cada linea
    char line[100];
    char* pch;
    char* pch1;
    Level* level = new Level;
    
    // nom del nivell
    fgets(line, 100, f);
    if((pch = strstr(line, "###")) != NULL) 
    {
        sscanf(pch+3, " %s", name);
        std::cout << " + Level Name: \"" << name << "\"" << std::endl;
        level->name = name;
    }
    else
    {
        std::cout << "Level Name: not found" << std::endl;
        level->name = "DEFAULT";
    }
    
    // variables del while
    float x,y,z;
    eObjType type;
    StaticObj* SObj;
    DinamicObj* DObj;
    while (feof(f) == 0)
    {
        fgets(line, 100, f);

        // buscar "<<<"
        if((pch = strstr(line, "<<<")) == NULL) 
            continue;

        // Skybox
        if((pch1 = strstr(pch+3, "eSkybox")) != NULL)
        {
            while (feof(f) == 0)
            {
                fgets(line, 100, f);
                
                //textura
                if((pch = strstr(line, "tex:")) != NULL)
                {
                    sscanf(pch+4, " %s", tex);

                    cfgMesh* cfgSB = cfgSkyboxCreat(tex);
                    level->Skybox = *cfgSB;
                }
                // tag de fi
                else if((pch = strstr(line, ">>>")) != NULL)
                    break;
                else if (strlen(line) > 1)
                    std::cout << "Line error: \"" << line << "\""<< std::endl;
            }
        }
        // Player
        else if((pch1 = strstr(pch+3, "ePlayer")) != NULL)
        {
            while (feof(f) == 0)
            {
                fgets(line, 100, f);

                // posicio
                if((pch = strstr(line, "p:")) != NULL)
                {
                    x = 0; y = 0; z = 0;
                    sscanf(pch+2, " (%f,%f,%f)", &x,&y,&z);

                    DinamicObj* sPlayer = new DinamicObj{ePlayer, Vector3(x,y,z)};
                    level->player = *sPlayer;
                }
                // tag de fi
                else if((pch = strstr(line, ">>>")) != NULL) 
                    break;
                else if (strlen(line) > 1)
                    std::cout << "Line error: \"" << line << "\""<< std::endl;
            }
        }
        // llista Static
        else if((pch1 = strstr(pch+3, "LStatics")) != NULL)
        {
            int id = 0;
            level->numSObj = id;
            while (feof(f) == 0)
            {
                fgets(line, 100, f);

                // buscar la tag "---" + el type del objecte
                if((pch = strstr(line, "---")) != NULL && id < MAXOBJ)
                {
                    // guardar la struct
                    SObj = &(level->sObjs[id]);

                    // buscar la type de la linea
                    pch += 3;
                    type = eFloor;

                    for (int i = 0; type == eFloor && i < SIZEOFOT; i++)
                    {
                        // si es el element
                        if(strstr(pch, nameType[i].name) != NULL) 
                            type = nameType[i].type;
                    }

                    if (type == eFloor)
                        continue;
                    
                    SObj->type = type;
                    SObj->pos = Vector3();
                    SObj->rot = Vector3();
                    id++;
                    level->numSObj = id;
                }
                // posicio
                else if((pch = strstr(line, "p:")) != NULL)
                {
                    sscanf(pch+2, " (%f,%f,%f)", &x,&y,&z);

                    SObj->pos = Vector3(x,y,z);
                }
                // rotacio
                else if((pch = strstr(line, "r:")) != NULL)
                {
                    sscanf(pch+2, " (%f,%f,%f)", &x,&y,&z);

                    SObj->rot = Vector3(x,y,z);
                }
                // tag de fi
                else if((pch = strstr(line, ">>>")) != NULL) 
                    break;
                else if (strlen(line) > 1)
                    std::cout << "Line error: \"" << line << "\""<< std::endl;
            }
        }
        // llista dinamics
        else if((pch1 = strstr(pch+3, "LDinamics")) != NULL)
        {
            int id = 0;
            level->numDObj = id;
            while (feof(f) == 0)
            {
                fgets(line, 100, f);

                // buscar la tag "---" + el type del objecte
                if((pch = strstr(line, "---")) != NULL && id < MAXOBJ)
                {
                    // guardar la struct
                    DObj = &(level->dObjs[id]);

                    // buscar la type de la linea
                    pch += 3;
                    type = eFloor;

                    for (int i = 0; type == eFloor && i < SIZEOFOT; i++)
                    {
                        // si es el element
                        if(strstr(pch, nameType[i].name) != NULL) 
                            type = nameType[i].type;
                    }

                    if (type == eFloor)
                        continue;
                    
                    DObj->type = type;
                    DObj->pos = Vector3();
                    id++;
                    level->numDObj = id;
                }
                // posicio
                else if((pch = strstr(line, "p:")) != NULL)
                {
                    sscanf(pch+2, " (%f,%f,%f)", &x,&y,&z);

                    DObj->pos = Vector3(x,y,z);
                }
                // tag de fi
                else if((pch = strstr(line, ">>>")) != NULL) 
                    break;
                else if (strlen(line) > 1)
                    std::cout << "Line error: \"" << line << "\""<< std::endl;
            }
        }
        else if (strlen(line) > 1)
            std::cout << "Line error: \"" << line << "\""<< std::endl;
    }
    // tancar fitxer
    fclose(f);
    return level;
}