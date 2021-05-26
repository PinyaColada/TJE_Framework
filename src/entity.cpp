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
	mesh->mesh = Mesh::getMeshAndBounding("data/Box/MetalBox.obj","data/Box/ColissionBox.obj");
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
	mesh->mesh = Mesh::getMeshAndBounding("data/Floor/Floor.obj");
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

float minim_y;
float radius = 3;
float margen = 0.1;
Vector3 centreplayer = Vector3(0,radius,0);

void Player::move(Vector3 dir, float speed, std::vector<Object*> static_objects, std::vector<Object*> dinamic_objects)
{
    //calculamos el target idial
    Vector3 position = model.getTranslation();
    Vector3 target = position + dir * speed;

    //calculamos las coliciones
    Object* object;
    isFalling = true;
    minim_y = -1000;

    //for para static_objects
    for (int i = 0; i < static_objects.size(); i++)
    {
        object = static_objects[i];
        if(onCollision(object, position, speed, target))
            continue;
        if(hasGround(object, position)){
            isFalling = false;
            continue;
        }
        minim_y = minimHeight(object, position, minim_y);
    }

    //for para dinamic_objects
    for (int i = 0; i < dinamic_objects.size(); i++)
    {
        object = dinamic_objects[i];
        if(onCollision(object, position, speed, target))
            continue;
        if(hasGround(object, position)){
            isFalling = false;
            continue;
        }
        minim_y = minimHeight(object, position, minim_y);
    }

    target.clampY(minim_y, 10000);
    //std::cout<<"target.y: "<<target.y<<std::endl;
    //std::cout<<"minim_y: "<<minim_y<<std::endl;

    //aplicamos el movimiento
    model.setTranslation(target.x, target.y, target.z);
}

bool Player::onCollision(Object* object, Vector3 position, float speed, Vector3& target)
{
    //calculamos la colision de 1 objeto
    Vector3 coll, norm;
    float target_y = target.y;
    Vector3 centre = position + centreplayer;

    if (object->mesh == NULL || !object->mesh->mesh->testSphereBoundingCollision( object->model, centre, radius, coll, norm))
        return false;
    
    //actualizamos el objetivo
    Vector3 push_away = normalize(coll - position) * speed;
    target = position - push_away;
    
    //comprovamos si el eje y es correcto
    target.y = target_y;

    return true;
}

bool Player::hasGround(Object* object, Vector3 position)
{
    Vector3 coll, norm;

    if (object->mesh->mesh->testRayBoundingCollision( object->model, position, Vector3(0, -1, 0), coll, norm, radius + margen))
        return true;
    
    return false;
}

float Player::minimHeight(Object* object, Vector3 position, float lastMin)
{
    Vector3 coll, norm;
    float minim = lastMin;

    if (object->mesh->mesh->testRayBoundingCollision( object->model, position, Vector3(0, -1, 0), coll, norm)){
        if (!(coll.y + margen > minim)){
            // if(object->name == BOX){
            //     printf("caixa\n");
            //     // printf("minim: %f\n", minim);
            // }
            minim = coll.y + margen;
            // if(object->name == BOX){
            //     printf("minim: %f\n", minim);
            //     printf("coll.y: %f\n", coll.y);
            // }
        } // no se si coll s'actualizaria be
    }
    
    return minim;
}