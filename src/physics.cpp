#include "physics.h"

Physics::Physics(){
}

void Physics::updateModel(double elapsed_time, Matrix44* model)
{
    // Hacemos caer la matriz :)
    Vector3 pos = model->getTranslation();

    vel.y += (-g * elapsed_time);

    pos = pos + (vel * elapsed_time);

    model->setTranslation(pos);
}