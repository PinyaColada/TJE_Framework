#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>

enum eType {
    physicsBox,
    physicsPlayer,

    SIZEOFCFG,
    DEFAULT = -1
};

struct strNameCfg{
    const char* name;
    eType type;
};

// ----- class: cfgGeneric -----
class cfgGeneric 
{
public:
    // Atributs
    eType type; 

    // Metodes
    virtual void ReadCfg(FILE* f) = 0;
};

// ----- class: cfgPhysics -----
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

// funcions que es criden desde fora
extern void InitCfg();
extern cfgGeneric* getCfg(eType type);
extern void LoadCfg(const char* filename);

#endif 