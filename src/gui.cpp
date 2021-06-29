#include "gui.h"
#include "mesh.h"
#include "input.h"

// taula amb el flipuvs i el range de cada element
sRangeGui TableOfRanges[SIZEOFEG] = {
    {false, Vector4(0.0, 0.0, 0.5, 0.5)},
    {false, Vector4(0.0, 0.5, 0.5, 0.5)},
    {false, Vector4(0.5, 0.0, 0.5, 0.5)},
    {false, Vector4(0.5, 0.5, 0.5, 0.5)},
    {false, Vector4(0.0, 0.0, 1.0, 1.0)}
};

bool isIn(Vector2 p, float x, float y, float w, float h)
{
    float min_x = x - w * 0.5;
    float min_y = y - h * 0.5;
    float max_x = x + w * 0.5;
    float max_y = y + h * 0.5;

    return p.x > min_x && p.y > min_y && p.x < max_x && p.y < max_y;
}

bool hasButton(eElementsGui type)
{
    switch (type)
    {
        case GAME:
        case LOAD:
        case EXIT:
        case SAVE:
            return true;
        
        default:
            return false;
    }
}

// ------------------------------------ class: Gui  --------------------------------------
Gui::Gui(const char *TexName, int width, int height)
{
    shader = Shader::Get("data/shaders/basic.vs", "data/shaders/gui.fs");
    spells = Texture::Get(TexName);
    setDimCamera(width, height);
}

bool Gui::render(sElementGui elm)
{
    sRangeGui info = TableOfRanges[elm.type];
    // boto
    if(hasButton(elm.type))
        return renderButton(elm.pos.x, elm.pos.y, elm.dim.x, elm.dim.y, info.range, info.flipuvs);
    // no boto
    renderElement(elm.pos.x, elm.pos.y, elm.dim.x, elm.dim.y, info.range, info.flipuvs);
    return false;
}

void Gui::renderElement(float x, float y, float w, float h, Vector4 range, bool flipuvs)
{
    // crea el quad
    Mesh quad;
    quad.createQuad(x, y, w, h, flipuvs);
    Matrix44 quadModel;

    // actualitza el shader
    shader->enable();

    shader->setUniform("u_color", Vector4(1,1,1,1));
    shader->setUniform("u_model", quadModel);
    shader->setUniform("u_viewprojection", cam.viewprojection_matrix);
    shader->setTexture("u_texture", spells, 0);
    shader->setUniform("u_tex_range", range);
    shader->setUniform("u_texture_tiling", 1.0f);
    quad.render(GL_TRIANGLES);

    shader->disable();
}

bool Gui::renderButton(float x, float y, float w, float h, Vector4 range, bool flipuvs)
{
    // crea el quad
    Mesh quad;
    quad.createQuad(x, y, w, h, flipuvs);
    Matrix44 quadModel;

    // calcul de si esta pulsat
    bool hover = isIn(Input::mouse_position, x, y, w, h);
    bool pressed = Input::isMousePressed(SDL_BUTTON_LEFT); // crec que seria aixi

    // actualitza el shader
    shader->enable();

    shader->setUniform("u_color", Vector4(1,1,1,1));
    shader->setUniform("u_model", quadModel);
    shader->setUniform("u_viewprojection", cam.viewprojection_matrix);
    shader->setTexture("u_texture", spells, 0);
    shader->setUniform("u_tex_range", range);
    shader->setUniform("u_texture_tiling", 1.0f);
    quad.render(GL_TRIANGLES);

    shader->disable();
    if(hover) printf("HOVER\n");
    return hover && pressed;
}

void Gui::setDimCamera(int width, int height)
{
    cam.setOrthographic(0, width, height, 0, -1, 1);
}