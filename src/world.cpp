#include "world.h"
#include "entity.h"

// ----------------------------------------- class: World -----------------------------------------
World::World()
{
    cubo = new Prueva();
}

void World::setCamera( int window_width, int window_height )
{
    //create our camera
	camera = new Camera();
	camera->lookAt(Vector3(0.f,100.f, 100.f),Vector3(0.f,0.f,0.f), Vector3(0.f,1.f,0.f)); //position the camera and point to 0,0,0
	camera->setPerspective(70.f,window_width/(float)window_height,0.1f,10000.f); //set the projection, we want to be perspective
}

World::World( int window_width, int window_height ) 
{ 
    setCamera(window_width, window_height); 
    cubo = new Prueva();
}