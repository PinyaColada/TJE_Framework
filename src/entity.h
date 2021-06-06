#ifndef ENTITY_H
#define ENTITY_H

#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "camera.h"
#include "physics.h"
#include "skin.h"

enum eEntityName {
    ENTITY,
    MESH,
    LIGHT,
    PLAYER,
    BOX,
    FLOOR,
    SKYBOX,
    BLOCKLARGE,
    BLOCKLONG,
    BLOCKUNIT
};

enum eBlocktype {
    BLARGE,
    BLONG,
    BUNIT,

    SIZEOFBLOCK
};

struct block2enums
{
    eEntityName entity;
    eObjType type;
};

struct block2enumsM
{
    eEntityName entity;
    eCfgMesh mesh;
};

// ----------------------------------------- class: Entity -----------------------------------------
class Entity
{
public:
    // Atributos
    eEntityName name; 
    Matrix44 model;

    // Metodos
    Entity(); //constructor
  
    virtual void render(Camera* camera) = 0;
    virtual void update(float elapsed_time) = 0;

    Vector3 getPosition(); 
    Vector3 getDir(); 
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
    eEntityName object; 

    // Metodos
    EntityMesh(eEntityName obj, cfgMesh* cfgM = NULL);
 
    void render(Camera* camera);
    void update(float dt);
};

// ----------------------------------------- class: Object -------------------------------------
class Object : public Entity
{
 public:
    // Atributos
    EntityMesh* mesh = NULL;

    int idList;

    // Metodos
    Object(){}; //constructor
    Object(EntityMesh* m) { mesh = m; };

    void render(Camera* camera);
    void update(float dt){};
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
    // float radius = 3;
    // float margen = 0.1;
    // float dead_y = -200;

    Vector3 spawn;

    // Metodos
    DinamicObject(){}; //constructor

    void setCfgD(eType type);

    virtual void move(Vector3 dir, float elapsed_time, std::vector<Object*> static_objects, std::vector<DinamicObject*> dinamic_objects) = 0;

    virtual void movePicked(Matrix44 player) = 0;

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
    Vector3 playerPos;

    // Metodos
    Box(EntityMesh* m,  Vector3 pos = Vector3(0,100,0)); //constructor

    void move(Vector3 dir, float elapsed_time, std::vector<Object*> static_objects, std::vector<DinamicObject*> dinamic_objects);

    void movePicked(Matrix44 player);
};

// ----------------------------------------- class: Floor -------------------------------
class Floor : public Object
{
public:
    // Atributos

    // Metodos
    Floor(); //constructor
};

// ----------------------------------------- class: Block -------------------------------

class Block : public Object
{
public:
    // Atributos
    eBlocktype type;
    // Metodos
    Block(EntityMesh* m, Vector3 pos, eBlocktype type);

};

// ----------------------------------------- class: Player -------------------------------
class Player : public DinamicObject
{
public:
    // Atributos
    cfgPlayer* cfgP;
    float Speed;

    // Vector3 altura = Vector3(0, 70, 0);

    DinamicObject* boxPicked = NULL;
    
    // Metodos
    Player(); 

    void move(Vector3 dir, float elapsed_time, std::vector<Object*> static_objects, std::vector<DinamicObject*> dinamic_objects);

    void movePicked(Matrix44 player){};

    void SelectBox(DinamicObject* picked);
    void LeaveBox();
};

// ----------------------------------------- class: EntityLight -------------------------------
class EntityLight {};

#endif 