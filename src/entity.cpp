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
    Vector3 target = model.getTranslation() + dir;
    model.setTranslation(target.x, target.y, target.z);
}

void Player::trueTarget(std::vector<Object*> objects, double elapsed_time, Vector3& target)
{
    //calculamos el centro de la esfera de colisión del player elevandola hasta la cintura
    Vector3 character_center = model.getTranslation();

    //para cada objecto de la escena...
    Vector3 coll;
    Vector3 collnorm;

    for (int i = 0; i < objects.size(); i++){
        //comprobamos si colisiona el objeto con la esfera (radio 3)
        if (!objects[i]->mesh->mesh->testSphereCollision( objects[i]->model, character_center, 3, coll, collnorm)){
            std::cout<<"Aqui no hay suelo"<<std::endl;
            continue; //si no colisiona, pasamos al siguiente objeto
        }


        //si la esfera está colisionando muevela a su posicion anterior alejandola del objeto
        // Vector3 push_away = normalize(coll - character_center) * elapsed_time;
        // position = previous_pos - push_away; //move to previous pos but a little bit further
        std::cout<<"Aqui hay suelo"<<std::endl;
    }
}
