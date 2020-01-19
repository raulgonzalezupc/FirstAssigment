#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "Globals.h"
#include <string>
#include "MathGeoLib/include/Geometry/AABB.h"
#include <vector>
#include "Components/Component.h"
#include "Components/Transform.h"
#include "Components/Camera.h"
#include "Mesh.h"

class Component;
class GameObject {

public :

	int uuid;
	const char* name;
	Mesh* myMesh = nullptr;
	std::vector<Component*> components;
	GameObject* parent = nullptr;
	std::vector<GameObject*> children;
	bool staticGO = false;
	Transform* myTransform = nullptr;
	AABB* boundingBox = nullptr;
	AABB* globalBoundingBox = nullptr;
public:
	//constructors + destructors
	GameObject() {}

	GameObject(const char* name) : name(name) {
		CreateComponent(ComponentType::Transform);
	}
	GameObject(const char* name, const float3& pos, const Quat& rot) : name(name) {
		CreateTransform(pos, rot);
	}
	~GameObject() {}

public: 
	update_status Update();

public:
	//components
	Component* CreateComponent(const ComponentType);
	Component* FindComponent(const ComponentType);

	GameObject* FindChild(const char * name);

	void DeleteChild(const GameObject*);
	void ComputeAABB();
	void DrawAABB();
	void ShowProperties();

private:
	void CreateTransform(const float3&, const Quat&);
};
#endif
