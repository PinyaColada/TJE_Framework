#include "config.h"
#include <iostream>

struct pPhysics PhysicsBox = { physicsBox };
struct pPhysics PhysicsPlayer = { physicsPlayer };


void* cfgTable[SIZEOFLIST]={
    &PhysicsBox,
    &PhysicsPlayer
};

void* getCfg(eType type)
{
    return cfgTable[type];
}

void LoadCfg()
{
    std::cout << "--- Load data ---" << std::endl;
    pPhysics* PhysicsBox = (pPhysics*) cfgTable[physicsBox];
    {
        PhysicsBox->g = 1000;
        PhysicsBox->g_jump = 1000;
        PhysicsBox->v_jump = 500;
    }
    pPhysics* PhysicsPlayer = (pPhysics*) cfgTable[physicsPlayer];
    {
        PhysicsPlayer->g = 1000;
        PhysicsPlayer->g_jump = 4000;
        PhysicsPlayer->v_jump = 600;
    }
}