#include "loader.h"
#include "framework.h"

strObjType nameType[SIZEOFOT] = {
    {"ePlayer",ePlayer},
    {"efloor",efloor},
    {"eBLarge",eBLarge},
    {"eBLong",eBLong},
    {"eBUnit",eBUnit},
    {"eBox",eBox}
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
        if(sobj.type == efloor)
            continue;

        fprintf(f,"--- %s\n", nameType[sobj.type].name);
        fprintf(f,"p: "); sobj.pos.exportVector(f);
        fprintf(f,"h: "); sobj.rot.exportVector(f);
    }

    fprintf(f,">>>\n\n");

    // llista dinamics
    fprintf(f,"<<< %s\n", "LDtatics");

    DinamicObj dobj;
    for(int i = 0; i < level->numDObj; i++)
    {
        dobj = level->dObjs[i];
        if(dobj.type == efloor)
            continue;
        
        fprintf(f,"--- %s\n", nameType[dobj.type].name);
        fprintf(f,"p: "); dobj.pos.exportVector(f);
    }

    fprintf(f,">>>\n\n");

    // tancar fitxer
    fclose(f);
    std::cout << "Level Saved" << std::endl;
}
