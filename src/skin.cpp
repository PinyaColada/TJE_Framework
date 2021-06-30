#include "skin.h"

// llista de objectes amb str
Object2str TableObj2str[SIZEOFOBJ] = {
    {"PLAYER", PLAYER},
    {"BOX", BOX},
    {"SAWHUNTER", SAWHUNTER},
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
    {"HUB",HUB,"data/Levels/Hub.txt"},
    {"HIKE1",HIKE1,"data/Levels/BranchHike_1.txt"},
    {"HIKE2",HIKE2,"data/Levels/BranchHike_2.txt"},
    {"JUMP1",JUMP1,"data/Levels/BranchJump_1.txt"},
    {"JUMP2",JUMP2,"data/Levels/BranchJump_2.txt"},
    {"SAW1",SAW1,"data/Levels/BranchSaw_1.txt"},
    {"SAW2",SAW2,"data/Levels/BranchSaw_2.txt"},
    {"WIN",WIN,""}
};

bool isTimeStopped = false;
bool toBlur = true;
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
        case SAWHUNTER:
            return true;
        
        default:
            return false;
    }
}

bool hasSaw(eObjectName type)
{
    switch (type)
    {
        case SAW:
        case SAWHUNTER:
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
                    "data/Blocks/Saw.png",
                    "data/Blocks/Saw.obj",
                    "");
    cfgMeshCreat(   cfgTableMesh[SAWHUNTER],
                    "data/Blocks/GoldenSaw.png",
                    "data/Blocks/Saw.obj",
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
        case HUB: 
            return Vector4(0,1,0,1);    // Verd
        case HIKE1: 
        case HIKE2: 
            return Vector4(1,0,1,1);  // Lila
        case JUMP1:
        case JUMP2: 
            return Vector4(1,0,0,1);  // vermell
        case SAW1:
        case SAW2: 
            return Vector4(1,1,1,1);  // blanc
        case WIN: 
            return Vector4(1,1,0,1);    // groc

        default: 
            return Vector4(0.5,0.5,0.5,1); // gris
    }
}