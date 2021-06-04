#ifndef PHYSICS_H
#define PHYSICS_H

#include "framework.h"
#include "config.h"

class Physics
{
public:
    // Atributs
    Vector3 vel = Vector3(0, 0, 0);
    cfgPhysics* cfg;
    float min_y = -1000;
    bool isJump = false;;

    // Metodes
    Physics(eType type);

    void updateModel(double elapsed_time, Matrix44* model);

    Vector3 updateMove(double elapsed_time, Vector3 pos, bool isFalling = true);

    void Jump();
};

#endif 