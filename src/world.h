#ifndef WORLD_H
#define WORLD_H

#include "entity.h"

// ----------------------------------------- class: Scene -----------------------------------------
class Scene
{
public:
    // Atributos
    eScene name;

    std::vector<EntityLight*> lights;
    std::vector<DinamicObject*> dinamic_objects;
    std::vector<Object*> static_objects;

    EntityMesh* skybox = NULL;
    Vector3 spawn;

    // Metodos
    Scene(eScene n, bool hasInfo = false, Vector3 sp = Vector3(0,120,0));

    void getSkybox(const char* fileSkybox, const char* fileSkyboxTex);

    void defaultScene();
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

    JewelMask JewelsWin = 0;

    // Metodos
    World( int window_width, int window_height );

    EntityMesh* searchMesh( eObjectName obj );

    void setCamera( int window_width, int window_height );
    void SelectBox();
    void editMap();

    void changeScene(eScene nextScene);
    void respawn();
    bool hasWin();

    // guardar i carregar Scenes
    Level* SaveScene();
    void LoadScene(Level* level, eScene nameScene = DEFAULTSCENE);
};



#endif 