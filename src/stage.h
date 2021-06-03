#ifndef STAGE_H
#define STAGE_H

#include "world.h"

enum eStageID {
    PLAY
};

enum eModeID{
    EDIT,
    GAMEPLAY,
};

//------------------------------------ class: Stage  --------------------------------------
class Stage 
{
public:
    eModeID idmode = EDIT;
    eStageID idSatge;
    World* world = NULL;
    bool mouse_locked = true; //tells if the mouse is locked (not seen)

    virtual void Render() = 0;

    virtual void Update(double elapsed_time) = 0;

    void setWorld(World* w) { world = w; };

    void updateMouse();

    //events
    virtual void onKeyDown( SDL_KeyboardEvent event ) = 0;
    virtual void onMouseButtonDown( SDL_MouseButtonEvent event ) = 0;
};

//------------------------------------ class: PlayStage  ----------------------------------
class PlayStage : public Stage
{
public:

    PlayStage();

    void Render();

    void Update(double elapsed_time);

    void AddBoxInFront();
    void SelectObject();

    //events
    void onKeyDown( SDL_KeyboardEvent event );
    void onMouseButtonDown( SDL_MouseButtonEvent event );
};

#endif 