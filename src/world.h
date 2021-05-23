#ifndef WORLD_H
#define WORLD_H

#include "entity.h"

enum eScene{
    DEMO,
    NIVELDELAVA
};

// ----------------------------------------- class: Scene -----------------------------------------
class Scene
{
public:

    Scene();
    std::vector<EntityLight*> lights;
    std::vector<Object*> dinamic_objects;
    std::vector<Object*> static_objects;

};
// ----------------------------------------- class: World -----------------------------------------
class World
{
public:
    // Atributos
    Camera* camera = NULL; //our global camera

    std::vector<Scene*> scenes;
    std::vector<EntityMesh*> meshs;

    int window_width;
    int window_height;
    eScene current_scene;

    Player* player;

    // Metodos
    World( int window_width, int window_height );

    EntityMesh* searchMesh( eEntityName obj );

    void setCamera( int window_width, int window_height );
    void SelectBox();
    void LeaveBox();
};



#endif 