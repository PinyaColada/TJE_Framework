#include "input.h"
#include "world.h"
#include "entity.h"

// ----------------------------------------- class: Scene -----------------------------------------
Scene::Scene(eScene n, bool hasInfo, Vector3 sp) 
{
    name = n;
    spawn = sp;
    EntityLight* lus = new EntityLight(Vector3(0, 200, 0), Vector3(-0.00785305, -0.472548, 0.88127), Vector3(1, 1, 1), .5);
    lights.push_back(lus);

    // carga la Scene per defecte
    if(!hasInfo)
        defaultScene();
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

void Scene::defaultScene()
{
    // Skybox
    getSkybox("data/Skybox/sphere.obj", "data/Skybox/Night.png");

    // Floor
    Object* floor = new Floor();
    static_objects.push_back(floor);
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
    printf("current_scene: %d\n", current_scene);
    // A partir de aqui empezamos a cargar el mapa.

    player = new Player();
    changeScene(current_scene);
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
    Scene* scene = scenes[current_scene];

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

    Scene* scene = scenes[current_scene];

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

void World::changeScene(eScene nextScene)
{
    // canviar el spawn del player
    player->spawn = scenes[nextScene]->spawn;

    // caniar el current
    current_scene = nextScene;

    // deixar les caixa
    player->LeaveBox();
    
    // respawn el player
    player->respawn();
}

// lista de enums per trobar quin amb quin al carregar (ORDENAR PER eObjType DE skin.h!!!)
block2enums block2Info[SIZEOFOT] = {
    {PLAYER,ePlayer, NotUse},
    {FLOOR,efloor, NotUse},
    {BLOCKLARGE,eBLarge, BLARGE},
    {BLOCKLONG,eBLong, BLONG},
    {BLOCKUNIT,eBUnit, BUNIT},
    {BOX,eBox, NotUse},
    {JEWEL,eJewel, BJEWEL},
    {MUSHROOM,eMushroom, BMUSHROOM},
    {ROCK,eRock, BROCK},
    {WEED,eWeed, BWEED}
};

// guardar i carregar Scenes
NameLevel TableSceneNames[SIZEOFSCENE] = {
    {"DEMO",DEMO},
    {"NIVELDELAVA",NIVELDELAVA}
};

Level* World::SaveScene()
{
    // crea level + nom per defecte
    Level* level = new Level{"DEMO"};

    Scene* scene = scenes[current_scene];

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
    // Nom del nivell
    eScene nameScene = DEFAULTSCENE;
    for (int i = 0; nameScene == DEFAULTSCENE && i < SIZEOFSCENE; i++)
    {
        // si es el element
        if(strstr(level->name, TableSceneNames[i].cName) != NULL) 
            nameScene = TableSceneNames[i].eName;
    }
    // Nom del nivell no trobat
    if(nameScene == DEFAULTSCENE)
    {
        std::cout << "Error: Name of level not found" << std::endl;
        return;
    }

    // player spawn
    if(level->player.type != ePlayer)
    {
        std::cout << "Error: spawn of player nt fount" << std::endl;
        return;
    }

    // trobar i colocar el nivell en la llist on toqui
    int lastid = scenes.size();
    Scene* scene = new Scene(nameScene, true, level->player.pos);
    if(lastid > nameScene)
        scenes[nameScene] = scene;
    else
    {
        // crea el elemnt default que faltin
        for(int id = lastid; id < nameScene; id++)
        {
            scenes.push_back(new Scene( (eScene)id) );
        }
        // afegeix en el index corresponent la scene
        scenes.push_back(scene);
    }

    // skybox
    scene->getSkybox(level->Skybox.mesh, level->Skybox.texture);

    EntityMesh* m;
    eEntityName name;

    // llista statica
    StaticObj sobj;
    Object* objb;
    for(int i = 0; i < level->numSObj; i++)
    {
        sobj = level->sObjs[i];
        switch (sobj.type)
        {
            case eBLarge:
            case eBLong:
            case eBUnit:
            case eJewel:
            case eMushroom:
            case eRock:
            case eWeed:
            {
                if (block2Info[sobj.type].extra == NotUse)
                {
                    std::cout << "Error: type Static Uncorrect" << std::endl;
                    break;
                }
                name = block2Info[sobj.type].entity;
                m = searchMesh(name);
                objb = new Block(m, sobj.pos, block2Info[sobj.type].extra); // falta la rotacio implemetar en el constructor

                // afeges la mesh del obj si no estaba
                if( m == NULL )
                {
		            meshs.push_back(objb->mesh);
	            }

                // afegim la Box a la llista de objectes
                objb->idList = scene->dinamic_objects.size();
                scene->static_objects.push_back(objb);

                break;
            }
            default:
                std::cout << "Error: Object Static Unespectet" << std::endl;
                break;
        }
    }
    // llista dinamica
    DinamicObj dobj;
    DinamicObject* objd;
    for(int i = 0; i < level->numDObj; i++)
    {
        dobj = level->dObjs[i];
        switch (dobj.type)
        {
            case eBox:
            {
                name = block2Info[dobj.type].entity;
                m = searchMesh(name);
                objd = new Box(m, dobj.pos);

                // afeges la mesh del obj si no estaba
                if( m == NULL )
                {
		            meshs.push_back(objd->mesh);
	            }
                
                // afegim la Box a la llista de objectes
                objd->idList = scene->dinamic_objects.size();
                scene->dinamic_objects.push_back(objd);

                break;
            }
            default:
                std::cout << "Error: Object Dinamic Unespectet" << std::endl;
                break;
        }
    }
    std::cout << " + Level Loaded" << std::endl; 
}
