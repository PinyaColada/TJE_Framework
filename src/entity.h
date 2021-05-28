#ifndef ENTITY_H
#define ENTITY_H

#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "camera.h"
#include "physics.h"

enum eEntityName {
    ENTITY,
    MESH,
    LIGHT,
    PLAYER,
    BOX,
    FLOOR,
    SKYBOX
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
    EntityMesh( eEntityName obj ); //constructor
 
    void render(Camera* camera);
    void update(float dt);
};

// ----------------------------------------- class: Object -------------------------------------
class Object : public Entity
{
 public:
    // Atributos
    EntityMesh* mesh;

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

    // Metodos
    DinamicObject(){}; //constructor

    virtual void move(Vector3 dir, float speed, std::vector<Object*> static_objects, std::vector<DinamicObject*> dinamic_objects) = 0;

    bool onCollision(Object* object, Vector3 position, float speed, Vector3& target);
    bool hasGround(Object* object, Vector3 position);
    float minimHeight(Object* object, Vector3 position, float lastMin);
};

// ----------------------------------------- class: Box -------------------------------
class Box : public DinamicObject
{
public:
    // Atributos
    Vector3 playerPos;

    // Metodos
    Box(EntityMesh* m); //constructor

    void move(Vector3 dir, float speed, std::vector<Object*> static_objects, std::vector<DinamicObject*> dinamic_objects);
};

// ----------------------------------------- class: Floor -------------------------------
class Floor : public Object
{
public:
    // Atributos

    // Metodos
    Floor(); //constructor
};

// ----------------------------------------- class: Player -------------------------------
class Player : public DinamicObject
{
public:
    // Atributos
    Vector3 altura = Vector3(0, 70, 0);

    Camera* camera;

    DinamicObject* boxPicked;
    
    // Metodos
    Player(Camera* camera); 

    void move(Vector3 dir);

    void move(Vector3 dir, float speed, std::vector<Object*> static_objects, std::vector<DinamicObject*> dinamic_objects);

    // bool onCollision(Object* object, Vector3 position, float speed, Vector3& target);
    // bool hasGround(Object* object, Vector3 position);
    // float minimHeight(Object* object, Vector3 position, float lastMin);
};

// ----------------------------------------- class: EntityLight -------------------------------
class EntityLight {};

#endif 