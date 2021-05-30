#ifndef PHYSICS_H
#define PHYSICS_H

#include "framework.h"

#define g 1000

class Physics
{
public:
    // Atributs
    Vector3 vel = Vector3(0, 0, 0);
    float min_y = -1000;

    // Metodes
    Physics();

    void updateModel(double elapsed_time, Matrix44* model);

    Vector3 updateMove(double elapsed_time, Vector3 pos, bool isFalling = true);
    // bool isColiding();
};

#endif 