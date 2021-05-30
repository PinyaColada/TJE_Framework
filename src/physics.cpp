#include "physics.h"

Physics::Physics(){
}

void Physics::updateModel(double elapsed_time, Matrix44* model)
{
    // Hacemos caer la matriz :)
    Vector3 pos = model->getTranslation();

    vel.y -= g * elapsed_time;
    pos = pos + (vel * elapsed_time);

    pos.y = clamp(pos.y, min_y, 10000);

    //reseteo de la velcidad
    if(pos.y <= min_y)
        vel.y = 0;

    model->setTranslation(pos);
}

Vector3 Physics::updateMove(double elapsed_time, Vector3 pos, bool isFalling)
{
    if (isFalling && pos.y > min_y){
        vel.y -= g * elapsed_time;
        pos = pos + (vel * elapsed_time);

        pos.y = clamp(pos.y, min_y, 10000);

    } else {
        vel.y = 0;
    }

    return pos;
}
