#include "stage.h"
#include "input.h"
#include "world.h"
#include "entity.h"
#include "loader.h"

//float mouse_speed = 100.0f;
float x_rotation;
float y_rotation;
Object* selectedObject;

//------------------------------------ class: Stage  ----------------------------------------
void Stage::updateMouse()
{
    mouse_locked = !mouse_locked;
	SDL_ShowCursor(!mouse_locked);
}

eStageID Stage::passStage()
{
	if(isComplite)
	{
		isComplite = false;
		return nextSatge;
	}
	else
	{
		return idSatge;
	}
}

//------------------------------------ class: IntroStage  ----------------------------------
IntroStage::IntroStage()
{
	idSatge = INTRO;
	nextSatge = MENU;
}

void IntroStage::Render()
{
	drawText(100, 100, "INTRO", Vector3(1, 1, 1), 10);
}

void IntroStage::Update(double elapsed_time)
{

}

//events
void IntroStage::onKeyDown( SDL_KeyboardEvent event )
{
	isComplite = true;
}

void IntroStage::onMouseButtonDown( SDL_MouseButtonEvent event )
{

}

//------------------------------------ class: MenuStage  ----------------------------------
MenuStage::MenuStage()
{
	idSatge = MENU;
	nextSatge = PLAY;
}

void MenuStage::Render()
{
	drawText(100, 100, "MENU", Vector3(1, 1, 1), 10);
}

void MenuStage::Update(double elapsed_time)
{

}

//events
void MenuStage::onKeyDown( SDL_KeyboardEvent event )
{
	isComplite = true;
}

void MenuStage::onMouseButtonDown( SDL_MouseButtonEvent event )
{

}

//------------------------------------ class: PlayStage  ------------------------------------
PlayStage::PlayStage()
{
    idSatge = PLAY;
	nextSatge = END;

    //hide the cursor
    mouse_locked = false;
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse
}

void PlayStage::Render() 
{
	Camera* camera = world->camera;
	Player* player = world->player;
	cfgPlayer* cfgP = player->cfgP;

	// moure la camera a la pos del player
	if(idmode == GAMEPLAY)
		camera->lookAt(player->model.getTranslation() +  cfgP->altura, cfgP->altura + player->model.getTranslation() + player->model.frontVector(), Vector3(0,1.01,0));
    
    //set the camera as default
	camera->enable();

	//set flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
   
	Scene* scene = world->scenes[world->current_scene];
	Object* object;

	// Skybox
	EntityMesh* skybox = scene->skybox;
	if (skybox != NULL){
		// mover skybox en la pos de player
		skybox->model.setTranslation(camera->eye);

		// render de skybox
		skybox->render(camera);
	}

	//render objectes	
	for (int id = 0; id < scene->static_objects.size(); id++)
	{
		//for para static_objects	
		object = scene->static_objects[id];
		object->render(camera, scene->lights);
	}

	for (int id = 0; id < scene->dinamic_objects.size(); id++)
	{
		//for para static_objects	
		object = scene->dinamic_objects[id];
		object->render(camera, scene->lights);
	}

	DinamicObject* picked = world->boxPicked;
	if (picked != NULL)
		picked->mesh->mesh->bounding->renderBounding(picked->model);

	Object* BlockPic = world->BlockPicked;
	if (BlockPic != NULL)
		BlockPic->mesh->mesh->bounding->renderBounding(BlockPic->model);

	//Draw the floor grid	
	drawGrid();
}

void PlayStage::Update(double elapsed_time) 
{
    Camera* camera = world->camera;
	Player* player = world->player;
	Scene* scene = world->scenes[world->current_scene];
	float speed = player->Speed; // obtenim el valor de speed per alterarlo

	DinamicObject* object;

	for (int i = 0; i < scene->dinamic_objects.size(); i++)
	{ 
		object = scene->dinamic_objects[i];
		if(!object->isCatch)
			object->move(Vector3(), elapsed_time, scene->static_objects, scene->dinamic_objects);
	}

	// buscar la box que pot ser piked
	world->SelectBox();

	switch(idmode){
		case GAMEPLAY: {
			// rotem la camera
			x_rotation += Input::mouse_delta.x * 0.05f * DEG2RAD;
			y_rotation += Input::mouse_delta.y * 0.05f * DEG2RAD;

			y_rotation = clamp(y_rotation, -70 * DEG2RAD, 70 * DEG2RAD);

			Vector3 dir = Vector3(	sin(x_rotation),
									sin(y_rotation), 
									cos(x_rotation) * cos(y_rotation));

			player->model.setFrontAndOrthonormalize(dir);
	
			// apliquem els efectes de la camera i speed
			bool isShift = Input::isKeyPressed(SDL_SCANCODE_LSHIFT); // si tenim el Shift pulsat
			camera->fov += isShift? 1 : -0.5;  // actualitzem el fov
			speed *= isShift? 1.1 : 0.9;       // actualitzem la speed
			
			cfgPlayer* cfgP = player->cfgP;
			// fem clamps del valors
			camera->fov = clamp(camera->fov, 70, 80);
			speed = clamp(speed, cfgP->minSpeed, cfgP->maxSpeed);
			player->Speed = speed; // guardem els canvis de speed

			// Calculem el moviment del player
			Vector3 aux(player->model.frontVector().x, 0, player->model.frontVector().z);
			aux = aux.normalize();
			dir = Vector3();
			
			if (Input::isKeyPressed(SDL_SCANCODE_W)) dir = dir + aux;
			if (Input::isKeyPressed(SDL_SCANCODE_S)) dir = dir + -1 * aux;
			if (Input::isKeyPressed(SDL_SCANCODE_A)) dir = dir + -1 * aux.perpendicular();
			if (Input::isKeyPressed(SDL_SCANCODE_D)) dir = dir + aux.perpendicular();
			if (Input::wasKeyPressed(SDL_SCANCODE_SPACE) && !player->isFalling) {
				player->physic->Jump();
			} 

			player->move(dir, elapsed_time, scene->static_objects, scene->dinamic_objects);


			// Ho posam lo mes guapo que podem :)
			Input::centerMouse();
			SDL_ShowCursor(false);

			break;
		}
		case EDIT: {
			speed *= elapsed_time;

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

			Object* BlockPicked = world->BlockPicked;

			if (BlockPicked != NULL) {
				if (Input::isKeyPressed(SDL_SCANCODE_U)) BlockPicked->model.translate(Vector3(0, 1, 0));
				if (Input::isKeyPressed(SDL_SCANCODE_I)) BlockPicked->model.translate(Vector3(1, 0, 0));
				if (Input::isKeyPressed(SDL_SCANCODE_O)) BlockPicked->model.translate(Vector3(0, -1, 0));
				if (Input::isKeyPressed(SDL_SCANCODE_J)) BlockPicked->model.translate(Vector3(0, 0, 1));
				if (Input::isKeyPressed(SDL_SCANCODE_K)) BlockPicked->model.translate(Vector3(-1, 0, 0));
				if (Input::isKeyPressed(SDL_SCANCODE_L)) BlockPicked->model.translate(Vector3(0, 0, -1));
				if (Input::isKeyPressed(SDL_SCANCODE_N)) BlockPicked->model.rotate(0.01f, Vector3(0, 1, 0));
				if (Input::isKeyPressed(SDL_SCANCODE_M)) BlockPicked->model.rotate(-0.01f, Vector3(0, 1, 0));
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

	// calculem les dades de la nova Box
	Vector3 origin = camera->eye;
	Vector3 dir = camera->getRayDirection(Input::mouse_position.x, Input::mouse_position.y, world->window_width, world->window_height);
	Vector3 up = Vector3(0, 1, 0);
	Vector3 pos = RayPlaneCollision(Vector3(), up, origin, dir);

	// busque la mesh de la box
	EntityMesh* mesh = world->searchMesh( eEntityName::BOX );

	// creem la Box
	Box* box = new Box(mesh, pos + Vector3(0, 1000, 0));
	// si no existia la mesh guardem la nova mesh en la llista
	if( mesh == NULL ){
		world->meshs.push_back(box->mesh);
	}

	// afegim la Box a la llista de objectes
	box->idList = scene->dinamic_objects.size();
    scene->dinamic_objects.push_back(box);
}

void PlayStage::addBlockInFront(eBlocktype type)
{
	Camera* camera = world->camera;
	Scene* scene = world->scenes[world->current_scene];

	// calculem les dades del nou BlocK
	Vector3 origin = camera->eye;
	Vector3 dir = camera->getRayDirection(Input::mouse_position.x, Input::mouse_position.y, world->window_width, world->window_height);
	Vector3 up = Vector3(0, 1, 0);
	Vector3 pos = RayPlaneCollision(Vector3(), up, origin, dir);

	// busque la mesh del Block
	EntityMesh* mesh = NULL;
	switch (type) {
	case BLARGE: mesh = world->searchMesh(BLOCKLARGE); break;
	case BLONG: mesh = world->searchMesh(BLOCKLONG); break;
	case BUNIT: mesh = world->searchMesh(BLOCKUNIT); break;
	default: break;
	}

	// creem el Block
	Block* block = new Block(mesh, pos, type);
	// si no existia la mesh guardem la nova mesh en la llista
	if (mesh == NULL) {
		world->meshs.push_back(block->mesh);
	}

	// afegim el Block a la llista de objectes
	block->idList = scene->static_objects.size();
	scene->static_objects.push_back(block);
}

//events
void PlayStage::onKeyDown( SDL_KeyboardEvent event )
{
	switch (idmode) {
		case GAMEPLAY: {
			switch (event.keysym.sym) {
				case SDLK_3: idmode = EDIT; break;
			}
			break;
		}
		case EDIT: {
			switch (event.keysym.sym) {
				case SDLK_1: AddBoxInFront(); break;
				case SDLK_2:
					if (world->player->boxPicked == NULL)
						world->player->SelectBox(world->boxPicked);
					else
						world->player->LeaveBox();
					break;
				case SDLK_3: idmode = GAMEPLAY; break;
				case SDLK_5: addBlockInFront(BLARGE); break;
				case SDLK_6: addBlockInFront(BLONG); break;
				case SDLK_7: addBlockInFront(BUNIT); break;
				case SDLK_8: world->editMap(); break;
				case SDLK_F3: isComplite = true; break;
				case SDLK_F4: SaveLevel(world->SaveScene()); break;
			}
			break;
		}
	}	
}

void PlayStage::onMouseButtonDown( SDL_MouseButtonEvent event )
{
	switch(event.button)
	{
		case SDL_BUTTON_MIDDLE:
			updateMouse();
			break;
		case SDL_BUTTON_LEFT:
			if (idmode != GAMEPLAY)
				break;
			Player* player = world->player;
			if(player->boxPicked == NULL)
				player->SelectBox(world->boxPicked);
			else 
				player->LeaveBox();
			break;
	}
}

//------------------------------------ class: EndStage  ----------------------------------
EndStage::EndStage()
{
	idSatge = END;
	nextSatge = MENU;
}

void EndStage::Render()
{
	drawText(100, 100, "END", Vector3(1, 1, 1), 10);
}

void EndStage::Update(double elapsed_time)
{

}

//events
void EndStage::onKeyDown( SDL_KeyboardEvent event )
{
	isComplite = true;
}

void EndStage::onMouseButtonDown( SDL_MouseButtonEvent event )
{

}