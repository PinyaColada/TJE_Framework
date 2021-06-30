#ifndef STAGE_H
#define STAGE_H

#include "world.h"
#include "gui.h"		
#include "portability.h"

enum eStageID {
    INTRO,
    MENU,
    TUTORIAL,
    PLAY,
    END,

    SIZEOFSTAGE,
    EXIT = -1
};

enum eModeID{
    EDIT,
    GAMEPLAY,
};

// ------------------------------------ class: Stage  --------------------------------------
class Stage 
{
public:
    eModeID idmode = GAMEPLAY;
    eStageID idSatge;
    eStageID nextSatge;

    World* world = NULL;
    Gui* gui = NULL;

    bool mouse_locked = true; // tells if the mouse is locked (not seen)
    bool isComplite = false;
    bool needRender = false;		
    bool isWorld = false;		
    bool isGui = false;

    std::vector<sElementGui> guiElements;

    void Render();
    virtual void RenderGame() = 0;		
    void RenderGui();

    virtual void Update(double elapsed_time) = 0;

    void RenderWorld();
    void UpdateWorld(double elapsed_time, bool isMove = true);

    void setWorld(World* w) { world = w; isWorld = true; };
    void setGui(Gui* g) { gui = g; isGui = true; };

    void updateMouse();

    // events
    virtual void onKeyDown( SDL_KeyboardEvent event ) = 0;
    virtual void onMouseButtonDown( SDL_MouseButtonEvent event ) = 0;

    virtual void onPressButton(eElementsGui type) = 0;

    eStageID passStage();
    virtual eStageID whatNext() = 0;
};

// ------------------------------------ class: IntroStage  ----------------------------------
class IntroStage : public Stage
{
public:

    IntroStage(World* w = NULL, Gui* g = NULL);

    sElementGui* title;
    float time = 0.0f;
    Vector2 center;

    void RenderGame();

    void Update(double elapsed_time);

    // events
    void onKeyDown( SDL_KeyboardEvent event );
    void onMouseButtonDown( SDL_MouseButtonEvent event );

    void onPressButton(eElementsGui type);

    eStageID whatNext();
};

// ------------------------------------ class: MenuStage  ----------------------------------
class MenuStage : public Stage
{
public:

    MenuStage(World* w = NULL, Gui* g = NULL);

    void RenderGame();

    void Update(double elapsed_time);

    // events
    void onKeyDown( SDL_KeyboardEvent event );
    void onMouseButtonDown( SDL_MouseButtonEvent event );

    void onPressButton(eElementsGui type);

    eStageID whatNext();
};

// ------------------------------------ class: TutorStage  ----------------------------------
class TutorStage : public Stage
{
public:

    TutorStage(World* w = NULL, Gui* g = NULL);

    void RenderGame(){};

    void Update(double elapsed_time){};

    // events
    void onKeyDown( SDL_KeyboardEvent event );
    void onMouseButtonDown( SDL_MouseButtonEvent event ){};

    void onPressButton(eElementsGui type){};

    eStageID whatNext() {return nextSatge;};
};

// ------------------------------------ class: PlayStage  ----------------------------------
class PlayStage : public Stage
{
public:
    PlayStage(World* w = NULL, Gui* g = NULL);

    float timeCounter = 0.0f;
    float coolDownCounter = 0.0f;

    // --- Audio ---
    #ifdef _WINDOWS_
    Audio* windSound;
    Audio* stopTimeAudio;

    bool isWindSounding = false;
    #endif

    void RenderGame();

    void Update(double elapsed_time);

    void addDinamicInFront(eObjectName type);
    void addBlockInFront(eObjectName type);

    void deleteBlock();

    void editBlocks(Object* BlockPicked);
    void editSaw(Saw* SawPicked);

    // events
    void onKeyDown( SDL_KeyboardEvent event );
    void onMouseButtonDown( SDL_MouseButtonEvent event );

    void onPressButton(eElementsGui type){};

    eStageID whatNext();
};

// ------------------------------------ class: EndStage  ----------------------------------
class EndStage : public Stage
{
public:

    EndStage(World* w = NULL, Gui* g = NULL);

    void RenderGame(){};

    void Update(double elapsed_time){};

    // events
    void onKeyDown( SDL_KeyboardEvent event );
    void onMouseButtonDown( SDL_MouseButtonEvent event );

    void onPressButton(eElementsGui type){};

    eStageID whatNext() {return nextSatge;};
};

#endif 