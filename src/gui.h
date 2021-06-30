#ifndef GUI_H
#define GUI_H

#include "camera.h"
#include "texture.h"
#include "shader.h"
#include "framework.h"

enum eElementsGui{
    CONTINUE,
    LEAVE,
    START,
    TITLE,
    TEXT,

    SIZEOFEG
};

// --- Strucks ---
struct sElementGui{
    eElementsGui type;
    Vector2 pos;    //[x,y]
    Vector2 dim;    //[w,h]
    bool isCenter;
    bool allWindow;
    bool isBlock;
};
struct sRangeGui{
    bool flipuvs;
    Vector4 range;  //[x,y,w,h]
};

// ------------------------------------ class: Gui  --------------------------------------
class Gui
{
public:
    // Atributos
    Camera cam;
    Texture* spells;
    Shader* shader;

    Audio* sound;

    int window_width;
    int window_height;
    bool hasSave;

    // Metodos
    Gui(const char *TexName, int width, int height, bool Save = true);

    bool render(sElementGui elm);

    void renderElement(float x, float y, float w, float h, Vector4 range, bool flipuvs);
    bool renderButton(float x, float y, float w, float h, Vector4 range, bool flipuvs, bool block);

    void setDimCamera(int width, int height);

    sElementGui creatElement(eElementsGui type, float x, float y, float w, float h, bool allwindow = false, bool center = true, bool block = false);
};

#endif 