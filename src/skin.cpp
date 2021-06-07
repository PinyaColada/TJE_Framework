#include "skin.h"

// ---------- carregar Meshs ----------
cfgMesh* cfgTableMesh[SIZEOFMESHS];

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
    for (int i = 0; i < SIZEOFMESHS; i++)
    {
        eCfgMesh name = (eCfgMesh) i;
        // crea cada element en default
        cfgMesh* cfg = new cfgMesh{name};
        cfgTableMesh[name] = cfg;
    }

    // Poner la info
    cfgMeshCreat(   cfgTableMesh[eFloor],   // elemento 
                    "data/Floor/Floor.png", // texture
                    "data/Floor/Floor.obj", // mesh
                    "");                    // bounding

    cfgMeshCreat(   cfgTableMesh[eBoxMesh], 
                    "data/Box/MetalBox.png",
                    "data/Box/MetalBox.obj", 
                    "data/Box/ColissionBox.obj");                    

    cfgMeshCreat(   cfgTableMesh[eBLargeMesh], 
                    "data/Blocks/Large.png",
                    "data/Blocks/Large.obj", 
                    "");      

    cfgMeshCreat(   cfgTableMesh[eBLongMesh], 
                    "data/Blocks/Long.png",
                    "data/Blocks/Long.obj", 
                    "");      

    cfgMeshCreat(   cfgTableMesh[eBUnitMesh], 
                    "data/Blocks/Unit.png",
                    "data/Blocks/Unit.obj", 
                    "");

    cfgMeshCreat(   cfgTableMesh[eJewel],
                    "data/Blocks/Jewel.png",
                    "data/Blocks/Jewel.obj",
                    "");

    cfgMeshCreat(   cfgTableMesh[eMushroom],
                    "data/Blocks/Mushroom.png",
                    "data/Blocks/Mushroom.obj",
                    "");

    cfgMeshCreat(   cfgTableMesh[eRock],
                    "data/Blocks/Rock.png",
                    "data/Blocks/Rock.obj",
                    "");

    cfgMeshCreat(   cfgTableMesh[eWeed],
                    "data/Blocks/Weed.png",
                    "data/Blocks/Weed.obj",
                    "");
}

cfgMesh* getCfgMesh(eCfgMesh name)
{
    return cfgTableMesh[name];
}

cfgMesh* cfgSkyboxCreat(const char* texture)
{
    // crea cada element en default
    cfgMesh* cfg = new cfgMesh{eSkybox};

    // Poner la info
    cfgMeshCreat(   cfg,                        // elemento 
                    texture,                    // texture
                    "data/Skybox/sphere.obj",   // mesh
                    "");                        // bounding

    return cfg;
}