#ifndef WORLD_H
#define WORLD_H

#include "entity.h"

// ----------------------------------------- class: Scene -----------------------------------------
class Scene
{
public:

    Scene();
    std::vector<EntityLight*> lights;
    //std::vector<EntityMesh*> objects; aixo es per fer sa llista des objectes

};
// ----------------------------------------- class: World -----------------------------------------
class World
{
public:
    Camera* camera = NULL; //our global camera

    //----------------------crear cubo--------------------------
    Prueva* cubo = NULL;
    //EntityLight* lusesita = nullptr;

    World();
    World( int window_width, int window_height );

    void setCamera( int window_width, int window_height );
};



#endif 