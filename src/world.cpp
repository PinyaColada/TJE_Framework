#include "input.h"
#include "world.h"
#include "entity.h"

// ----------------------------------------- class: Scene -----------------------------------------
Scene::Scene()
{

}

// ----------------------------------------- class: World -----------------------------------------
World::World( int window_width, int window_height ) 
{ 
    this->window_width = window_width;
	this->window_height = window_height;
    setCamera(window_width, window_height); 
    Scene* scene = new Scene();
    scenes.push_back(scene);

    player = new Player();
}

void World::setCamera( int window_width, int window_height )
{
    //create our camera
	camera = new Camera();
	camera->lookAt(Vector3(0.f,100.f, 100.f),Vector3(0.f,0.f,0.f), Vector3(0.f,1.f,0.f)); //position the camera and point to 0,0,0
	camera->setPerspective(70.f,window_width/(float)window_height,0.1f,10000.f); //set the projection, we want to be perspective
}

EntityMesh* World::searchMesh( eEntityName obj )
{
    for (int id=0; id < meshs.size(); id++)
    {
        EntityMesh* mesh = meshs[id];
        if( mesh->object == obj ) return mesh;
    }
    return NULL;
}

void World::SelectBox()
{
    if (player->boxPicked != NULL) return;
	Scene* scene = scenes[0];

	Vector3 origin = camera->eye;
	Vector3 dir = camera->getRayDirection(Input::mouse_position.x, Input::mouse_position.y, window_width, window_height);
    Object* boxPicked = NULL;

	for (int id=0; id < scene->objects.size(); id++)
	{
		Object* object = scene->objects[id];

		Vector3 col;
		Vector3 normal;

		if (object->mesh == NULL) continue;
        if (object->name != eEntityName::BOX) continue;
		Mesh* mesh = object->mesh->mesh;
		if (!mesh->testRayCollision(object->model,origin,dir,col,normal)) continue;

		if (boxPicked == NULL) boxPicked = object;

		// hay que implemontar para q se quede la mas cerca, con el col probablemente
		break;
	}
    player->boxPicked = boxPicked;
}

void World::dejarBox()
{
    if (player->boxPicked != NULL) return;

    //codigo de soltar Box

    player->boxPicked = NULL;
}