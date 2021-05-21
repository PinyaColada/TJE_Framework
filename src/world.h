#ifndef WORLD_H
#define WORLD_H

#include "entity.h"

// ----------------------------------------- class: Scene -----------------------------------------
class Scene
{
public:

    Scene();
    std::vector<EntityLight*> lights;
    std::vector<Object*> objects;

};
// ----------------------------------------- class: World -----------------------------------------
class World
{
public:
    Camera* camera = NULL; //our global camera
    std::vector<Scene*> scenes;
    int window_width;
    int window_height;
    std::vector<EntityMesh*> meshs;
    Player* player;

    //EntityLight* lusesita = nullptr;

    World( int window_width, int window_height );

    void setCamera( int window_width, int window_height );

    EntityMesh* searchMesh( eEntityName obj );

    void SelectBox();
    void dejarBox();
};



#endif 