#include "gui.h"
#include "mesh.h"
#include "input.h"

// taula amb el flipuvs i el range de cada element
sRangeGui TableOfRanges[SIZEOFEG] = {
    {false, Vector4(0, 0.0, 0.5, 0.2)},
    {false, Vector4(0, 0.2, 0.5, 0.2)},
    {false, Vector4(0, 0.4, 0.5, 0.2)},
    {false, Vector4(0, 0.6, 1.0, 0.2)},
    {false, Vector4(0, 0.0, 1.0, 1.0)}
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
        case CONTINUE:
        case LEAVE:
        case START:
            return true;
        
        default:
            return false;
    }
}

// ------------------------------------ class: Gui  --------------------------------------
Gui::Gui(const char *TexName, int width, int height, bool Save)
{
    shader = Shader::Get("data/shaders/basic.vs", "data/shaders/gui.fs");
    spells = Texture::Get(TexName);
    setDimCamera(width, height);

    hasSave = Save;
}

bool Gui::render(sElementGui elm)
{
    sRangeGui info = TableOfRanges[elm.type];
    Vector2 pos = (elm.isCenter)? elm.pos + Vector2(window_width * 0.5, window_height * 0.5) : elm.pos;
    Vector2 dim = (elm.allWindow)? Vector2(window_width, window_height) : elm.dim;

    // boto
    if(hasButton(elm.type))
        return renderButton(pos.x, pos.y, dim.x, dim.y, info.range, info.flipuvs, elm.isBlock);
    // no boto
    renderElement(pos.x, pos.y, dim.x, dim.y, info.range, info.flipuvs);
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

bool Gui::renderButton(float x, float y, float w, float h, Vector4 range, bool flipuvs, bool block)
{
    // crea el quad
    Mesh quad;
    quad.createQuad(x, y, w, h, flipuvs);
    Matrix44 quadModel;

    // calcul de si esta pulsat
    bool hover = isIn(Input::mouse_position, x, y, w, h);
    bool pressed = Input::isMousePressed(SDL_BUTTON_LEFT); // crec que seria aixi

    range.x = (hover || block)? range.x + range.z : range.x;
    Vector4 color = block? Vector4(0.5,0.5,0.5,0.8) : Vector4(1,1,1,1);

    // actualitza el shader
    shader->enable();

    shader->setUniform("u_color", color);
    shader->setUniform("u_model", quadModel);
    shader->setUniform("u_viewprojection", cam.viewprojection_matrix);
    shader->setTexture("u_texture", spells, 0);
    shader->setUniform("u_tex_range", range);
    shader->setUniform("u_texture_tiling", 1.0f);
    quad.render(GL_TRIANGLES);

    shader->disable();
    return hover && pressed && !block;
}

void Gui::setDimCamera(int width, int height)
{
    window_width = width;
    window_height = height;

    cam.setOrthographic(0, window_width, window_height, 0, -1, 1);
}

sElementGui Gui::creatElement(eElementsGui type, float x, float y, float w, float h, bool allwindow, bool center, bool block)
{
    return {type, Vector2(x, y), Vector2(w, h), center, allwindow, block && !hasSave};
}
