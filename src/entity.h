#ifndef ENTITY_H
#define ENTITY_H

#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "camera.h"

enum eEntityName {
    ENTITY,
    MESH,
    LIGHT,
    CUBO
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

    Object(){}; //constructor
    Object(EntityMesh* m) { mesh = m; };

    //methods overwritten 
    void render(Camera* camera);
    void update(float dt){};
};

// ----------------------------------------- objeto de prueva: Cubo -------------------------------
class Cubo : public Object
{
public:

    //methods overwritten 
    Cubo(EntityMesh* m); //constructor

};

class EntityLight {};

#endif 