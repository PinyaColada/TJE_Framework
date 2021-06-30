#ifndef GUI_H
#define GUI_H

#include "camera.h"
#include "texture.h"
#include "shader.h"

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

    int window_width;
    int window_height;

    // Metodos
    Gui(const char *TexName, int width, int height);

    bool render(sElementGui elm);

    void renderElement(float x, float y, float w, float h, Vector4 range, bool flipuvs);
    bool renderButton(float x, float y, float w, float h, Vector4 range, bool flipuvs);

    void setDimCamera(int width, int height);

    sElementGui creatElement(eElementsGui type, float x, float y, float w, float h, bool center = true);
};

#endif 