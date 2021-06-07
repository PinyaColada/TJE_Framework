#ifndef SKIN_H
#define SKIN_H

#include "framework.h"

#define MAXOBJ 100

// ---------- carregar Meshs ----------
enum eCfgMesh {
    eFloor,
    eBLargeMesh,
    eBLongMesh,
    eBUnitMesh,
    eBoxMesh,
    eJewelMesh,
    eMushroomMesh,
    eRockMesh,
    eWeedMesh,

    SIZEOFMESHS,

    eSkybox
};

struct cfgMesh
{
    eCfgMesh name;

    const char* texture;
	const char* mesh;
    const char* bounding;
    const char* vsf;
    const char* psf;
    Vector4 color;
};

// ---------- carregar nivell ----------
enum eObjType {
    ePlayer,
    efloor,
    eBLarge,
    eBLong,
    eBUnit,
    eBox,
    eJewel,
    eMushroom,
    eRock,
    eWeed,

    SIZEOFOT
};

struct StaticObj
{
    eObjType type;

    Vector3 pos;
    Vector3 rot;
};

struct DinamicObj
{
    eObjType type;

    Vector3 pos;
};

struct Level
{
    const char* name;

    DinamicObj player;

    cfgMesh Skybox;

    int numSObj;
    int numDObj;

    StaticObj sObjs[MAXOBJ];
    DinamicObj dObjs[MAXOBJ];
};

// funcions que es criden desde fora
extern void InitCfgMesh();
extern cfgMesh* getCfgMesh(eCfgMesh name);
extern cfgMesh* cfgSkyboxCreat(const char* texture);

#endif 