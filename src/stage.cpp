#include "stage.h"
#include "input.h"
#include "world.h"
#include "entity.h"
#include "loader.h"

#define w_gui 190
#define h_gui 49

float x_rotation;
float y_rotation;

HCHANNEL channel_for_walking;

bool isOnAir;

// ------------------------------------ class: Stage  ----------------------------------------
void Stage::Render()		
{
   	// render Game		
    if(needRender)		
    {		
        glDisable(GL_BLEND);		
        glEnable(GL_DEPTH_TEST);		
        glDisable(GL_CULL_FACE);		

		RenderGame();		
	}		
    // render Gui		
    if(isGui)		
    {		
        glDisable(GL_DEPTH_TEST);		
        glDisable(GL_CULL_FACE);		
        glEnable(GL_BLEND);		
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);		

        RenderGui();		

        glDisable(GL_BLEND);		
        glEnable(GL_CULL_FACE);		
        glEnable(GL_DEPTH_TEST);		
	}		
}

void Stage::RenderGui()		
{
	sElementGui elm;
    for(int i = 0; i < guiElements.size(); i++)	
	{
		elm = guiElements[i];
		if(gui->render(elm))
			onPressButton(elm.type);
	}
}

void Stage::RenderWorld()
{
	if(!isWorld)		
    {		
        std::cout << "Error: needed World" << std::endl;		
        return;		
    }
	// render de World
	world->Render();
}

void Stage::UpdateWorld(double elapsed_time, bool isMove)
{
	if(!isWorld)
    {		
        std::cout << "Error: needed World" << std::endl;		
        return;		
    }
	// update de World
	world->Update(elapsed_time);

	Player* player = world->player;
	Scene* scene = world->scenes[world->current_scene];

	// update del player
	player->move(elapsed_time, Vector3(), scene->static_objects, scene->dinamic_objects);

	if(!isMove)
		return;

	// rotem la camera
	x_rotation += 0.1 * DEG2RAD;
	y_rotation += Input::mouse_delta.y * 0.005f * DEG2RAD;

	y_rotation = clamp(y_rotation, -20 * DEG2RAD, 20 * DEG2RAD);

	Vector3 dir = Vector3(	cos(y_rotation) * sin(x_rotation),
							sin(y_rotation), 
							cos(y_rotation) * cos(x_rotation));

	world->player->model.setFrontAndOrthonormalize(dir);
}

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
		return whatNext();
	}
	else
	{
		return idSatge;
	}
}

// ------------------------------------ class: IntroStage  ----------------------------------
IntroStage::IntroStage(World* w, Gui* g)
{
	idSatge = INTRO;
	nextSatge = INTRO;
	needRender = true;

	if(w != NULL) setWorld(w);
	if(g == NULL)
		return;

	setGui(g);

	// creem els diferents elements
	guiElements.reserve(4);
	guiElements.push_back(gui->creatElement(TITLE, 0, -170, 2 * w_gui, h_gui));
	guiElements.push_back(gui->creatElement(START, 0, -56, w_gui, h_gui));
	guiElements.push_back(gui->creatElement(CONTINUE, 0, 0, w_gui, h_gui));
	guiElements.push_back(gui->creatElement(LEAVE, 0, 56, w_gui, h_gui));

	// guardem el per altera la pos
	title = &guiElements[0];
	center.x = title->pos.x;
	center.y = title->pos.y;
}

void IntroStage::RenderGame()
{
	RenderWorld();
}

void IntroStage::Update(double elapsed_time)
{
	UpdateWorld(elapsed_time);

	if(gui == NULL)
		return;
	
	time += elapsed_time;

	float A = 30;
	float vx = 0.7;
	float vy = 0.8;

	title->pos.x = A * sin(vx * time) + center.x;
	title->pos.y = A * sin(vy * time) + center.y;
}

// events
void IntroStage::onKeyDown( SDL_KeyboardEvent event )
{
	isComplite = true;
}

void IntroStage::onMouseButtonDown( SDL_MouseButtonEvent event )
{

}

void IntroStage::onPressButton(eElementsGui type)		
{		
    switch (type)
	{
	case CONTINUE:
		nextSatge = PLAY;
		isComplite = true;
		break;
	case LEAVE:
		nextSatge = EXIT;
		isComplite = true;
		break;
	case START:
		nextSatge = TUTORIAL;
		isComplite = true;
		world->reset();
		break;
	
	default:
		break;
	}
}

eStageID IntroStage::whatNext()
{
	eStageID next = nextSatge;
	nextSatge = INTRO;
	return next;
}

// ------------------------------------ class: MenuStage  ----------------------------------
MenuStage::MenuStage(World* w, Gui* g)
{
	idSatge = MENU;
	nextSatge = PLAY;
	needRender = true;

	if(w != NULL) setWorld(w);
	if(g == NULL)
		return;
	
	setGui(g);

	// creem els diferents elements
	guiElements.reserve(2);
	guiElements.push_back(gui->creatElement(CONTINUE, 0, -28, w_gui, h_gui));
	guiElements.push_back(gui->creatElement(LEAVE, 0, 28, w_gui, h_gui));
}

void MenuStage::RenderGame()
{
	RenderWorld();
}

void MenuStage::Update(double elapsed_time)
{
	UpdateWorld(elapsed_time, false);
}

// events
void MenuStage::onKeyDown( SDL_KeyboardEvent event )
{
	isComplite = true;
}

void MenuStage::onMouseButtonDown( SDL_MouseButtonEvent event )
{

}

void MenuStage::onPressButton(eElementsGui type)		
{		
	switch (type)
	{
	case CONTINUE:
		isComplite = true;
		break;
	case LEAVE:
		nextSatge = EXIT;
		isComplite = true;
		break;

	default:
		break;
	}
}

eStageID MenuStage::whatNext()
{
	eStageID next = nextSatge;
	nextSatge = PLAY;
	return next;
}

// ------------------------------------ class: TutorStage  ----------------------------------
TutorStage::TutorStage(World* w, Gui* g)
{
	idSatge = TUTORIAL;
	nextSatge = PLAY;
	needRender = true;

	if(w != NULL) setWorld(w);
	if(g == NULL)
		return;
	
	setGui(g);

	// creem els diferents elements
	guiElements.reserve(1);
	guiElements.push_back(gui->creatElement(TEXT, 0, 0, w_gui, h_gui, true));
}

// events
void TutorStage::onKeyDown( SDL_KeyboardEvent event )
{
	isComplite = true;
}

// ------------------------------------ class: PlayStage  ------------------------------------
PlayStage::PlayStage(World* w, Gui* g)
{
    idSatge = PLAY;
	nextSatge = END;
	needRender = true;

	if(w != NULL) setWorld(w);
	if(g != NULL) setGui(g);

	// --- Audio ---
	#ifdef _WINDOWS_
	stopTimeAudio = new Audio("data/Sounds/timestop.wav", false);
	#endif

    // hide the cursor
    mouse_locked = false;
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse
}

void PlayStage::RenderGame()
{
	if(!isWorld)		
    {		
        std::cout << "Error: needed World" << std::endl;		
        return;		
    }

	// render de World
	world->Render(idmode == GAMEPLAY);

	// Set parametres
	Camera* camera = world->camera;

	// Box seleccionada
	DinamicObject* picked = world->boxPicked;

	if (picked != NULL) {
		picked->mesh->mesh->bounding->renderBounding(picked->model);
	}

	// Block seleccionat
	Object* BlockPic = world->BlockPicked;

	if (BlockPic != NULL) {
		BlockPic->mesh->mesh->bounding->renderBounding(BlockPic->model);
		if (hasSaw(BlockPic->oName)) {
			((Saw*)BlockPic)->renderLimits(camera);
		}
	}

	//Draw the floor grid	
	//drawGrid();
}

void PlayStage::Update(double elapsed_time) 
{
	// Error de no tenir World
	if(!isWorld)		
	{		
        std::cout << "Error: needed World" << std::endl;		
        return;		
    }

	// Si has completat el joc
	if(world->hasWin()) {
		isComplite = true;
		world->player->pickedJewel = 0;
		world->changeScene(STARTLEVEL);
		return;
	}

	// Set parametres
    Camera* camera = world->camera;
	Player* player = world->player;

	// comprova si has mort
	if(player->isDead)
		world->respawn();
	// comprova si canvias de nivell
	if(world->current_scene != player->current_scene)
		world->changeScene(player->current_scene);

	// Set parametres
	Scene* scene = world->scenes[world->current_scene];
	float speed = player->Speed; // obtenim el valor de speed per alterarlo

	// Coyote Jump
	if (player->isFalling)
		player->fallingCounter -= elapsed_time;
	else
		player->fallingCounter = player->coyoteJump;

	// Parar temps
	timeCounter -= elapsed_time;
	coolDownCounter -= elapsed_time;

	isTimeStopped = timeCounter > 0 ? true : false;

	// update de objectes dinamics
	world->Update(elapsed_time);

	// buscar la box que pot ser piked
	world->SelectBox();

	switch(idmode){
		case GAMEPLAY: {
			// rotem la camera
			// calculem la rotacio actual
			x_rotation = atan2(player->getDir().x,player->getDir().z);
			y_rotation = asin(player->getDir().y);

			//x_rotation = asin(player->getDir().x);
			//y_rotation = asin(player->getDir().y);

			//x_rotation *= (cos(x_rotation) * cos(y_rotation) == player->getDir().z)? 1 : -1;

			// apliquem el moviment del ratoli
			x_rotation += Input::mouse_delta.x * 0.05f * DEG2RAD;
			y_rotation += Input::mouse_delta.y * 0.05f * DEG2RAD;

			y_rotation = clamp(y_rotation, -70 * DEG2RAD, 70 * DEG2RAD);

			Vector3 dir = Vector3(	cos(y_rotation) * sin(x_rotation),
									sin(y_rotation), 
									cos(y_rotation) * cos(x_rotation));

			player->model.setFrontAndOrthonormalize(dir);
	
			// apliquem els efectes de la camera i speed
			bool isShift = Input::isKeyPressed(SDL_SCANCODE_LSHIFT); // si tenim el Shift pulsat
			camera->fov += isShift ? 1 : -0.5;  // actualitzem el fov
			speed *= isShift ? 1.1 : 0.9;       // actualitzem la speed
			
			cfgPlayer* cfgP = player->cfgP;
			// fem clamps del valors
			camera->fov = clamp(camera->fov, 70, 80);
			speed = clamp(speed, cfgP->minSpeed, cfgP->maxSpeed);
			player->Speed = speed; // guardem els canvis de speed

			// Calculem el moviment del player
			Vector3 aux(player->getDir().x, 0, player->getDir().z);
			aux = aux.normalize();
			dir = Vector3();
			
			if (Input::isKeyPressed(SDL_SCANCODE_W)) dir = dir + aux;
			if (Input::isKeyPressed(SDL_SCANCODE_S)) dir = dir + -1 * aux;
			if (Input::isKeyPressed(SDL_SCANCODE_A)) dir = dir + -1 * aux.perpendicular();
			if (Input::isKeyPressed(SDL_SCANCODE_D)) dir = dir + aux.perpendicular();
			if (Input::wasKeyPressed(SDL_SCANCODE_SPACE) && (player->fallingCounter > 0) && !player->hasJumped) {
				player->physic->Jump();
				player->hasJumped = true;

				// --- Audio ---
				#ifdef _WINDOWS_
				player->jumpingAudio->play(0.25);
				#endif
			} 
			if (Input::wasKeyPressed(SDL_SCANCODE_Q) && (coolDownCounter < 0)) {
				// --- Audio ---
				#ifdef _WINDOWS_
				stopTimeAudio->play(0.05);
				#endif

				timeCounter = 9;
				coolDownCounter = timeCounter + 3;
			}

			// --- Audio ---
			#ifdef _WINDOWS_
			if ((dir != Vector3()) && !player->isFalling) {
				if (isShift) {
					if (!player->isSoundOfRunning) {
						player->runningAudio->play(0.05);
						player->walkingAudio->stop();
						player->isSoundOfRunning = true;
						player->isSoundOfWalking = false;
					}
				}
				else {
					if (!player->isSoundOfWalking) {
						player->walkingAudio->play(0.05);
						player->runningAudio->stop();
						player->isSoundOfRunning = false;
						player->isSoundOfWalking = true;
					}
				}
			} else {
				if (player->isSoundOfWalking || player->isSoundOfRunning) {
					player->walkingAudio->stop();
					player->runningAudio->stop();
					player->isSoundOfRunning = false;
					player->isSoundOfWalking = false;
				}
			}
			#endif

			player->move(elapsed_time, dir, scene->static_objects, scene->dinamic_objects);

			// --- Audio ---
			
			if (isOnAir && !player->isFalling) {
				player->hasJumped = false;
				#ifdef _WINDOWS_
				player->landingAudio->play(0.25);
				#endif
			}


			isOnAir = player->isFalling;
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
				if (!hasSaw(BlockPicked->oName))
					editBlocks(BlockPicked);
				else {
					Saw* sawPicked = (Saw*)BlockPicked;
					editSaw(sawPicked);
				}
			}


			if (mouse_locked)
				Input::centerMouse();
				SDL_ShowCursor(true);

			break;
		}
	}
}

void PlayStage::addDinamicInFront(eObjectName type)
{
	if(!hasDinamic(type))
	{
		return;
	}

    Camera* camera = world->camera;
	Scene* scene = world->scenes[world->current_scene];

	// calculem les dades de la nova Box
	Vector3 origin = camera->eye;
	Vector3 dir = camera->getRayDirection(Input::mouse_position.x, Input::mouse_position.y, world->window_width, world->window_height);
	Vector3 up = Vector3(0, 1, 0);
	Vector3 pos = RayPlaneCollision(Vector3(), up, origin, dir);

	// busque la mesh de la box
	EntityMesh* mesh = world->searchMesh( type );

	// creem el objecte dinamic
	DinamicObject* dinamic;
	switch (type)
	{
	case BOX:
		dinamic = new Box(mesh, pos + Vector3(0, h_spawn, 0));
		break;
	case SAW:
		dinamic = new SawBasic(mesh, pos);
		break;
	case SAWHUNTER:
		dinamic = new SawHunter(mesh, pos);
		break;
	default:
		std::cout << " the object \"" << TableObj2str[type].name << "\" is not a dinamic object" << std::endl;
		break;
	}

	// si no existia la mesh guardem la nova mesh en la llista
	if( mesh == NULL ){
		world->meshs.push_back(dinamic->mesh);
	}

	// afegim el dinamic a la llista de objectes
	dinamic->idList = scene->dinamic_objects.size();
    scene->dinamic_objects.push_back(dinamic);
}

void PlayStage::addBlockInFront(eObjectName type)
{
	if(!hasBlock(type))
	{
		return;
	}

	Camera* camera = world->camera;
	Scene* scene = world->scenes[world->current_scene];

	// calculem les dades del nou BlocK
	Vector3 origin = camera->eye;
	Vector3 dir = camera->getRayDirection(Input::mouse_position.x, Input::mouse_position.y, world->window_width, world->window_height);
	Vector3 up = Vector3(0, 1, 0);
	Vector3 pos = RayPlaneCollision(Vector3(), up, origin, dir);

	// busque la mesh del Block
	EntityMesh* mesh = world->searchMesh(type);

	// creem el Block
	Block* block;
	switch (type)
	{
	case JEWEL:
		block = new Jewel(mesh, pos, DEFAULTSCENE);
		break;
	default:
		block = new Block(mesh, pos, type);
		break;
	}

	// si no existia la mesh guardem la nova mesh en la llista
	if (mesh == NULL) {
		world->meshs.push_back(block->mesh);
	}

	// afegim el Block a la llista de objectes
	block->idList = scene->static_objects.size();
	scene->static_objects.push_back(block);
}

void PlayStage::deleteBlock()
{
	Scene* scene = world->scenes[world->current_scene];
	for (int i = 0; i < scene->static_objects.size(); i++) {
		if (world->BlockPicked == scene->static_objects[i])
			scene->static_objects.erase(scene->static_objects.begin() + i);
	}
	for (int i = 0; i < scene->dinamic_objects.size(); i++) {
		if (world->BlockPicked == scene->dinamic_objects[i])
			scene->dinamic_objects.erase(scene->dinamic_objects.begin() + i);
	}
}

void PlayStage::editBlocks(Object* BlockPicked) {
	if (Input::isKeyPressed(SDL_SCANCODE_U)) BlockPicked->model.translate(Vector3(0, 1, 0));
	if (Input::isKeyPressed(SDL_SCANCODE_I)) BlockPicked->model.translate(Vector3(1, 0, 0));
	if (Input::isKeyPressed(SDL_SCANCODE_O)) BlockPicked->model.translate(Vector3(0, -1, 0));
	if (Input::isKeyPressed(SDL_SCANCODE_J)) BlockPicked->model.translate(Vector3(0, 0, 1));
	if (Input::isKeyPressed(SDL_SCANCODE_K)) BlockPicked->model.translate(Vector3(-1, 0, 0));
	if (Input::isKeyPressed(SDL_SCANCODE_L)) BlockPicked->model.translate(Vector3(0, 0, -1));
	if (Input::isKeyPressed(SDL_SCANCODE_N)) BlockPicked->model.rotate(0.01f, Vector3(0, 1, 0));
	if (Input::isKeyPressed(SDL_SCANCODE_M)) BlockPicked->model.rotate(-0.01f, Vector3(0, 1, 0));
}

void PlayStage::editSaw(Saw* SawPicked) {
	if (Input::isKeyPressed(SDL_SCANCODE_U)) SawPicked->center = SawPicked->center + Vector3(0, 1, 0);
	if (Input::isKeyPressed(SDL_SCANCODE_I)) SawPicked->center = SawPicked->center + Vector3(1, 0, 0);
	if (Input::isKeyPressed(SDL_SCANCODE_O)) SawPicked->center = SawPicked->center + Vector3(0, -1, 0);
	if (Input::isKeyPressed(SDL_SCANCODE_J)) SawPicked->center = SawPicked->center + Vector3(0, 0, 1);
	if (Input::isKeyPressed(SDL_SCANCODE_K)) SawPicked->center = SawPicked->center + Vector3(-1, 0, 0);
	if (Input::isKeyPressed(SDL_SCANCODE_L)) SawPicked->center = SawPicked->center + Vector3(0, 0, -1);
	if (Input::isKeyPressed(SDL_SCANCODE_Y)) SawPicked->rad += 10;
	if (Input::isKeyPressed(SDL_SCANCODE_H)) SawPicked->rad -= 10;
	if (Input::isKeyPressed(SDL_SCANCODE_N)) SawPicked->model_position.rotate(0.01f, SawPicked->model_position.topVector());
	if (Input::isKeyPressed(SDL_SCANCODE_M)) SawPicked->model_position.rotate(-0.01f, SawPicked->model_position.topVector());
}

// events
void PlayStage::onKeyDown( SDL_KeyboardEvent event )
{
	switch (idmode) 
	{
		case GAMEPLAY: {
			switch (event.keysym.sym) {
				case SDLK_ESCAPE:
					nextSatge = MENU;
					isComplite = true;
					break;
				case SDLK_3: idmode = EDIT; break;
				case SDLK_4: world->player->current_scene = (eScene( (world->current_scene+1) % world->scenes.size() )); break;
			}
			break;
		}
		case EDIT: {
			switch (event.keysym.sym) {
				case SDLK_ESCAPE:
					nextSatge = MENU;
					isComplite = true;
					break;
				case SDLK_1: addDinamicInFront(BOX); break;
				case SDLK_2:
					if (world->player->boxPicked == NULL)
						world->player->SelectBox(world->boxPicked);
					else
						world->player->LeaveBox();
					break;
				case SDLK_3: idmode = GAMEPLAY; break;
				case SDLK_4: world->player->current_scene = (eScene((world->current_scene+1) % world->scenes.size())); break;
				case SDLK_5: addBlockInFront(BLOCKLARGE); break;
				case SDLK_6: addBlockInFront(BLOCKLONG); break;
				case SDLK_7: addBlockInFront(BLOCKUNIT); break;
				case SDLK_8: world->editMap(); break;
				case SDLK_9: addDinamicInFront(SAWHUNTER); break;
				case SDLK_0: {
					if(world->BlockPicked == NULL || world->BlockPicked->oName != JEWEL)
						break;
					Jewel* jewel = (Jewel*) world->BlockPicked;

					// if per treure la jewel de la llista de win
					if(jewel->next_scene == WIN) 
						world->JewelsWin &= ~jewel->idMask;
					
					// cambiar de next_scene
					jewel->next_scene = (eScene( (jewel->next_scene+1) % SIZEOFSCENE ));

					// if per ficar la jewel de la llista de win
					if(jewel->next_scene == WIN) 
						world->JewelsWin |= jewel->idMask;
					std::cout << " the level of Jewel is \"" << TableSceneNames[jewel->next_scene].cName << "\"" << std::endl;
					break;
				}
				case SDLK_z: addBlockInFront(JEWEL); break;
				case SDLK_x: addBlockInFront(MUSHROOM); break;
				case SDLK_c: addBlockInFront(ROCK); break;
				case SDLK_v: addBlockInFront(WEED); break;
				case SDLK_b: addDinamicInFront(SAW); break;
				case SDLK_F3: isComplite = true; break;
				case SDLK_F4: SaveLevel(world->SaveScene()); break;
				case SDLK_F5: deleteBlock(); break;
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

eStageID PlayStage::whatNext()
{
	eStageID next = nextSatge;
	nextSatge = END;
	SDL_ShowCursor(true);
	return next;
}

// ------------------------------------ class: EndStage  ----------------------------------
EndStage::EndStage(World* w, Gui* g)
{
	idSatge = END;
	nextSatge = INTRO;
	needRender = true;

	if(w != NULL) setWorld(w);
	if(g == NULL)
		return;
	
	setGui(g);

	// creem els diferents elements
	guiElements.reserve(1);
	guiElements.push_back(gui->creatElement(TEXT, 0, 0, w_gui, h_gui, true));
}

void EndStage::RenderGame()
{
	RenderWorld();
}

void EndStage::Update(double elapsed_time)
{
	UpdateWorld(elapsed_time);
}

// events
void EndStage::onKeyDown( SDL_KeyboardEvent event )
{
	isComplite = true;
}

void EndStage::onMouseButtonDown( SDL_MouseButtonEvent event )
{

}
