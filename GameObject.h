#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "Globals.h"
#include <string>
#include <vector>
#include "Components/Component.h"

class Component;
class GameObject {

public :

	int uuid;
	std::string name;
	GameObject* parent;
	std::vector<GameObject*> children;
	std::vector<Component*> components;


	GameObject() {}
	~GameObject() {}
	update_status Update();


};



#endif 