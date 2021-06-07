#include "input.h"
#include "world.h"
#include "entity.h"

// ----------------------------------------- class: Scene -----------------------------------------
Scene::Scene(eScene n, Vector3 sp, bool sb) 
{
    name = n;
    spawn = sp;
    if(!sb)
        getSkybox("data/Skybox/sphere.obj", "data/Skybox/Night.png");
    EntityLight* lus = new EntityLight(Vector3(0, 200, 0), Vector3(-0.00785305, -0.472548, 0.88127), Vector3(1, 1, 1), .5);
    lights.push_back(lus);
}

void Scene::getSkybox(const char* fileSkybox, const char* fileSkyboxTex)
{
    skybox = new EntityMesh( SKYBOX );

    skybox->texture = new Texture();
 	skybox->texture->load(fileSkyboxTex);
	skybox->mesh = Mesh::Get(fileSkybox);
    skybox->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    skybox->color = Vector4(1, 1, 1, 1);
    skybox->model.rotate(180 * DEG2RAD, Vector3(0, 0, 1));
}

// ----------------------------------------- class: World -----------------------------------------
World::World( int window_width, int window_height ) 
{ 
    this->window_width = window_width;
	this->window_height = window_height;

    setCamera(window_width, window_height); 

    Scene* scene = new Scene(DEMO);
    scenes.push_back(scene);

    current_scene = DEMO;
    // A partir de aqui empezamos a cargar el mapa.
    Object* floor = new Floor();
    scenes[current_scene]->static_objects.push_back(floor);

    player = new Player();
    player->spawn = scene->spawn;
}

void World::setCamera( int window_width, int window_height )
{
    //create our camera
	camera = new Camera();
	camera->lookAt(Vector3(0.f,100.f, 100.f),Vector3(1.f,0.f,0.f), Vector3(0.f,1.f,0.f)); //position the camera and point to 0,0,0
	camera->setPerspective(70.f,window_width/(float)window_height,0.1f,10000.f); //set the projection, we want to be perspective
}

EntityMesh* World::searchMesh( eEntityName obj )
{
    EntityMesh* mesh;
    
    for (int id=0; id < meshs.size(); id++)
    {
        mesh = meshs[id];
        if( mesh->object == obj ) return mesh;
    }
    return NULL;
}

void World::SelectBox()
{
    boxPicked = NULL;
    if (player->boxPicked != NULL) 
        return;
    Scene* scene = scenes[0];

    Vector3 origin = camera->eye;
    Vector3 dir = camera->getRayDirection(Input::mouse_position.x, Input::mouse_position.y, window_width, window_height);
    Vector3 col, normal, pos;
    float distPicked, distObject;
    DinamicObject* object;
    Mesh* mesh; 
    
    for (int id=0; id < scene->dinamic_objects.size(); id++)
    {
        object = scene->dinamic_objects[id];

        pos = object->getPosition();

        if (object->mesh == NULL) continue;
        if (object->name != BOX) continue;

        mesh = object->mesh->mesh;

        if (!mesh->testRayCollision(object->model,origin,dir,col,normal,100)) 
            continue;

        if (boxPicked == NULL){
            boxPicked = object;
            continue;
        }
        // LLeva coses que se creen aqui
        distPicked = boxPicked->getPosition().distance(origin);
        distObject = object->getPosition().distance(origin);
        
        if (distPicked > distObject){
            boxPicked = object;
        }
    }   
}

void World::editMap()
{
    BlockPicked = NULL;

    Scene* scene = scenes[0]; // Hardcodejat

    Vector3 origin = camera->eye;
    Vector3 dir = camera->getRayDirection(Input::mouse_position.x, Input::mouse_position.y, window_width, window_height);
    Vector3 col, normal;
    Object* object;
    Mesh* mesh;

    for (int id = 0; id < scene->static_objects.size(); id++)
    {
        object = scene->static_objects[id];

        if (object->mesh == NULL) continue;

        mesh = object->mesh->mesh;

        if (!mesh->testRayCollision(object->model, origin, dir, col, normal))
            continue;

        BlockPicked = object;

        break;
    }
}

// lista de enums per trobar quin amb quin al carregar (ORDENAR PER eObjType DE skin.h!!!)
block2enums block2Info[SIZEOFOT] = {
    {PLAYER,ePlayer, NotUse},
    {FLOOR,efloor, NotUse},
    {BLOCKLARGE,eBLarge, BLARGE},
    {BLOCKLONG,eBLong, BLONG},
    {BLOCKUNIT,eBUnit, BUNIT},
    {BOX,eBox, NotUse}
};

// guardar i carregar Scenes
NameLevel TableSceneNames[SIZEOFSCENE] = {
    {"DEMO",DEMO},
    {"NIVELDELAVA",NIVELDELAVA}
};

Level* World::SaveScene()
{
    // crea level + nom per defecte
    Level* level = new Level{"DEFAULT"};

    Scene* scene = scenes[0]; // Hardcodejat

    // Skybox
    cfgMesh* cfgSB = cfgSkyboxCreat(scene->skybox->texture->filename.c_str());
    level->Skybox = *cfgSB;

    // Player
    DinamicObj* sPlayer = new DinamicObj{ePlayer, player->model.getTranslation() + Vector3(0,120,0)};
    level->player = *sPlayer;

    Object* object;
    eObjType type;
    // llista de Statics
    StaticObj sObjs[MAXOBJ];
    int max = 0;
    for (int id = 0; id < scene->static_objects.size() && id < MAXOBJ; id++)
    {
        object = scene->static_objects[id];
        // buscar el type
        for(int i = 0; i < sizeof(block2Info)/sizeof(block2enums); i++)
        {
            if(object->name != block2Info[i].entity) 
                continue;

            type = block2Info[i].type;
            break;
        }

        // afegir el elemnt a la llista
        level->sObjs[id] = *(new StaticObj{type, object->model.getTranslation(), object->model.getTranslation()}); // falta la rotacio
        max++;
    }
    level->numSObj = max;

    // llista de Dinamics
    DinamicObj dObjs[MAXOBJ];
    max = 0;
    for (int id = 0; id < scene->dinamic_objects.size() && id < MAXOBJ; id++)
    {
        object = scene->dinamic_objects[id];
        // buscar el type
        for(int i = 0; i < sizeof(block2Info)/sizeof(block2enums); i++)
        {
            if(object->name != block2Info[i].entity) 
                continue;

            type = block2Info[i].type;
            break;
        }
        
        // afegir el elemnt a la llista
        level->dObjs[id] = *(new DinamicObj{type, object->model.getTranslation() + Vector3(0,120,0)});
        max++;
    }
    level->numDObj = max;

    return level;
}

void World::LoadScene(Level* level)
{
    // Nom del nivell (no fa res de moment)
    eScene nameScene = DEFAULTSCENE;
    for (int i = 0; nameScene == DEFAULTSCENE && i < SIZEOFSCENE; i++)
    {
        // si es el element
        if(strstr(level->name, TableSceneNames[i].cName) != NULL) 
            nameScene = TableSceneNames[i].eName;
    }

    // player spawn
    if(level->player.type != ePlayer)
    {
        std::cout << "Error: spawn of player nt fount" << std::endl;
        return;
    }
    int id = scenes.size();
    scenes.push_back(new Scene(nameScene, level->player.pos, true));

    // skybox
    scenes[id]->getSkybox(level->Skybox.mesh, level->Skybox.texture);

    EntityMesh* m;
    eEntityName name;
    Object* obj;

    // llista statica
    StaticObj sobj;
    for(int i = 0; i < level->numSObj; i++)
    {
        sobj = level->sObjs[i];
        switch (sobj.type)
        {
            case eBLarge:
            case eBLong:
            case eBUnit:
            {
                if (block2Info[sobj.type].extra == NotUse)
                {
                    std::cout << "Error: type Static Uncorrect" << std::endl;
                    break;
                }
                name = block2Info[sobj.type].entity;
                m = searchMesh(name);
                obj = new Block(m, sobj.pos, block2Info[sobj.type].extra); // falta la rotacio implemetar en el constructor

                // afeges la mesh del obj si no estaba
                if( m == NULL )
                {
		            meshs.push_back(obj->mesh);
	            }
                break;
            }
            default:
                std::cout << "Error: Object Static Unespectet" << std::endl;
                break;
        }
    }
    // llista dinamica
    DinamicObj dobj;
    for(int i = 0; i < level->numDObj; i++)
    {
        dobj = level->dObjs[i];
        switch (dobj.type)
        {
            case eBox:
            {
                name = block2Info[sobj.type].entity;
                m = searchMesh(name);
                obj = new Box(m, dobj.pos);

                // afeges la mesh del obj si no estaba
                if( m == NULL )
                {
		            meshs.push_back(obj->mesh);
	            }
                break;
            }
            default:
                std::cout << "Error: Object Dinamic Unespectet" << std::endl;
                break;
        }
    }
    std::cout << " + Level Loaded" << std::endl; 
}
