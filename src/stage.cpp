#include "stage.h"
#include "input.h"
#include "world.h"
#include "entity.h"

float mouse_speed = 100.0f;

//------------------------------------ class: Stage  ----------------------------------------
void Stage::updateMouse()
{
    mouse_locked = !mouse_locked;
	SDL_ShowCursor(!mouse_locked);
}

//------------------------------------ class: PlayStage  ------------------------------------
PlayStage::PlayStage()
{
    idSatge = PLAY;

    //hide the cursor
    mouse_locked = false;
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse
}

void PlayStage::Render() 
{
    Camera* camera = world->camera;
    //set the camera as default
	camera->enable();

	//set flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
   
	//create model matrix for cube
	Scene* scene = world->scenes[0];
	//printf("size: %d\n",scene->objects.size());
	for (int id=0; id < scene->objects.size(); id++)
	{
		//printf("id: %d\n",id);
		Object* object = scene->objects[id];
		object->render(camera);
		//printf("%d\n",object->getPosition());
	}

	//Draw the floor grid
	drawGrid();
}

void PlayStage::Update(double elapsed_time) 
{
    Camera* camera = world->camera;
	float speed = elapsed_time * mouse_speed; //the speed is defined by the elapsed_time so it goes constant

	//mouse input to rotate the cam
	if ((Input::mouse_state & SDL_BUTTON_LEFT) || mouse_locked ) //is left button pressed?
	{
		camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f,-1.0f,0.0f));
		camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector( Vector3(-1.0f,0.0f,0.0f)));
	}

	//async input to move the camera around
	if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT) ) speed *= 10; //move faster with left shift
	if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera->move(Vector3(0.0f, 0.0f,-1.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) camera->move(Vector3(-1.0f,0.0f, 0.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_Q)) camera->moveGlobal(Vector3(0.0f,-1.0f, 0.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_E)) camera->moveGlobal(Vector3(0.0f,1.0f, 0.0f) * speed);

	//añadir, mover, seleccionar objetos

	//to navigate with the mouse fixed in the middle
	if (mouse_locked)
		Input::centerMouse();
}

void PlayStage::AddCuboInFront()
{
    Camera* camera = world->camera;
	Scene* scene = world->scenes[0];

	Vector3 origin = camera->eye;
	Vector3 dir = camera->getRayDirection(Input::mouse_position.x, Input::mouse_position.y, world->window_width, world->window_height);
	Vector3 pos = RayPlaneCollision(Vector3(), Vector3(0, 1, 0), origin, dir);

	Cubo* cubo = new Cubo();
	cubo->model.setTranslation(pos.x, pos.y, pos.z);
    scene->objects.push_back(cubo);
	//pq el punt 0,0,0!!! pq dir = 0!!!! 
	//video: Colisiones 2, time: 24:41
}