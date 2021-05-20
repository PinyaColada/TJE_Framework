#include "entity.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "camera.h"

// ----------------------------------------- class: Entity -----------------------------------------
Entity::Entity(){}

Vector3 Entity::getPosition() { return Vector3(1,1,1); } 

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

// ----------------------------------------- objeto de prueva -------------------------------
Cubo::Cubo(EntityMesh* m)
{
    name = eEntityName::CUBO;

    //si existeix la mesh
    if( m != NULL ){
        mesh = m;
        return;
    }

    //si no existeix la mesh
    mesh = new EntityMesh( eEntityName::CUBO );

	mesh->texture = new Texture();
 	mesh->texture->load("data/texture.tga");
	mesh->mesh = Mesh::Get("data/box.ASE");
	mesh->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    mesh->color = Vector4(1,1,1,1);
}
