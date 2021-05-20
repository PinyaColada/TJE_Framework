#ifndef STAGE_H
#define STAGE_H

#include "world.h"

enum eStageID {
    PLAY
};

extern float mouse_speed;

//------------------------------------ class: Stage  --------------------------------------
class Stage 
{
public:
    eStageID idSatge;
    World* world = NULL;
    bool mouse_locked = true; //tells if the mouse is locked (not seen)

    virtual void Render() = 0;

    virtual void Update(double elapsed_time) = 0;

    void setWorld(World* w) { world = w; };

    void updateMouse();
};

//------------------------------------ class: PlayStage  ----------------------------------
class PlayStage : public Stage
{
public:

    PlayStage();

    void Render();

    void Update(double elapsed_time);

    void AddCuboInFront();
    void SelectObject();
};

#endif 