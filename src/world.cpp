#include "world.h"
#include "entity.h"

// ----------------------------------------- class: Scene -----------------------------------------
Scene::Scene()
{
    //Cubo* cubo = new Cubo();
    //objects.push_back(cubo);
}

// ----------------------------------------- class: World -----------------------------------------
// World::World()
// {
//     Scene* scene = new Scene();
//     scenes.push_back(scene);
// }
World::World( int window_width, int window_height ) 
{ 
    this->window_width = window_width;
	this->window_height = window_height;
    setCamera(window_width, window_height); 
    Scene* scene = new Scene();
    scenes.push_back(scene);
}

void World::setCamera( int window_width, int window_height )
{
    //create our camera
	camera = new Camera();
	camera->lookAt(Vector3(0.f,100.f, 100.f),Vector3(0.f,0.f,0.f), Vector3(0.f,1.f,0.f)); //position the camera and point to 0,0,0
	camera->setPerspective(70.f,window_width/(float)window_height,0.1f,10000.f); //set the projection, we want to be perspective
}