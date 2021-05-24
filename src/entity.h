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
    FLOOR
};

// ----------------------------------------- class: Entity -----------------------------------------
class Entity
{
public:
    Entity(); //constructor
    //virtual ~Entity(); //destructor
 
    //some attributes 
    eEntityName name; 
    Matrix44 model;
 
    //methods overwritten by derived classes 
    virtual void render(Camera* camera) = 0;
    virtual void update(float elapsed_time) = 0;

    //some useful methods...
    Vector3 getPosition(); 
    Vector3 getDir(); 
};

// ----------------------------------------- class: EntityMesh -------------------------------------
class EntityMesh : public Entity
{
public:
    //Attributes of this class 
    Mesh* mesh = NULL;
    Texture* texture = NULL;
    Shader* shader = NULL;
    Vector4 color;
    eEntityName object; 

    EntityMesh( eEntityName obj ); //constructor

    //methods overwritten 
    void render(Camera* camera);
    void update(float dt);
};

// ----------------------------------------- class: Object -------------------------------------
class Object : public Entity
{
 public:
    EntityMesh* mesh;
    Physics* physic;

    Object(){}; //constructor
    Object(EntityMesh* m) { mesh = m; };

    //methods overwritten 
    bool onColission(Object* target);

    void render(Camera* camera);
    void update(float dt){};
};

// ----------------------------------------- class: Box -------------------------------
class Box : public Object
{
public:

    Box(EntityMesh* m); //constructor
};

// ----------------------------------------- class: Box -------------------------------
class Floor : public Object
{
public:

    Floor(); //constructor
};

// ----------------------------------------- class: Player -------------------------------
class Player : public Object
{
public:
    // Atributos
    Vector3 altura = Vector3(0, 70, 0);

    Camera* camera;

    Object* boxPicked;

    // Metodos
    Player(Camera* camera); 

    void move(Vector3 dir);

    void move(Vector3 dir, float speed, std::vector<Object*> static_objects, std::vector<Object*> dinamic_objects);

    bool onCollision(Object* object, Vector3 centre, Vector3 position, float speed, Vector3& target, bool& isFalling);
};

// ----------------------------------------- class: EntityLight -------------------------------
class EntityLight {};

#endif 