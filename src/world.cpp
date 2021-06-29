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

    // craer la scene on inicias per si no existeix
    current_scene = eScene(0);

    Scene* scene = new Scene(current_scene);
    scenes.push_back(scene);

    player = new Player();

    player->current_scene = current_scene;
    changeScene(current_scene);
}

void World::setCamera( int window_width, int window_height )
{
    // create our camera
	camera = new Camera();
	camera->lookAt(Vector3(0.f,100.f, 100.f),Vector3(1.f,0.f,0.f), Vector3(0.f,1.f,0.f)); //position the camera and point to 0,0,0
	camera->setPerspective(70.f,window_width/(float)window_height,0.1f,10000.f); //set the projection, we want to be perspective
}

EntityMesh* World::searchMesh( eObjectName obj )
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
        if (object->oName != BOX) continue;

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

    for (int i = 0; i < scene->dinamic_objects.size(); i++)
    {
        object = scene->dinamic_objects[i];

        if (object->mesh == NULL) continue;

        mesh = object->mesh->mesh;

        if (!mesh->testRayCollision(object->model, origin, dir, col, normal))
            continue;

        BlockPicked = object;

        break;
    }
}

void World::Render(bool CamInPlayer)
{
	cfgPlayer* cfgP = player->cfgP;

	// moure la camera a la pos del player
	if(CamInPlayer)
		camera->lookAt(player->getPosition() +  cfgP->altura, cfgP->altura + player->getPosition() + player->getDir(), Vector3(0,1.01,0));
    
    // set the camera as default
	camera->enable();
   
	Scene* scene = scenes[current_scene];
	Object* object;

	// Skybox
	EntityMesh* skybox = scene->skybox;
	if (skybox != NULL){
		// mover skybox en la pos de player
		skybox->model.setTranslation(camera->eye);

		// render de skybox
		skybox->render(camera);
	}

	// render objectes	
	for (int id = 0; id < scene->static_objects.size(); id++)
	{
		//for para static_objects	
		object = scene->static_objects[id];
		switch( object->oName ){
			case JEWEL:
				((Jewel*)object)->render(camera, scene->lights);
				break;
			default:
				object->render(camera, scene->lights);
				break;
		}
		
	}

	for (int id = 0; id < scene->dinamic_objects.size(); id++)
	{
		//for para dinamics_objects	
		object = scene->dinamic_objects[id];
		object->render(camera, scene->lights);
	}
}

void World::Update(double elapsed_time)
{
    Scene* scene = scenes[current_scene];
    
    // Update de objectes dinamics
    DinamicObject* object;
	for (int i = 0; i < scene->dinamic_objects.size(); i++)
	{ 
		object = scene->dinamic_objects[i];
		if(!object->isCatch && !isTimeStopped)
			object->move(elapsed_time, player->getPosition(), scene->static_objects, scene->dinamic_objects);
		if (object->oName == SAW || object->oName == SAWHUNTER) {
			if (!isTimeStopped)
				object->model.rotate(elapsed_time, object->model.frontVector());

			// --- Audio ---
			#ifdef _WINDOWS_
			Saw* saw = (Saw*) object;
			if (saw->isSawDoingNoise == false) {
				saw->sawNoise->play(0.5);
				saw->isSawDoingNoise = true;
			}
			else {
				float distance = player->model.getTranslation().distance(saw->model.getTranslation());
				distance = 10 / (distance);
				clamp(distance, 0.001, 0.5);
				if (isTimeStopped)
					distance = 0;
				saw->sawNoise->setVolume(distance);
			}
			#endif
		}
	}
}

void World::changeScene(eScene nextScene)
{
    if (nextScene == WIN)
        nextScene = STARTLEVEL;

    if(nextScene >= scenes.size())
    {
        player->current_scene = current_scene;
        return;
    }
    // canviar el spawn del player
    player->spawn = scenes[nextScene]->spawn;

    // caniar el current
    current_scene = nextScene;

    // respawn
    respawn();
}

void World::respawn()
{
    // deixar box
    player->LeaveBox();
    BlockPicked = NULL;

    // respawn player
    player->respawn();

    // respawn objectes dinamics
    Scene* scene = scenes[current_scene];
    DinamicObject* object;
	for (int i = 0; i < scene->dinamic_objects.size(); i++)
	{ 
		object = scene->dinamic_objects[i];
		object->respawn();
	}

    player->isDead = false;
    player->current_scene = current_scene;
}

bool World::hasWin()
{
    if(JewelsWin != 0 && (JewelsWin & player->pickedJewel) == JewelsWin)
        return true;
    else
        return false;
}

void World::reset()
{
    changeScene(STARTLEVEL);
	player->pickedJewel = 0;
}

Level* World::SaveScene()
{
    Scene* scene = scenes[current_scene];

    // crea level + nom del nivell actual
    Level* level = new Level{ TableSceneNames[scene->name].cName };

    // Skybox
    cfgMesh* cfgSB = cfgSkyboxCreat(scene->skybox->texture->filename.c_str());
    level->Skybox = *cfgSB;

    // Player
    DinamicObj* sPlayer = new DinamicObj{PLAYER, player->getPosition() + Vector3(0,h_spawn,0)};
    level->player = *sPlayer;

    Object* object;

    // llista de Statics
    StaticObj sObjs[MAXOBJ];
    int max = 0;
    for (int id = 0; id < scene->static_objects.size() && id < MAXOBJ; id++)
    {
        object = scene->static_objects[id];

        // afegir el elemnt a la llista
        level->sObjs[id] = *(new StaticObj{object->oName, object->getPosition(), object->getDir()}); 
        if(object->oName == JEWEL)
            level->sObjs[id].scene = ((Jewel*)object)->next_scene;
        max++;
    }
    level->numSObj = max;

    // llista de Dinamics
    DinamicObj dObjs[MAXOBJ];
    max = 0;
    for (int id = 0; id < scene->dinamic_objects.size() && id < MAXOBJ; id++)
    {
        object = scene->dinamic_objects[id];
        
        // afegir el elemnt a la llista
        switch (object->oName)
        {
            case BOX:
                level->dObjs[id] = *(new DinamicObj{object->oName, object->getPosition() + Vector3(0,h_spawn,0)});
                max++;
                break;
            case SAW:
            {
                Saw* saw = (Saw*)object;
                level->dObjs[id] = *(new DinamicObj{object->oName, saw->center, saw->getDir(), saw->rad, saw->speed});
                max++;
            }
            default:
                break;
        }
        
    }
    level->numDObj = max;

    return level;
}

void World::LoadScene(Level* level, eScene nameScene)
{
    if(level == NULL)
    {
        return;
    }
    // Nom del nivell si no es passat per parametre
    if(nameScene == DEFAULTSCENE)
    {
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
    }

    // player spawn
    if(level->player.type != PLAYER)
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

    // llista statica
    StaticObj sobj;
    Object* objb;
    for(int i = 0; i < level->numSObj; i++)
    {
        sobj = level->sObjs[i];
        if(!hasBlock(sobj.type))
        {
            std::cout << "Error: " << TableObj2str[sobj.type].name << " has not Block" << std::endl;
            continue;
        }
        
        m = searchMesh(sobj.type);

        switch (sobj.type)
        {
            case JEWEL:
                objb = new Jewel(m, sobj.pos, sobj.scene, sobj.rot); 
                if(sobj.scene == WIN) // inicia la maskara per saber quan guanyas
                    JewelsWin |= ((Jewel*)objb)->idMask;
                break;
            default:
                objb = new Block(m, sobj.pos, sobj.type, sobj.rot); 
                break;
        }
        // afeges la mesh del obj si no estaba
        if( m == NULL )
        {
            meshs.push_back(objb->mesh);
        }

        // afegim la Box a la llista de objectes
        objb->idList = scene->dinamic_objects.size();
        scene->static_objects.push_back(objb);
    }
    // llista dinamica
    DinamicObj dobj;
    DinamicObject* objd;
    for(int i = 0; i < level->numDObj; i++)
    {
        dobj = level->dObjs[i];
        if(!hasBlock(sobj.type))
        {
            std::cout << "Error: " << TableObj2str[sobj.type].name << " has not Dinamic" << std::endl;
            continue;
        }

        m = searchMesh(dobj.type);

        switch (dobj.type)
        {
            case BOX:
                objd = new Box(m, dobj.pos);
                break;
            case SAW:
                objd = new SawBasic(m, dobj.pos, dobj.rot, dobj.rad, dobj.vel);
                break;
            case SAWHUNTER:
                objd = new SawHunter(m, dobj.pos, dobj.rot, dobj.rad, dobj.vel);
                break;
            default:
                break;
        }
        // afeges la mesh del obj si no estaba
        if( m == NULL )
        {
            meshs.push_back(objd->mesh);
        }
        
        // afegim la Box a la llista de objectes
        objd->idList = scene->dinamic_objects.size();
        scene->dinamic_objects.push_back(objd);
    }
    std::cout << " + Level Loaded" << std::endl; 
}
