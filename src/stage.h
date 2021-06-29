#ifndef STAGE_H
#define STAGE_H

#include "world.h"
#include "gui.h"		
#include "portability.h"

enum eStageID {
    INTRO,
    MENU,
    PLAY,
    END,

    SIZEOFSTAGE
};

enum eModeID{
    EDIT,
    GAMEPLAY,
};

// ------------------------------------ class: Stage  --------------------------------------
class Stage 
{
public:
    eModeID idmode = EDIT;
    eStageID idSatge;
    eStageID nextSatge;

    World* world = NULL;
    Gui* gui = NULL;

    bool mouse_locked = true; // tells if the mouse is locked (not seen)
    bool isComplite = false;
    bool needRender = false;		
    bool isWorld = false;		
    bool isGui = false;

    void Render();
    virtual void RenderGame() = 0;		
    virtual void RenderGui() = 0;

    virtual void Update(double elapsed_time) = 0;

    void setWorld(World* w) { world = w; isWorld = true; };
    void setGui(Gui* g) { gui = g; isGui = true; };

    void updateMouse();

    // events
    virtual void onKeyDown( SDL_KeyboardEvent event ) = 0;
    virtual void onMouseButtonDown( SDL_MouseButtonEvent event ) = 0;

    virtual void onPressButton(eElementsGui type) = 0;

    eStageID passStage();
};

// ------------------------------------ class: IntroStage  ----------------------------------
class IntroStage : public Stage
{
public:

    IntroStage();

    void RenderGame();
    void RenderGui();

    void Update(double elapsed_time);

    // events
    void onKeyDown( SDL_KeyboardEvent event );
    void onMouseButtonDown( SDL_MouseButtonEvent event );

    void onPressButton(eElementsGui type);
};

// ------------------------------------ class: MenuStage  ----------------------------------
class MenuStage : public Stage
{
public:

    MenuStage();

    void RenderGame();
    void RenderGui();

    void Update(double elapsed_time);

    // events
    void onKeyDown( SDL_KeyboardEvent event );
    void onMouseButtonDown( SDL_MouseButtonEvent event );

    void onPressButton(eElementsGui type);
};

// ------------------------------------ class: PlayStage  ----------------------------------
class PlayStage : public Stage
{
public:

    PlayStage();

    float timeCounter = 0.0f;
    float coolDownCounter = 0.0f;

    #ifdef _WINDOWS_
    Audio* stopTimeAudio;
    #endif

    void RenderGame();
    void RenderGui();

    void Update(double elapsed_time);

    void addDinamicInFront(eObjectName type);
    void addBlockInFront(eObjectName type);

    void deleteBlock();

    void editBlocks(Object* BlockPicked);
    void editSaw(Saw* SawPicked);

    // events
    void onKeyDown( SDL_KeyboardEvent event );
    void onMouseButtonDown( SDL_MouseButtonEvent event );

    void onPressButton(eElementsGui type);
};

// ------------------------------------ class: EndStage  ----------------------------------
class EndStage : public Stage
{
public:

    EndStage();

    void RenderGame();
    void RenderGui();

    void Update(double elapsed_time);

    // events
    void onKeyDown( SDL_KeyboardEvent event );
    void onMouseButtonDown( SDL_MouseButtonEvent event );

    void onPressButton(eElementsGui type);
};

#endif 