#include "physics.h"

Physics::Physics(){
}

void Physics::updateModel(double elapsed_time, Matrix44* model)
{
    // Hacemos caer la matriz :)
    Vector3 pos = model->getTranslation();
    Vector3 acc(0, -g, 0);

    vel = vel + (acc * elapsed_time);
    pos = pos + (vel * elapsed_time);

    pos.y = clamp(pos.y, 0, 10000);

    model->setTranslation(pos);
}