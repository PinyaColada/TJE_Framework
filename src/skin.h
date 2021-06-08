#ifndef SKIN_H
#define SKIN_H

#include "framework.h"

#define MAXOBJ 100

enum eEntityName {
    ENTITY,
    MESH,
    LIGHT,
    OBJECT
};

enum eObjectName {
    PLAYER,
    BOX,
    FLOOR,
    SKYBOX,
    BLOCKLARGE,
    BLOCKLONG,
    BLOCKUNIT,
    JEWEL,
    MUSHROOM,
    ROCK,
    WEED,

    SIZEOFOBJ
};

struct Object2str {
    const char* name;
    eObjectName type;
};

extern Object2str TableObj2str[SIZEOFOBJ];
extern bool hasBlock(eObjectName type);

// ---------- carregar Meshs ----------
struct cfgMesh
{
    eObjectName name;

    const char* texture;
	const char* mesh;
    const char* bounding;
    const char* vsf;
    const char* psf;
    Vector4 color;
};

// ---------- carregar nivell ----------

struct StaticObj
{
    eObjectName type;

    Vector3 pos;
    Vector3 rot;
};

struct DinamicObj
{
    eObjectName type;

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
extern cfgMesh* getCfgMesh(eObjectName name);
extern cfgMesh* cfgSkyboxCreat(const char* texture);

#endif 