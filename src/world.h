#ifndef WORLD_H
#define WORLD_H

#include "entity.h"

// ----------------------------------------- class: World -----------------------------------------
class World
{
public:
    Camera* camera = NULL; //our global camera

    //----------------------crear cubo--------------------------
    Prueva* cubo = NULL;

    World();
    World( int window_width, int window_height );

    void setCamera( int window_width, int window_height );
};

#endif 