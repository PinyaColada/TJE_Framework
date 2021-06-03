#ifndef PHYSICS_H
#define PHYSICS_H

#include "framework.h"

class Physics
{
public:
    // Atributs
    Vector3 vel = Vector3(0, 0, 0);
    float g;
    float g_jump;
    float min_y = -1000;
    bool isJump;

    // Metodes
    Physics(float gnew = 1000, float g_jumpnew = 0);

    void updateModel(double elapsed_time, Matrix44* model);

    Vector3 updateMove(double elapsed_time, Vector3 pos, bool isFalling = true);

    void Jump();
};

#endif 