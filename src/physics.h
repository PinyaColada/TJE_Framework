#ifndef PHYSICS_H
#define PHYSICS_H

#include "framework.h"

#define g 1000

class Physics
{
public:
    // Atributs
    Vector3 vel = Vector3(0, 500, 0);

    // Metodes
    Physics();

    void updateModel(double elapsed_time, Matrix44* model);
    // bool isColiding();
};

#endif 