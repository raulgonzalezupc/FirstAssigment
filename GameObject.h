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
	const char* name;
	std::vector<Component*> components;
	GameObject* parent;
	std::vector<GameObject*> children;
	bool staticGO = false;

public:
	//constructors + destructors
	GameObject() {}

	GameObject(const char* name) : name(name) {
		CreateComponent(ComponentType::Transform);
	}

	~GameObject() {}

public: 
	update_status Update();

public:
	//components
	Component* CreateComponent(const ComponentType);
	Component* FindComponent(const ComponentType);

	void DeleteChild(const GameObject*);
	void ShowProperties();

private:
	void CreateTransform(const float3&, const Quat&);
};
#endif
