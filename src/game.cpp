#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "fbo.h"
#include "shader.h"
#include "input.h"
#include "animation.h"
#include "stage.h"
#include "world.h"
#include "entity.h"
#include "loader.h"
#include "extra/bass.h"
#include "portability.h"

#include <cmath>

//some globals
Animation* anim = NULL;
FBO* fbo = NULL;

Game* Game::instance = NULL;

enum eGuiID {
    gButton,
    gTutorial,
    gEnd,

    SIZEOFGUIS,
};

Game::Game(int window_width, int window_height, SDL_Window* window)
{
	this->window_width = window_width;
	this->window_height = window_height;
	this->window = window;
	instance = this;
	must_exit = false;

	fps = 0;
	frame = 0;
	time = 0.0f;
	elapsed_time = 0.0f;

	// --- Audio ---
	#ifdef _WINDOWS_
	if (BASS_Init(-1, 44100, 0, 0, NULL) == false) //-1 significa usar el por defecto del sistema operativo
		std::cout << "Problemes a s'hora de inicialitzar sa llibraria BASS" << std::endl;
	#endif

	//OpenGL flags
	glEnable( GL_CULL_FACE ); //render both sides of every triangle
	glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer

	//carregar la configuracions
	InitCfg();
	InitCfgMesh();
	LoadCfg("data/Configuration.txt");
	printf("\n");
	
	//crear World i Gui
	world = new World(window_width,window_height);

	guis.reserve(SIZEOFGUIS);
	guis.push_back(new Gui("data/UI/Buttons.png", window_width, window_height));
	guis.push_back(new Gui("data/UI/Tutorial.png", window_width, window_height));
	guis.push_back(new Gui("data/UI/Final.png", window_width, window_height));

	// load levels
	LoadLeveols();
	
	//Create Stages
	idCS = eStageID::INTRO;

	stages.reserve(SIZEOFSTAGE);
	stages.push_back(new IntroStage(world, guis[gButton]));
	stages.push_back(new MenuStage(world,  guis[gButton]));
	stages.push_back(new TutorStage(NULL,  guis[gTutorial]));
	stages.push_back(new PlayStage(world, NULL));
	stages.push_back(new EndStage(NULL,  guis[gEnd]));
}

//what to do when the image has to be draw
void Game::render(void)
{
	//set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	stages[idCS]->Render();

	//render the FPS, Draw Calls, etc
	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);
}

void Game::update(double seconds_elapsed)
{
	idCS = stages[idCS]->passStage();
	if(idCS == EXIT) must_exit = true;
	toBlur = (idCS == PLAY)? false : true;

	stages[idCS]->Update(seconds_elapsed);
}

//Keyboard event handler (sync input)
void Game::onKeyDown( SDL_KeyboardEvent event )
{
	Stage* stage = stages[idCS];
	switch(event.keysym.sym)
	{
		case SDLK_F1: 
			Shader::ReloadAll(); 
			break; 
		case SDLK_F2: 
			LoadCfg("data/Configuration.txt");
			break; 
		default:
			stages[idCS]->onKeyDown( event );
			break;
	}
}

void Game::onKeyUp(SDL_KeyboardEvent event)
{
}

void Game::onGamepadButtonDown(SDL_JoyButtonEvent event)
{

}

void Game::onGamepadButtonUp(SDL_JoyButtonEvent event)
{

}

void Game::onMouseButtonDown( SDL_MouseButtonEvent event )
{
	stages[idCS]->onMouseButtonDown( event );
}

void Game::onMouseButtonUp(SDL_MouseButtonEvent event)
{
}

void Game::onMouseWheel(SDL_MouseWheelEvent event)
{
	world->player->Speed *= event.y > 0 ? 1.1 : 0.9;
}

void Game::onResize(int width, int height)
{
    std::cout << "window resized: " << width << "," << height << std::endl;
	// resize Game
	glViewport( 0,0, width, height );
	window_width = width;
	window_height = height;
	// resize World
	world->camera->aspect =  width / (float)height;
	world->window_width = window_width;
	world->window_height = window_height;
	// resize Guis
	Gui* gui;
	for(int i = 0; i < guis.size(); i++)
	{
		gui = guis[i];
		gui->setDimCamera(window_width, window_height);
	}
}

void Game::LoadLeveols()
{
	for(int i = 0; i < SIZEOFSCENE; i++)
	{
		if(strlen(TableSceneNames[i].filename) == 0)
			continue;
		world->LoadScene(LoadLevel(TableSceneNames[i].filename), (eScene)i);
	}
	world->changeScene(STARTLEVEL);
}
