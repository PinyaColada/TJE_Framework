#include "entity.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "camera.h"

// ----------------------------------------- class: Entity -----------------------------------------
Entity::Entity(){}

Vector3 Entity::getPosition() { return model.getTranslation(); } 
Vector3 Entity::getDir() { return model.frontVector(); }

// ----------------------------------------- class: EntityLight -------------------------------	
EntityLight::EntityLight(Vector3 pos, Vector3 target, Vector3 color, float intensity) {
    model.setTranslation(pos);
    model.setFrontAndOrthonormalize(target);
    this->color = color;
    this->intensity = intensity;
}

// ----------------------------------------- class: EntityMesh -------------------------------------
EntityMesh::EntityMesh(eObjectName obj, cfgMesh* cfgM)
{
    eName = MESH;
    object = obj;

    if(cfgM == NULL) return;

    texture = new Texture();
 	texture->load(cfgM->texture);
	mesh = Mesh::getMeshAndBounding(cfgM->mesh, cfgM->bounding);
	shader = Shader::Get(cfgM->vsf, cfgM->psf);
    color = cfgM->color;
}

void EntityMesh::render(Camera* camera)
{
    // enable shader and pass uniforms	
    Vector3 ambient_light(0.5411, 0.7607, 0.9019);
    Vector3 light_color(1, 1, 1);
    float intensity = 0.25f;

    shader->enable();
    shader->setUniform("u_color", color);
    shader->setUniform("u_model", model);
    shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
    shader->setTexture("u_texture", texture, 0);

    shader->setUniform("u_light_color", light_color);
    shader->setUniform("u_light_intensity", intensity);

    shader->setUniform("u_ambient_light", ambient_light);
    shader->setUniform("u_is_time_stopped", isTimeStopped);

    // render the mesh using the shader	
    mesh->render(GL_TRIANGLES);

    // disable the shader after finishing rendering	
    shader->disable();
}


void EntityMesh::render(Camera* camera, std::vector<EntityLight*> lights)
{
    Vector3 ambient_light(0.5411, 0.7607, 0.9019);
    // enable shader and pass uniforms	
    shader->enable();
    shader->setUniform("u_color", color);
    shader->setUniform("u_model", model);
    shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
    shader->setTexture("u_texture", texture, 0);

    shader->setUniform("u_light_color", lights[0]->color);
    shader->setUniform("u_light_pos", lights[0]->getPosition());
    shader->setUniform("u_light_target", lights[0]->getDir());
    shader->setUniform("u_light_intensity", lights[0]->intensity);

    shader->setUniform("u_ambient_light", ambient_light);
    shader->setUniform("u_is_time_stopped", isTimeStopped);

    // render the mesh using the shader	
    mesh->render(GL_TRIANGLES);

    // disable the shader after finishing rendering	
    shader->disable();
}

void EntityMesh::update(float dt)
{

}

// ----------------------------------------- class: Object -------------------------------------

void Object::render(Camera* camera, std::vector<EntityLight*> lights)
{
    if (mesh == NULL) return;
    mesh->model = this->model;
    mesh->render(camera, lights);
}

void Object::render(Camera* camera)
{
    if(mesh == NULL) return;
    mesh->model = this->model;
    mesh->render(camera);
}

// ----------------------------------------- class: Floor -------------------------------
Floor::Floor()
{
    eName = OBJECT;
    oName = FLOOR;

    cfgMesh* cfgM = getCfgMesh(FLOOR);

    // prova de errors
    if (cfgM == NULL)
        std::cout << "Error: Config not found" << std::endl;

    // si no existeix la mesh
    mesh = new EntityMesh(FLOOR, cfgM);
}

// ----------------------------------------- class: Block -------------------------------
Block::Block(EntityMesh* m, Vector3 pos, eObjectName type)
{
    Init(m, pos, type);
}

void Block::Init(EntityMesh* m, Vector3 pos, eObjectName type)
{
    eName = OBJECT;
    oName = type; 
    model.setTranslation(pos);

    // si existeix la mesh
    if (m != NULL) {
        mesh = m;
        return;
    }

    cfgMesh* cfgM = getCfgMesh(type);

    // prova de errors
    if (cfgM == NULL)
        std::cout << "Error: Config not found" << std::endl;

    // si no existeix la mesh
    mesh = new EntityMesh(oName, cfgM);
}

// ----------------------------------------- class: Jewel -------------------------------
Jewel::Jewel(EntityMesh* m, Vector3 pos, eScene ns)
{
    Init(m, pos, JEWEL);

    next_scene = ns;
    idMask = 1<<numJewels;
    numJewels++;
}

void Jewel::render(Camera* camera, std::vector<EntityLight*> lights)
{
    if (mesh == NULL) return;
    mesh->color = colorJewel(next_scene);
    mesh->model = this->model;
    mesh->render(camera, lights);
}

void Jewel::render(Camera* camera)
{
    if(mesh == NULL) return;
    mesh->color = colorJewel(next_scene);
    mesh->model = this->model;
    mesh->render(camera);
}


// ----------------------------------------- class: DinamicObject -------------------------------------
void DinamicObject::setCfgD(eType type)
{
    // Busca la configuracio
    cfgGeneric* cfg = getCfg(type);

    if (cfg != NULL && ((cfgGeneric*)cfg)->type == type)
        cfgD = (cfgDinamic*) cfg;
    else
        cfgD = new cfgDinamic();
}

bool DinamicObject::onCollision(Object* object, Vector3 position, float speed, Vector3& target)
{
    // calculamos la colision de 1 objeto
    Vector3 coll, norm;
    float target_y = target.y;
    Vector3 centreObject = Vector3(0,cfgD->radius,0);
    Vector3 centre = position + centreObject;

    if (object->mesh == NULL || !object->mesh->mesh->testSphereBoundingCollision( object->model, centre, cfgD->radius, coll, norm))
        return false;
    
    // actualizamos el objetivo
    Vector3 push_away = normalize(coll - position) * speed;
    target = position - push_away;
    
    // comprovamos si el eje y es correcto
    target.y = target_y;

    return true;
}

bool DinamicObject::hasGround(Object* object, Vector3 position)
{
    Vector3 coll, norm;

    if (object->mesh->mesh->testRayBoundingCollision( object->model, position, Vector3(0, -1, 0), coll, norm, cfgD->radius + cfgD->margen))
        return true;
    
    return false;
}

float DinamicObject::minimHeight(Object* object, Vector3 position, float lastMin)
{
    Vector3 coll, norm;
    float minim = lastMin;

    if (object->mesh->mesh->testRayBoundingCollision( object->model, position, Vector3(0, -1, 0), coll, norm)){
        if ((coll.y + cfgD->margen > minim)){
            minim = coll.y + cfgD->margen;
        }
    }
    return minim;
}

void DinamicObject::respawn()
{
    model.setTranslation(spawn);
    physic->vel.y = 0;
}

// ----------------------------------------- class: Box -------------------------------
Box::Box(EntityMesh* m, Vector3 pos)
{
    eName = OBJECT;
    oName = BOX;
    spawn = pos;
    setCfgD(dinamicsBox);
    physic = new Physics(physicsBox);

    // Busca la configuracio
    cfgGeneric* cfg = getCfg(box);

    if (cfg != NULL && ((cfgGeneric*)cfg)->type == box)
        cfgB = (cfgBox*) cfg;
    else
        cfgB = new cfgBox();

    respawn();

    // si existeix la mesh
    if( m != NULL ){
        mesh = m;
        return;
    }

    cfgMesh* cfgM = getCfgMesh(BOX);

    // prova de errors
    if (cfgM == NULL)
        std::cout << "Error: Config not found" << std::endl;

    // si no existeix la mesh
    mesh = new EntityMesh(BOX, cfgM);
}

void Box::move(Vector3 dir, float elapsed_time, std::vector<Object*> static_objects, std::vector<DinamicObject*> dinamic_objects)
{
    // calculem la posicio
    Vector3 position = getPosition();
    Vector3 target = Vector3();

    // comprovem que la caixa no estigui agafada
    if (isCatch)
        return;
    
    // donem valors inicials
    Object* object;
    isFalling = true;
    float minim_y = -1000;

    // for para static_objects
    for (int i = 0; i < static_objects.size(); i++)
    {
        object = static_objects[i];
        if (!hasCollition(object->oName))
            continue;
        if(!isFalling)
            break;
        if(hasGround(object, position)){
            isFalling = false;
            continue;
        }
        minim_y = minimHeight(object, position, minim_y);
    }

    // for para dinamic_objects
    for (int i = 0; i < dinamic_objects.size(); i++)
    {
        object = dinamic_objects[i];
        if(!isFalling)
            break;
        if(idList == object->idList)
            continue;
        if(hasGround(object, position)){
            isFalling = false;model.setTranslation(target);
        }
        minim_y = minimHeight(object, position, minim_y);
    }

    // donem el valor minim
    physic->min_y = minim_y;

    // calculem el target
    target = physic->updateMove(elapsed_time, position, isFalling);

    // comprovem si esta per sota de la altura minim
    if(target.y < cfgD->dead_y){
        respawn();
        return;
    }
    

    // aplicamos el movimiento
    model.setTranslation(target);
}

void Box::movePicked(Matrix44 player, std::vector<Object*> static_objects, std::vector<DinamicObject*> dinamic_objects)
{
    Vector3 pos = getPosition();
    Vector3 playerPos = player.getTranslation();

    // calcul del moviment ideal
    Vector3 dir = player.frontVector();

    float h = cfgB->max_h * (clamp(dir.y, -0.6, 0.7) + 0.6);
    h = clamp(h, cfgB->min_h, cfgB->max_h);

    dir = Vector3(dir.x, 0, dir.z).normalize();

    // aplica la rotacio
    Vector3 rot = dir;

    // calcul del target
    Vector3 target = playerPos + Vector3(0,h,0) + cfgB->distPicked * dir;
    dir = target - pos;
    double modul = dir.length();
    // proba de errors
    if(modul == 0)
    {
        model.setTranslation(target);
        model.setFrontAndOrthonormalize(rot);
        return;
    }

    // calcul de variables pels fors
    dir = dir.normalize();
    Object* object;
    Vector3 coll, norm;
    float rad = cfgD->radius;
    Vector3 center = pos + Vector3(0,rad,0);
    float minim_y = cfgB->min_h + playerPos.y;

    // for para static_objects
    for (int i = 0; i < static_objects.size() && modul != 0; i++)
    {
        object = static_objects[i];
        if (!hasCollition(object->oName))
            continue;
        minim_y = minimHeight(object, pos, minim_y);
        if (!object->mesh->mesh->testRayBoundingCollision( object->model, center, dir, coll, norm, modul))
            continue;

        modul = (coll-center).length() - rad;
    }
    // for para dinamic_objects
    for (int i = 0; i < dinamic_objects.size() && modul != 0; i++)
    {
        object = dinamic_objects[i];
        if(object->idList == idList)
            continue;
        minim_y = minimHeight(object, pos, minim_y);
        if (!object->mesh->mesh->testRayBoundingCollision( object->model, center, dir, coll, norm, modul))
            continue;

        modul = (coll-center).length() - rad;
    }
    // calcula la minima y
    target = pos + dir * modul;
    target.clampY(minim_y, 1000);

    // calcula la nova rotacio
    rot = target - playerPos;
    rot = Vector3(rot.x, 0, rot.z).normalize();

    // aplica el canvi
    model.setTranslation(target);
    model.setFrontAndOrthonormalize(rot);

}

// ----------------------------------------- class: Player -------------------------------
Player::Player()
{
    setCfgD(dinamicsPlayer);
    physic = new Physics(physicsPlayer);
    eName = OBJECT;
    oName = PLAYER;

    pickedJewel = 0;

    // Busca la configuracio
    cfgGeneric* cfg = getCfg(player);

    if (cfg != NULL && ((cfgGeneric*)cfg)->type == player)
        cfgP = (cfgPlayer*) cfg;
    else
        cfgP = new cfgPlayer();

    Speed = cfgP->minSpeed;
}

void Player::move(Vector3 dir, float elapsed_time, std::vector<Object*> static_objects, std::vector<DinamicObject*> dinamic_objects)
{
    // calculamos el target idial
    Vector3 position = getPosition();
    float speed = elapsed_time * Speed;
    Vector3 target = position + dir * speed;

    // comprovem si esta per sota de la altura minim
    if(target.y < cfgD->dead_y){
        isDead = true;
        return;
    }

    // calculamos las coliciones
    Object* object;
    isFalling = true;
    float minim_y = -1000;
    bool next = false;

    // for para static_objects
    for (int i = 0; !next && i < static_objects.size(); i++)
    {
        object = static_objects[i];
        if (!hasCollition(object->oName))
            continue;
        if(onCollision(object, position, speed, target))
        {
            // switch per comprobar xocs amb objectes amb especific
            switch (object->oName)
            {
                case JEWEL:
                {
                    Jewel* jwl = (Jewel*)object;

                    // descartem casos invalits
                    if(jwl->next_scene == DEFAULTSCENE)
                        break;
                    // cas de WIN
                    else if(jwl->next_scene == WIN)
                    {
                        pickedJewel |= jwl->idMask;
                        current_scene = DEMO;
                    }
                    // resta de casos
                    else
                        current_scene = jwl->next_scene;

                    next = true;
                    break;
                }
                default:
                    break;
            }
            continue;
        }
        if(hasGround(object, position)){
            isFalling = false;
            continue;
        }
        minim_y = minimHeight(object, position, minim_y);
    }
    // for para dinamic_objects
    for (int i = 0; !next && i < dinamic_objects.size(); i++)
    {
        object = dinamic_objects[i];
        if(onCollision(object, position, speed, target))
            continue;
        if(hasGround(object, position)){
            isFalling = false;
            continue;
        }
        minim_y = minimHeight(object, position, minim_y);
    }

    // donem el valor minim
    physic->min_y = minim_y;

    // calculem el target
    target = physic->updateMove(elapsed_time, target, isFalling);

    // aplicamos el movimiento
    model.setTranslation(target);

    // mover la boxPicked
    if(boxPicked != NULL)
        boxPicked->movePicked(model, static_objects, dinamic_objects);
}

void Player::SelectBox(DinamicObject* picked)
{
    if (picked == NULL || boxPicked != NULL)
        return;

    // codigo agafar Box
    boxPicked = picked;
    boxPicked->isCatch = false;
}

void Player::LeaveBox()
{
    if (boxPicked == NULL) 
        return;

    // codigo de soltar Box
    boxPicked->isCatch = false;
    boxPicked->physic->vel.y = 0;
    boxPicked = NULL;
}
