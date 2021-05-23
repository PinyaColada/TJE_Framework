#ifndef PHYSICS_H
#define PHYSICS_H

#include "framework.h"

#define g 10

class Physics
{
public:
    // Atributs
    Vector3 vel;

    // Metodes
    Physics();
    
    void updateModel(double elapsed_time, Matrix44* model);
    // bool isColiding();
};

#endif 