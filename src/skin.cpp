#include "skin.h"

// llista de objectes amb str
Object2str TableObj2str[SIZEOFOBJ] = {
    {"PLAYER", PLAYER},
    {"BOX", BOX},
    {"SAW", SAW},
    {"FLOOR", FLOOR},
    {"SKYBOX", SKYBOX},
    {"BLOCKLARGE", BLOCKLARGE},
    {"BLOCKLONG", BLOCKLONG},
    {"BLOCKUNIT", BLOCKUNIT},
    {"JEWEL", JEWEL},
    {"MUSHROOM", MUSHROOM},
    {"ROCK", ROCK},
    {"WEED", WEED}
};

// llista de Scenes amb str
NameLevel TableSceneNames[SIZEOFSCENE] = {
    {"DEMO",DEMO,""},
    {"NIVELDELAVA",NIVELDELAVA,"data/Levels/Prueva.txt"},
    {"NEW",NEW,"data/Levels/NewLevel.txt"},
    {"WIN",WIN,""}
};

bool isTimeStopped = false;
int numJewels = 0;

bool hasBlock(eObjectName type)
{
    switch (type)
    {
        case BLOCKLARGE:
        case BLOCKLONG:
        case BLOCKUNIT:
        case JEWEL:
        case MUSHROOM:
        case ROCK:
        case WEED:
            return true;
        
        default:
            std::cout << "Error: " << TableObj2str[type].name << " has not Static" << std::endl;
            return false;
    }
}

bool hasCollition(eObjectName type)
{
    switch (type)
    {
        case MUSHROOM:
        case ROCK:
        case WEED:
            return false;
        
        default:
            return true;
    }
}

bool hasDinamic(eObjectName type)
{
    switch (type)
    {
        case BOX:
        case SAW:
            return true;
        
        default:
            return false;
    }
}

// ---------- carregar Meshs ----------
cfgMesh* cfgTableMesh[SIZEOFOBJ];

void cfgMeshCreat(  cfgMesh* cfg,
                    const char* texture,
	                const char* mesh,
                    const char* bounding,
                    const char* vsf = "data/shaders/basic.vs",
                    const char* psf = "data/shaders/texture.fs",
                    Vector4 color = Vector4(1,1,1,1)    )
{
    cfg->texture=texture;
    cfg->mesh=mesh;
    cfg->bounding=bounding;
    cfg->vsf=vsf;
    cfg->psf=psf;
    cfg->color=color;
}

// funcions que es criden desde fora
void InitCfgMesh()
{
    std::cout << " * Load Mesh Info" << std::endl;
    for (int i = 0; i < SIZEOFOBJ; i++)
    {
        eObjectName name = (eObjectName) i;
        // crea cada element en default
        cfgMesh* cfg = new cfgMesh{name};
        cfgTableMesh[name] = cfg;
    }

    // Poner la info
    cfgMeshCreat(   cfgTableMesh[FLOOR],   // elemento 
                    "data/Floor/Floor.png", // texture
                    "data/Floor/Floor.obj", // mesh
                    "");                    // bounding

    cfgMeshCreat(   cfgTableMesh[BOX], 
                    "data/Box/MetalBox.png",
                    "data/Box/MetalBox.obj", 
                    "data/Box/ColissionBox.obj");
                    
    cfgMeshCreat(   cfgTableMesh[SAW],
                    "data/Blocks/saw.png",
                    "data/Blocks/saw.obj",
                    "");
    cfgMeshCreat(   cfgTableMesh[BLOCKLARGE], 
                    "data/Blocks/Large.png",
                    "data/Blocks/Large.obj", 
                    "");      

    cfgMeshCreat(   cfgTableMesh[BLOCKLONG], 
                    "data/Blocks/Long.png",
                    "data/Blocks/Long.obj", 
                    "");      

    cfgMeshCreat(   cfgTableMesh[BLOCKUNIT], 
                    "data/Blocks/Unit.png",
                    "data/Blocks/Unit.obj", 
                    "");

    cfgMeshCreat(   cfgTableMesh[JEWEL],
                    "data/Blocks/Jewel.png",
                    "data/Blocks/Jewel.obj",
                    "");

    cfgMeshCreat(   cfgTableMesh[MUSHROOM],
                    "data/Blocks/Mushroom.png",
                    "data/Blocks/Mushroom.obj",
                    "");

    cfgMeshCreat(   cfgTableMesh[ROCK],
                    "data/Blocks/Rock.png",
                    "data/Blocks/Rock.obj",
                    "");

    cfgMeshCreat(   cfgTableMesh[WEED],
                    "data/Blocks/Weed.png",
                    "data/Blocks/Weed.obj",
                    "");
}

cfgMesh* getCfgMesh(eObjectName name)
{
    return cfgTableMesh[name];
}

cfgMesh* cfgSkyboxCreat(const char* texture)
{
    // crea cada element en default
    cfgMesh* cfg = new cfgMesh{SKYBOX};

    // Poner la info
    cfgMeshCreat(   cfg,                        // elemento 
                    texture,                    // texture
                    "data/Skybox/sphere.obj",   // mesh
                    "");                        // bounding

    return cfg;
}

Vector4 colorJewel(eScene level)
{
    switch(level){
        case DEMO: return Vector4(0,0,1,1);         // blau
        case NIVELDELAVA: return Vector4(1,0,0,1);  // vermell
        case WIN: return Vector4(1,1,0,1);          // groc

        default: return Vector4(0.5,0.5,0.5,1);     // gris
    }
}