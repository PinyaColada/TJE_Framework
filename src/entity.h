#ifndef ENTITY_H
#define ENTITY_H

#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "camera.h"

// ----------------------------------------- class: Entity -----------------------------------------
class Entity
{
public:
    Entity(); //constructor
    //virtual ~Entity(); //destructor
 
    //some attributes 
    std::string name; 
    Matrix44 model;
 
    //methods overwritten by derived classes 
    virtual void render() = 0;
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

    //methods overwritten 
    void render();
    void update(float dt);
};

// ----------------------------------------- objeto de prueva -------------------------------
class Prueva : public EntityMesh
{
public:

    //methods overwritten 
    Prueva(); //constructor
    void renderprueva(Camera* camera);
};


#endif 