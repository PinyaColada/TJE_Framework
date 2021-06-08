#ifndef LOADER_H
#define LOADER_H

#include "skin.h"

struct strObjType{
    const char* name;
    eObjectName type;
};

// funcions que es criden desde fora
extern void SaveLevel(Level* level);
extern Level* LoadLevel(const char* filename);

#endif 