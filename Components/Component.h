
#ifndef Component_h
#define Component_h
#include "../Globals.h"

class GameObject;
class Component {
public:
	int uuid;
	ComponentType type;
	bool active;
	GameObject* owner;
	Component() {}
	Component(GameObject* owner, const ComponentType type) : owner(owner), type(type) {}
	virtual ~Component() {}
	virtual void Enable() { active = true; }
	virtual update_status Update() { return UPDATE_CONTINUE; }
	virtual void Disable() { active = false; }
	virtual void DrawView() {}
};
#endif