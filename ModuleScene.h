#ifndef _MODULESCENE_H_
#define _MODULESCENE_H_


#include "Module.h"
#include "Globals.h"
#include "Components/Camera.h"
#include "GameObject.h"
#include <vector>

class ModuleScene : public Module
{
public:
	ModuleScene();
	virtual ~ModuleScene();
	bool Init();
public :
	GameObject* game;
	GameObject* scene;
	GameObject* bakerHouse;
	Camera* camScene;
	Camera* camGame;

	void LoadDefaultScene(const char * path);

private:
	GameObject* CreateGameObject(const char * name);
	std::vector<GameObject*> game_objects;

};

#endif