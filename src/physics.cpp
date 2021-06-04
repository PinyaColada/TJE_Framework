#include "physics.h"

Physics::Physics(eType type)
{
    // Busca la configuracio
    cfgGeneric* cfgP = getCfg(type);

    if (cfgP != NULL && ((cfgGeneric*)cfgP)->type == type)
        cfg = (cfgPhysics*) cfgP;
    else
        cfg = new cfgPhysics();
}

void Physics::updateModel(double elapsed_time, Matrix44* model)
{
    // Hacemos caer la matriz :)
    Vector3 pos = model->getTranslation();

    vel.y -= cfg->g * elapsed_time;
    pos = pos + (vel * elapsed_time);

    pos.y = clamp(pos.y, min_y, 10000);

    // reseteo de la velcidad
    if(pos.y <= min_y)
        vel.y = 0;

    model->setTranslation(pos);
}

Vector3 Physics::updateMove(double elapsed_time, Vector3 pos, bool isFalling)
{
    if (isJump || isFalling){
        if (vel.y > 0){
            vel.y -= cfg->g_jump * elapsed_time;
        }
        else {
            vel.y -= cfg->g * elapsed_time;
            if(!isFalling){
                vel.y = 0;
                isJump = false;
            }
        }
        pos = pos + vel * elapsed_time;
        pos.y = clamp(pos.y, min_y, 10000);
    } 
    else {
        vel.y = 0;
    }
    return pos;
}

void Physics::Jump()
{
    isJump = true;
	vel.y = cfg->v_jump;
}