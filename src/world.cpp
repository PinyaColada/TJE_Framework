#include "input.h"
#include "world.h"
#include "entity.h"

// ----------------------------------------- class: Scene -----------------------------------------
Scene::Scene() 
{
    spawn = Vector3(0,120,0);
    getSkybox("data/Skybox/sphere.obj", "data/Skybox/Night.png");
}

void Scene::getSkybox(const char* fileSkybox, const char* fileSkyboxTex)
{
    skybox = new EntityMesh( SKYBOX );

    skybox->texture = new Texture();
 	skybox->texture->load(fileSkyboxTex);
	skybox->mesh = Mesh::Get(fileSkybox);
    skybox->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    skybox->color = Vector4(1, 1, 1, 1);
    skybox->model.rotate(180 * DEG2RAD, Vector3(0, 0, 1));
}

// ----------------------------------------- class: World -----------------------------------------
World::World( int window_width, int window_height ) 
{ 
    this->window_width = window_width;
	this->window_height = window_height;

    setCamera(window_width, window_height); 

    Scene* scene = new Scene();
    scenes.push_back(scene);

    current_scene = DEMO;
    // A partir de aqui empezamos a cargar el mapa.
    Object* floor = new Floor();
    scenes[current_scene]->static_objects.push_back(floor);

    player = new Player();
    player->spawn = scene->spawn;
}

void World::setCamera( int window_width, int window_height )
{
    //create our camera
	camera = new Camera();
	camera->lookAt(Vector3(0.f,100.f, 100.f),Vector3(1.f,0.f,0.f), Vector3(0.f,1.f,0.f)); //position the camera and point to 0,0,0
	camera->setPerspective(70.f,window_width/(float)window_height,0.1f,10000.f); //set the projection, we want to be perspective
}

EntityMesh* World::searchMesh( eEntityName obj )
{
    EntityMesh* mesh;
    
    for (int id=0; id < meshs.size(); id++)
    {
        mesh = meshs[id];
        if( mesh->object == obj ) return mesh;
    }
    return NULL;
}

void World::SelectBox()
{
    boxPicked = NULL;
    if (player->boxPicked != NULL) 
        return;
    Scene* scene = scenes[0];

    Vector3 origin = camera->eye;
    Vector3 dir = camera->getRayDirection(Input::mouse_position.x, Input::mouse_position.y, window_width, window_height);
    Vector3 col, normal, pos;
    float distPicked, distObject;
    DinamicObject* object;
    Mesh* mesh; 
    
    for (int id=0; id < scene->dinamic_objects.size(); id++)
    {
        object = scene->dinamic_objects[id];

        pos = object->getPosition();

        if (object->mesh == NULL) continue;
        if (object->name != BOX) continue;

        mesh = object->mesh->mesh;

        if (!mesh->testRayCollision(object->model,origin,dir,col,normal,100)) 
            continue;

        if (boxPicked == NULL){
            boxPicked = object;
            continue;
        }
        // LLeva coses que se creen aqui
        distPicked = boxPicked->getPosition().distance(origin);
        distObject = object->getPosition().distance(origin);
        
        if (distPicked > distObject){
            boxPicked = object;
        }
    }   
}
