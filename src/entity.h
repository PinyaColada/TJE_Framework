#ifndef ENTITY_H
#define ENTITY_H

#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "camera.h"
#include "physics.h"
#include "skin.h"

// ----------------------------------------- class: Entity -----------------------------------------
class Entity
{
public:
    // Atributos
    eEntityName eName; 
    Matrix44 model;

    // Metodos
    Entity();
  
    virtual void render(Camera* camera) = 0;
    virtual void update(float elapsed_time) = 0;

    Vector3 getPosition(); 
    Vector3 getDir(); 
};

// ----------------------------------------- class: EntityLight -------------------------------	
class EntityLight : public Entity {
public:
    float intensity;

    Vector3 color;

    int area_size;

    EntityLight(Vector3 pos, Vector3 target, Vector3 color, float intensity);

    virtual void render(Camera* camera) { return; }
    virtual void update(float elapsed_time) { return; }
};

// ----------------------------------------- class: EntityMesh -------------------------------------
class EntityMesh : public Entity
{
public:
    // Atributos
    Mesh* mesh = NULL;
    Texture* texture = NULL;
    Shader* shader = NULL;
    Vector4 color;
    eObjectName object; 

    // Metodos
    EntityMesh(eObjectName obj, cfgMesh* cfgM = NULL);
 
    void render(Camera* camera, std::vector<EntityLight*> lights);
    void render(Camera* camera);
    void update(float dt);
};

// ----------------------------------------- class: Object -------------------------------------
class Object : public Entity
{
 public:
    // Atributos
    eObjectName oName;

    EntityMesh* mesh = NULL;

    int idList;

    // Metodos
    Object(){};
    Object(EntityMesh* m) { mesh = m; };

    void render(Camera* camera, std::vector<EntityLight*> lights);
    void render(Camera* camera);
    void update(float dt){};
};

// ----------------------------------------- class: Floor -------------------------------
class Floor : public Object
{
public:
    // Metodos
    Floor();
};

// ----------------------------------------- class: Block -------------------------------
class Block : public Object
{
public:
    // Atributos

    // Metodos
    Block(){};
    Block(EntityMesh* m, Vector3 pos, eObjectName type);

    void Init(EntityMesh* m, Vector3 pos, eObjectName type);
};

// ----------------------------------------- class: Jewel -------------------------------
class Jewel : public Block
{
public:
    // Atributos
    eScene next_scene;

    JewelMask idMask;

    // Metodos
    Jewel(EntityMesh* m, Vector3 pos, eScene ns = DEFAULTSCENE);

    void render(Camera* camera, std::vector<EntityLight*> lights);
    void render(Camera* camera);
};

// ----------------------------------------- class: DinamicObject -------------------------------------
class DinamicObject : public Object
{
 public:
    // Atributos
    Physics* physic;

    bool isCatch = false;
    bool isFalling;

    cfgDinamic* cfgD;

    Vector3 spawn;

    // Metodos
    DinamicObject(){};

    void setCfgD(eType type);

    virtual void move(Vector3 dir, float elapsed_time, std::vector<Object*> static_objects, std::vector<DinamicObject*> dinamic_objects) = 0;

    virtual void movePicked(Matrix44 player, std::vector<Object*> static_objects, std::vector<DinamicObject*> dinamic_objects) = 0;

    bool onCollision(Object* object, Vector3 position, float speed, Vector3& target);
    bool hasGround(Object* object, Vector3 position);
    float minimHeight(Object* object, Vector3 position, float lastMin);

    void respawn();
};

// ----------------------------------------- class: Box -------------------------------
class Box : public DinamicObject
{
public:
    // Atributos
    cfgBox* cfgB;

    // Metodos
    Box(EntityMesh* m,  Vector3 pos = Vector3(0,100,0));

    void move(Vector3 dir, float elapsed_time, std::vector<Object*> static_objects, std::vector<DinamicObject*> dinamic_objects);

    void movePicked(Matrix44 player, std::vector<Object*> static_objects, std::vector<DinamicObject*> dinamic_objects);
};

// ----------------------------------------- class: Player -------------------------------
class Player : public DinamicObject
{
public:
    // Atributos
    cfgPlayer* cfgP;
    float Speed;

    DinamicObject* boxPicked = NULL;

    bool isDead = false;
    eScene current_scene = DEFAULTSCENE;

    JewelMask pickedJewel;
    
    // Metodos
    Player(); 

    void move(Vector3 dir, float elapsed_time, std::vector<Object*> static_objects, std::vector<DinamicObject*> dinamic_objects);

    void movePicked(Matrix44 player, std::vector<Object*> static_objects, std::vector<DinamicObject*> dinamic_objects){};

    void SelectBox(DinamicObject* picked);
    void LeaveBox();
};
#endif 