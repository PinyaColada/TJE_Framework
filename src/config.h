#ifndef CONFIG_H
#define CONFIG_H

#include "framework.h"
#include <stdio.h>

enum eType {
    dinamicsBox,
    dinamicsPlayer,
    player,
    physicsBox,
    physicsPlayer,

    SIZEOFCFG,
    DEFAULT = -1
};

struct strNameCfg{
    const char* name;
    eType type;
};

// ---------- class: cfgGeneric ----------
class cfgGeneric 
{
public:
    // Atributs
    eType type; 

    // Metodes
    virtual void ReadCfg(FILE* f) = 0;
};

// ---------- class: cfgPhysics ----------
class cfgPhysics : public cfgGeneric 
{
public:
    // Atributs
    float g;
    float g_jump;
    float v_jump;

    // Metodes
    cfgPhysics(eType name = DEFAULT);

    void ReadCfg(FILE* f);
};

// ---------- class: cfgDinamic ----------
class cfgDinamic : public cfgGeneric 
{
public:
    // Atributs
    float radius = 3;
    float margen = 0.1;
    float dead_y = -200;

    // Metodes
    cfgDinamic(eType name = DEFAULT);

    void ReadCfg(FILE* f);
};

// ---------- class: cfgPlayer ----------
class cfgPlayer : public cfgGeneric 
{
public:
    // Atributs
    float maxSpeed;
    float minSpeed;
    Vector3 altura;

    // Metodes
    cfgPlayer(eType name = DEFAULT);

    void ReadCfg(FILE* f);
};

// funcions que es criden desde fora
extern void InitCfg();
extern cfgGeneric* getCfg(eType type);
extern void LoadCfg(const char* filename);

#endif 