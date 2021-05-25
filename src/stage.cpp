#include "stage.h"
#include "input.h"
#include "world.h"
#include "entity.h"

float mouse_speed = 100.0f;
float acc;
float force;
float ja;
float x_rotation;
float y_rotation;
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
	Player* player = world->player;

	// 1,2,3 accion!
	if(idmode == GAMEPLAY)
		camera->lookAt(player->model.getTranslation() +  player->altura, player->altura + player->model.getTranslation() + player->model.frontVector(), Vector3(0,1.01,0));
    
    //set the camera as default
	camera->enable();

	//set flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
   
	Scene* scene = world->scenes[world->current_scene];
	
	Object* object;

	//for para static_objects
	for (int id = 0; id < scene->static_objects.size(); id++)
	{
		object = scene->static_objects[id];
		object->render(camera);
		object->mesh->mesh->renderBounding(object->model);
		// Matrix44 copymodel = object->model;
		// Mesh* mesh = object->mesh->mesh;
		// copymodel.translate(mesh->box.center.x, mesh->box.center.y, mesh->box.center.z);
		// copymodel.scale(mesh->box.halfsize.x, mesh->box.halfsize.y, mesh->box.halfsize.z);
		// object->mesh->mesh->bounding->renderBounding(copymodel);
	}

	//for para dinamic_objects
	for (int id = 0; id < scene->dinamic_objects.size(); id++)
	{
		object = scene->dinamic_objects[id];
		object->render(camera);
		object->mesh->mesh->renderBounding(object->model);
		// Matrix44 copymodel = object->model;
		// Mesh* mesh = object->mesh->mesh;
		// copymodel.translate(mesh->box.center.x, mesh->box.center.y, mesh->box.center.z);
		// copymodel.scale(mesh->box.halfsize.x, mesh->box.halfsize.y, mesh->box.halfsize.z);
		// //copymodel = copymodel * mesh->model_bounding;

		// object->mesh->mesh->bounding->renderBounding(copymodel);
	}



	//Draw the floor grid
	//drawGrid();
}

void PlayStage::Update(double elapsed_time) 
{
    Camera* camera = world->camera;
	float speed = elapsed_time * mouse_speed; //the speed is defined by the elapsed_time so it goes constant
	Player* player = world->player;
	Scene* scene = world->scenes[world->current_scene];

	Object* object;

	for (int i = 0; i < scene->dinamic_objects.size(); i++)
	{ 
		object = scene->dinamic_objects[i];
		object->physic->updateModel(elapsed_time, &object->model);
	}

	switch(idmode){
		case GAMEPLAY: {
			// pues rotamos un poquito 
			x_rotation += Input::mouse_delta.x * 0.05f * DEG2RAD;
			y_rotation += Input::mouse_delta.y * 0.05f * DEG2RAD;

			y_rotation = clamp(y_rotation, -70 * DEG2RAD, 70 * DEG2RAD);

			Vector3 dir = Vector3(	sin(x_rotation),
									sin(y_rotation), 
									cos(x_rotation) * cos(y_rotation));

			player->model.setFrontAndOrthonormalize(dir);

			// Con este vector calculamos segun el frontvector hacia donde se tiene que pirar el player
			Vector3 aux(player->model.frontVector().x, 0, player->model.frontVector().z);
			aux = aux.normalize();

			if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT) ){
				speed += 5; //move faster with left shift
				camera->fov += 1; // fer una transicio
			} else{
				speed -= 5;
				camera->fov -= .5; // fer una transicio
			} 
			camera->fov = clamp(camera->fov, 70, 80);
			speed = clamp(speed, 1, 2);

			dir = Vector3();

			if (Input::isKeyPressed(SDL_SCANCODE_W)) dir = dir + aux;
			if (Input::isKeyPressed(SDL_SCANCODE_S)) dir = dir + -1 * aux;
			if (Input::isKeyPressed(SDL_SCANCODE_A)) dir = dir + -1 * aux.perpendicular();
			if (Input::isKeyPressed(SDL_SCANCODE_D)) dir = dir + aux.perpendicular();
			if (Input::wasKeyPressed(SDL_SCANCODE_SPACE) && !player->isFalling) {
				ja = 25;
			} 

			player->move(dir, speed, scene->static_objects, scene->dinamic_objects);

			if (ja > 0){
				ja--;
				player->model.setTranslationY(player->model.getTranslation().y + force);
				force -= .5;
			} else if (ja == 0){
				force = 10;
			}

			if (player->isFalling){
				acc += .03f;
				acc = clamp(acc, -5, 3);
				player->model.setTranslationY(player->model.getTranslation().y - acc);
			} else {
				acc = 0;
			}

			// Ho posam lo mes guapo que podem :)
			Input::centerMouse();
			SDL_ShowCursor(false);

			//cubo movil
			if(player->boxPicked == NULL)
				break;
			Object* boxPicked = player->boxPicked;
			dir = player->model.frontVector();
			dir = 50 * Vector3(dir.x, 0, dir.z).normalize();
			Vector3 pos = player->model.getTranslation() + Vector3(0,10,0) + dir;
			boxPicked->model.setTranslation(pos);

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
	Scene* scene = world->scenes[world->current_scene];

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
    scene->dinamic_objects.push_back(box);
}