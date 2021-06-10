#ifndef SKIN_H
#define SKIN_H

#include "framework.h"

#define MAXOBJ 100

// enum de Entity
enum eEntityName {
    ENTITY,
    MESH,
    LIGHT,
    OBJECT
};

// enum de Objectes
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

// enum de Scene
enum eScene{
    DEMO,
    NIVELDELAVA,

    SIZEOFSCENE,
    DEFAULTSCENE = -1
};

// --- Strucks ---
struct Object2str {
    const char* name;
    eObjectName type;
};
struct NameLevel{
    const char* cName;
    eScene eName;
};

// --- externs ---
extern Object2str TableObj2str[SIZEOFOBJ];
extern NameLevel TableSceneNames[SIZEOFSCENE];

extern bool hasBlock(eObjectName type);
extern bool hasCollition(eObjectName type);
extern bool hasDinamic(eObjectName type);
extern bool isTimeStopped;

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
    eScene scene;
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