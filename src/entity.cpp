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

    physic = new Physics();
}

// ----------------------------------------- class: Floor -------------------------------
Floor::Floor()
{
    name = FLOOR;

    //si no existeix la mesh
    mesh = new EntityMesh( FLOOR );

	mesh->texture = new Texture();
 	mesh->texture->load("data/Floor/Floor.png");
	mesh->mesh = Mesh::Get("data/Floor/Floor.obj");
	mesh->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    mesh->color = Vector4(1,1,1,1);
}


// ----------------------------------------- class: Player -------------------------------
Player::Player(Camera* camera)
{
    name = eEntityName::PLAYER;
    this->camera = camera;
}

void Player::move(Vector3 dir)
{
    // std::cout<<"donde esta"<<std::endl;
    Vector3 target = model.getTranslation() + dir;
    model.setTranslation(target.x, target.y, target.z);
}
