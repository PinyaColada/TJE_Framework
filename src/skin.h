#ifndef SKIN_H
#define SKIN_H

#include "framework.h"

// ---------- carregar Meshs ----------
enum eCfgMesh {
    eFloor,
    eBLargeMesh,
    eBLongMesh,
    eBUnitMesh,
    eBoxMesh,

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
    eBLarge,
    eBLong,
    eBUnit,
    eBox
};

struct StaticObj
{
    eObjType type;

    Vector3 pos;
    Vector3 dir;
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

    StaticObj sObjs[100];
    DinamicObj dObjs[100];
};

// funcions que es criden desde fora
extern void InitCfgMesh();
extern cfgMesh* getCfgMesh(eCfgMesh name);

#endif 