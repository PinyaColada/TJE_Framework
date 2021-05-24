#include "entity.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "camera.h"

// ----------------------------------------- class: Entity -----------------------------------------
Entity::Entity(){}

Vector3 Entity::getPosition() { return Vector3(1,1,1); } 
Vector3 Entity::getDir() { return model.frontVector(); }

// ----------------------------------------- class: EntityMesh -------------------------------------
EntityMesh::EntityMesh( eEntityName obj )
{
    name = eEntityName::MESH;
    object = obj;
}

void EntityMesh::render(Camera* camera)
{
    //enable shader and pass uniforms
    shader->enable();
    shader->setUniform("u_color", color);
    shader->setUniform("u_model", model);
    shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
    shader->setTexture("u_texture", texture, 0 );

    //render the mesh using the shader
    mesh->render( GL_TRIANGLES );

    //disable the shader after finishing rendering
    shader->disable(); 

}

void EntityMesh::update(float dt)
{

}

// ----------------------------------------- class: Object -------------------------------------
// bool Object::onColission(Object* target)
// {

// }

void Object::render(Camera* camera)
{
    if(mesh == NULL) return;
    mesh->model = this->model;
    mesh->render(camera);
}
// ----------------------------------------- class: Box -------------------------------
Box::Box(EntityMesh* m)
{
    name = BOX;
    physic = new Physics();

    //si existeix la mesh
    if( m != NULL ){
        mesh = m;
        return;
    }

    //si no existeix la mesh
    mesh = new EntityMesh( BOX );

	mesh->texture = new Texture();
 	mesh->texture->load("data/Box/MetalBox.png");
	mesh->mesh = Mesh::Get("data/Box/MetalBox.obj");
	mesh->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    mesh->color = Vector4(1,1,1,1);
}

// ----------------------------------------- class: Floor -------------------------------
Floor::Floor()
{
    name = FLOOR;

    //si no existeix la mesh
    mesh = new EntityMesh( FLOOR );
    physic = new Physics();

	mesh->texture = new Texture();
 	mesh->texture->load("data/Floor/Floor.png");
	mesh->mesh = Mesh::Get("data/Floor/Floor.obj");
	mesh->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    mesh->color = Vector4(1,1,1,1);
}


// ----------------------------------------- class: Player -------------------------------
Player::Player(Camera* camera)
{
    physic = new Physics();
    name = eEntityName::PLAYER;
    this->camera = camera;
}

void Player::move(Vector3 dir)
{
    Vector3 target = model.getTranslation() + dir;
    model.setTranslation(target.x, target.y, target.z);
}

float acc;
float minim_y;

void Player::move(Vector3 dir, float speed, std::vector<Object*> static_objects, std::vector<Object*> dinamic_objects, double elapsed_time)
{
    //calculamos el target idial
    Vector3 position = model.getTranslation();
    Vector3 target = position + dir * speed;
    Vector3 centreplayer = position + Vector3(0,3,0);

    //calculamos las coliciones
    Object* object;
    bool isFalling = true;
    minim_y = -100;

    //for para static_objects
    for (int i = 0; i < static_objects.size(); i++)
    {
        object = static_objects[i];
        // if(!onCollision(object, centreplayer, position, speed, target, isFalling))
        //     continue;
        onCollision(object, centreplayer, position, speed, target, isFalling);
    }

    //for para dinamic_objects
    for (int i = 0; i < dinamic_objects.size(); i++)
    {
        object = dinamic_objects[i];
        // if(!onCollision(object, centreplayer, position, speed, target, isFalling))
        //     continue;
        onCollision(object, centreplayer, position, speed, target, isFalling);
    }

    //calculamos la pos final
    if (isFalling){
        acc += 0.0783; // Esta constante es extremadamente importante para que el juego funcione no tocar
        acc = clamp(acc, -5, 30);
        target.y -= acc;
        target.y = clamp(target.y, minim_y, 10000);
    } else {
        acc = 0;
    }

    //aplicamos el movimiento
    model.setTranslation(target.x, target.y, target.z);
}

bool Player::onCollision(Object* object, Vector3 centre, Vector3 position, float speed, Vector3& target, bool& isFalling)
{
    //calculamos la colision de 1 objeto
    Vector3 coll, norm;
    float target_y = target.y;
    

    if (object->mesh->mesh->testSphereCollision( object->model, centre, 3.1, coll, norm) &&
        object->mesh->mesh->testRayCollision( object->model, coll, Vector3(0, -1, 0), coll, norm, 3.1))
        isFalling = false;

    if (object->mesh->mesh->testRayCollision( object->model, position, Vector3(0, -1, 0), coll, norm))
        minim_y = coll.y + 0.1;

    if (object->mesh == NULL)
        return false;
    if (!object->mesh->mesh->testSphereCollision( object->model, centre, 3, coll, norm))
        return false;
    
    //actualizamos el objetivo
    Vector3 push_away = normalize(coll - position) * speed;
    target = position - push_away;
    
    //comprovamos si el eje y es correcto
    target.y = target_y;

    return true;
}
