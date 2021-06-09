#include "skin.h"

Object2str TableObj2str[SIZEOFOBJ] = {
    {"PLAYER", PLAYER},
    {"BOX", BOX},
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

// ---------- carregar Meshs ----------
cfgMesh* cfgTableMesh[SIZEOFOBJ];

void cfgMeshCreat(   cfgMesh* cfg,
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