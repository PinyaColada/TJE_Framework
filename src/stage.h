#ifndef STAGE_H
#define STAGE_H

#include "world.h"

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
    bool mouse_locked = true; // tells if the mouse is locked (not seen)
    bool isComplite = false;

    virtual void Render() = 0;

    virtual void Update(double elapsed_time) = 0;

    void setWorld(World* w) { world = w; };

    void updateMouse();

    // events
    virtual void onKeyDown( SDL_KeyboardEvent event ) = 0;
    virtual void onMouseButtonDown( SDL_MouseButtonEvent event ) = 0;

    eStageID passStage();
};

// ------------------------------------ class: IntroStage  ----------------------------------
class IntroStage : public Stage
{
public:

    IntroStage();

    void Render();

    void Update(double elapsed_time);

    // events
    void onKeyDown( SDL_KeyboardEvent event );
    void onMouseButtonDown( SDL_MouseButtonEvent event );
};

// ------------------------------------ class: MenuStage  ----------------------------------
class MenuStage : public Stage
{
public:

    MenuStage();

    void Render();

    void Update(double elapsed_time);

    // events
    void onKeyDown( SDL_KeyboardEvent event );
    void onMouseButtonDown( SDL_MouseButtonEvent event );
};

// ------------------------------------ class: PlayStage  ----------------------------------
class PlayStage : public Stage
{
public:

    PlayStage();

    float timeCounter = 0.0f;
    float coolDownCounter = 0.0f;

    void Render();

    void Update(double elapsed_time);

    void addDinamicInFront(eObjectName type);
    void addBlockInFront(eObjectName type);

    void deleteBlock();

    void editBlocks(Object* BlockPicked);
    void editSaw(Saw* SawPicked);

    // events
    void onKeyDown( SDL_KeyboardEvent event );
    void onMouseButtonDown( SDL_MouseButtonEvent event );
};

// ------------------------------------ class: EndStage  ----------------------------------
class EndStage : public Stage
{
public:

    EndStage();

    void Render();

    void Update(double elapsed_time);

    // events
    void onKeyDown( SDL_KeyboardEvent event );
    void onMouseButtonDown( SDL_MouseButtonEvent event );
};

#endif 