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
    // Atributos
    std::vector<EntityLight*> lights;
    std::vector<DinamicObject*> dinamic_objects;
    std::vector<Object*> static_objects;

    EntityMesh* skybox = NULL;
    Vector3 spawn;

    // Metodos
    Scene(Vector3 sp = Vector3(0,120,0), bool sb = false);

    void getSkybox(const char* fileSkybox, const char* fileSkyboxTex);
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
    DinamicObject* boxPicked = NULL;
    Object* BlockPicked = NULL;

    // Metodos
    World( int window_width, int window_height );

    EntityMesh* searchMesh( eEntityName obj );

    void setCamera( int window_width, int window_height );
    void SelectBox();
    void editMap();

    // guardar i carregar Scenes
    Level* SaveScene();
    void LoadScene(Level* level);
};



#endif 