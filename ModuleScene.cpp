#include "ModuleScene.h"
#include "Application.h"
#include "ModuleModelLoader.h"
#include "json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

ModuleScene::ModuleScene()
{
}


ModuleScene::~ModuleScene()
{
}

bool ModuleScene::Init()
{
	root = new GameObject("root");
	GameObject* game = new GameObject("Game");
	root->children.push_back(game);
	game->parent = root;

	GameObject* scene = new GameObject("Scene");
	root->children.push_back(scene);
	scene->parent = root;

	/*GameObject* bakerHouse = new GameObject("BakerHouse");
	root->children.push_back(bakerHouse);
	bakerHouse->parent = root;*/

	camGame = new Camera(game, 1);
	game->components.push_back(camGame);

	camScene = new Camera(scene, 2);
	scene->components.push_back(camScene);

	App->modelLoader->LoadModel("BakerHouse.fbx");

	SaveScene();

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
		////TODO: Implement and call avay OnLoad method from each component

	}
}

void ModuleScene::DrawAllBoundingBoxes()
{
	for (unsigned int i = 0; i < root->children.size(); i++)
	{
		root->children[i]->DrawAABB();
	}
}

GameObject* ModuleScene::CreateGameObject(const char* name, const float3& pos, const Quat& rot) const {
	return new GameObject(name, pos, rot);
}

GameObject* ModuleScene::CreateGameObjectByName(const char* name) const {
	return new GameObject(name);
}

update_status ModuleScene::Update()
{
	return UPDATE_CONTINUE;
}

void ModuleScene::SaveScene() {
	//TODO: Implement and call OnSave methods from each component

}

