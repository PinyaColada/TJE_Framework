#include "entity.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "camera.h"

Camera* c;

// ----------------------------------------- class: Entity -----------------------------------------
Entity::Entity(){}

Vector3 Entity::getPosition() { return Vector3(1,1,1); } 

// ----------------------------------------- class: EntityMesh -------------------------------------
void EntityMesh::render()
{
    //get the last camera that was activated
    Camera* camera = c;

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

// ----------------------------------------- objeto de prueva -------------------------------
Prueva::Prueva()
{
	texture = new Texture();
 	texture->load("data/texture.tga");
	mesh = Mesh::Get("data/box.ASE");
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    color = Vector4(1,1,1,1);
    
}
void Prueva::renderprueva(Camera* camera)
{
    if(!shader) return;
    c = camera;

    render();
}
