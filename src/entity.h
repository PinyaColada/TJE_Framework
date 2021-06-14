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
    Block(EntityMesh* m, Vector3 pos, eObjectName type, Vector3 front = Vector3(0, 0, 1));

    void Init(EntityMesh* m, Vector3 pos, eObjectName type, Vector3 front);
};

// ----------------------------------------- class: Jewel -------------------------------
class Jewel : public Block
{
public:
    // Atributos
    eScene next_scene;

    JewelMask idMask;

    // Metodos
    Jewel(EntityMesh* m, Vector3 pos, eScene ns = DEFAULTSCENE, Vector3 front = Vector3(0, 0, 1));

    void render(Camera* camera, std::vector<EntityLight*> lights);
    void render(Camera* camera);
};

// ----------------------------------------- class: DinamicObject -------------------------------------
class DinamicObject : public Object
{
 public:
    // Atributos
    Physics* physic = NULL;

    bool isCatch = false;
    bool isFalling;

    cfgDinamic* cfgD;

    Vector3 spawn;

    // Metodos
    DinamicObject(){};

    void setCfgD(eType type);

    virtual void move(float elapsed_time, Vector3 dir, std::vector<Object*> static_objects, std::vector<DinamicObject*> dinamic_objects) = 0;

    virtual bool movePicked(Matrix44 player, std::vector<Object*> static_objects, std::vector<DinamicObject*> dinamic_objects) = 0;

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

    void move(float elapsed_time, Vector3 dir, std::vector<Object*> static_objects, std::vector<DinamicObject*> dinamic_objects);

    bool movePicked(Matrix44 player, std::vector<Object*> static_objects, std::vector<DinamicObject*> dinamic_objects);
};

// ----------------------------------------- class: Saw -------------------------------
class Saw : public DinamicObject
{
public:
    // Atributos
    Vector3 center;

    int direction;

    cfgSaw* cfgS;

    float rad;
    float speed;

    Matrix44 model_position;

    // Metodos
    //Saw(EntityMesh* m, Vector3 pos, Vector3 front = Vector3(0, 0, 1), float dis = 100, float vel = 100);
    void Init(EntityMesh* m, Vector3 pos, Vector3 front, float dis, float vel, eObjectName type);

    virtual void move(float elapsed_time, Vector3 dir, std::vector<Object*> static_objects, std::vector<DinamicObject*> dinamic_objects) = 0;

    bool movePicked(Matrix44 player, std::vector<Object*> static_objects, std::vector<DinamicObject*> dinamic_objects) { return false; };
    void renderLimits(Camera* camera);
};

// ----------------------------------------- class: SawBasic -------------------------------
class SawBasic : public Saw
{
public:
    // Atributos

    // Metodos
    SawBasic(EntityMesh* m, Vector3 pos, Vector3 front = Vector3(0, 0, 1), float dis = 100, float vel = 100);

    void move(float elapsed_time, Vector3 dir, std::vector<Object*> static_objects, std::vector<DinamicObject*> dinamic_objects);
};

// ----------------------------------------- class: SawHunter -------------------------------
class SawHunter : public Saw
{
public:
    // Atributos

    // Metodos
    SawHunter(EntityMesh* m, Vector3 pos, Vector3 front = Vector3(0, 0, 1), float dis = 100, float vel = 100);

    void move(float elapsed_time, Vector3 playerPos, std::vector<Object*> static_objects, std::vector<DinamicObject*> dinamic_objects);
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

    void move(float elapsed_time, Vector3 dir, std::vector<Object*> static_objects, std::vector<DinamicObject*> dinamic_objects);

    bool movePicked(Matrix44 player, std::vector<Object*> static_objects, std::vector<DinamicObject*> dinamic_objects) { return false; };

    void SelectBox(DinamicObject* picked);
    void LeaveBox();
};
#endif 