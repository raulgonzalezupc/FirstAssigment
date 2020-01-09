#include "ModuleScene.h"
#include "Application.h"
#include "ModuleModelLoader.h"


ModuleScene::ModuleScene()
{
}


ModuleScene::~ModuleScene()
{
}

bool ModuleScene::Init()
{
	game = new GameObject("game");
	scene = new GameObject("Scene");
	bakerHouse = new GameObject("BakerHouse");

	camGame = new Camera(game, 1);
	game->components.push_back(camGame);

	camScene = new Camera(scene, 2);
	scene->components.push_back(camScene);

	App->modelLoader->LoadModel("BakerHouse.fbx");
	return true;
}


GameObject * ModuleScene::CreateGameObject(const char* name)
{
	GameObject* object = new GameObject(name);
	game_objects.push_back(object);
	return object;
}

void ModuleScene::LoadDefaultScene(const char* path)
{
	if (path != "")
	{
		//App->moduleloader->LoadModel("Models/BakerHouse.fbx");
	}
	else
	{
		//TODO: Read a json file and load the scene
	}
}