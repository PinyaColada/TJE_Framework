#include "stage.h"
#include "input.h"
#include "world.h"
#include "entity.h"

float mouse_speed = 100.0f;
Object* selectedObject;

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
	for (int id=0; id < scene->objects.size(); id++)
	{
		Object* object = scene->objects[id];
		object->render(camera);
	}

	//Draw the floor grid
	drawGrid();
}

void PlayStage::Update(double elapsed_time) 
{
    Camera* camera = world->camera;
	float speed = elapsed_time * mouse_speed; //the speed is defined by the elapsed_time so it goes constant
	Player* player = world->player;

	switch(idmode){
		case GAMEPLAY: {
			// pues rotamos un poquito 
			player->model.rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f,-1.0f,0.0f));
			player->model.rotate(Input::mouse_delta.y * 0.005f, Vector3(1.0f,0.0f,0.0f));

			// Con este vector calculamos segun el frontvector hacia donde se tiene que pirar el player
			Vector3 aux(player->model.frontVector().x, 0, player->model.frontVector().z);
			aux = aux.normalize();

			// ido se mou un poquet 
			if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT) ){
				speed += 5; //move faster with left shift
				camera->fov += 1; // fer una transicio
			} else{
				speed -= 5;
				camera->fov -= .5; // fer una transicio
			} 
			camera->fov = clamp(camera->fov, 70, 80);
			speed = clamp(speed, 1, 40);

			if (Input::isKeyPressed(SDL_SCANCODE_W)) player->move(aux * speed);
			if (Input::isKeyPressed(SDL_SCANCODE_S)) player->move(-1 * aux * speed);
			if (Input::isKeyPressed(SDL_SCANCODE_A)) player->move(-1 * aux.perpendicular() * speed);
			if (Input::isKeyPressed(SDL_SCANCODE_D)) player->move(aux.perpendicular() * speed);

			// 1,2,3 accion!
			camera->lookAt(player->model.getTranslation() +  player->altura, player->altura + player->model.getTranslation() + player->model.frontVector(), Vector3(0,1.01,0));

			// Ho posam lo mes guapo que podem :)
			Input::centerMouse();
			SDL_ShowCursor(false);
			break;
		}
		case EDIT: {
			if ((Input::mouse_state & SDL_BUTTON_LEFT) || mouse_locked ) {
				camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f,-1.0f,0.0f));
				camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector( Vector3(-1.0f,0.0f,0.0f)));
			}

			if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT) ) speed *= 10; //move faster with left shift
			if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
			if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera->move(Vector3(0.0f, 0.0f,-1.0f) * speed);
			if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
			if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) camera->move(Vector3(-1.0f,0.0f, 0.0f) * speed);
			if (Input::isKeyPressed(SDL_SCANCODE_Q)) camera->moveGlobal(Vector3(0.0f,-1.0f, 0.0f) * speed);
			if (Input::isKeyPressed(SDL_SCANCODE_E)) camera->moveGlobal(Vector3(0.0f,1.0f, 0.0f) * speed);

			Object* boxPicked = world->player->boxPicked;

			if (boxPicked != NULL){
				if (Input::isKeyPressed(SDL_SCANCODE_U)) boxPicked->model.rotate(50.0f * elapsed_time * DEG2RAD, Vector3(1,0,0));
				if (Input::isKeyPressed(SDL_SCANCODE_I)) boxPicked->model.rotate(50.0f * elapsed_time * DEG2RAD, Vector3(0,1,0));
				if (Input::isKeyPressed(SDL_SCANCODE_O)) boxPicked->model.rotate(50.0f * elapsed_time * DEG2RAD, Vector3(0,0,1));
				if (Input::isKeyPressed(SDL_SCANCODE_J)) boxPicked->model.rotate(-50.0f * elapsed_time * DEG2RAD, Vector3(1,0,0));
				if (Input::isKeyPressed(SDL_SCANCODE_K)) boxPicked->model.rotate(-50.0f * elapsed_time * DEG2RAD, Vector3(0,1,0));
				if (Input::isKeyPressed(SDL_SCANCODE_L)) boxPicked->model.rotate(-50.0f * elapsed_time * DEG2RAD, Vector3(0,0,1));
			}

			if (mouse_locked)
				Input::centerMouse();
				SDL_ShowCursor(true);

			break;
		}
	}
}

void PlayStage::AddBoxInFront()
{
    Camera* camera = world->camera;
	Scene* scene = world->scenes[0];

	Vector3 origin = camera->eye;
	Vector3 dir = camera->getRayDirection(Input::mouse_position.x, Input::mouse_position.y, world->window_width, world->window_height);
	Vector3 up = Vector3(0, 1, 0);
	Vector3 pos = RayPlaneCollision(Vector3(), up, origin, dir);

	EntityMesh* mesh = world->searchMesh( eEntityName::BOX );

	Box* box = new Box( mesh );
	if( mesh == NULL ){
		world->meshs.push_back(box->mesh);
	}
	box->model.setTranslation(pos.x, pos.y, pos.z);
    scene->objects.push_back(box);
}

void PlayStage::SelectObject()
{
    Camera* camera = world->camera;
	Scene* scene = world->scenes[0];

	Vector3 origin = camera->eye;
	Vector3 dir = camera->getRayDirection(Input::mouse_position.x, Input::mouse_position.y, world->window_width, world->window_height);

	for (int id=0; id < scene->objects.size(); id++)
	{
		Object* object = scene->objects[id];

		Vector3 col;
		Vector3 normal;

		if(object->mesh == NULL) continue;
		Mesh* mesh = object->mesh->mesh;
		if (!mesh->testRayCollision(object->model,origin,dir,col,normal)) continue;
		//testRayCollision( Matrix44 model, Vector3 ray_origin, Vector3 ray_direction, Vector3& collision, Vector3& normal, float max_ray_dist = 3.4e+38F, bool in_object_space = false );

		selectedObject = object;
		// hay que implemontar para q se quede la mas cerca, con el col probablemente
		break;
	}
}