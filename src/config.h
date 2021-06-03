#ifndef CONFIG_H
#define CONFIG_H

enum eType {
    physicsBox,
    physicsPlayer,

    SIZEOFLIST,
    DEFAULT = -1
};

struct pGeneric {
    eType type; 
};

struct pPhysics {
    eType type; 

    float g;
    float g_jump;
    float v_jump;  
};

struct pPlayer {
    eType type; 

    float g;


};

extern void* getCfg(eType type);
extern void LoadCfg();

#endif 