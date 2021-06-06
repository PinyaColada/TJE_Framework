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
    std::cout << "--- Load Mesh Info ---" << std::endl;
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
}

cfgMesh* getCfgMesh(eCfgMesh name)
{
    return cfgTableMesh[name];
}
