#include "GameObject.h"
#include "Components/Component.h"
#include "Components/Transform.h"
#include "imgui/imgui.h"
#include <algorithm>


void GameObject::CreateTransform(const float3& pos, const Quat& rot) 
{
	components.push_back(new Transform(this, pos, rot));
}
//Component* GameObject::CreateComponent(ComponentType type) {
//	if (type == ComponentType::Transform) {
//		components.push_back(new Transform(this));
//	}
//
//	Component *newComponent;
//	switch (type)
//	{
//	case ComponentType::Camera:
//		//components.push_back(new Camera());
//		break;
//
//	case ComponentType::Material:
//		//components.push_back(new Material());
//		break;
//
//	case ComponentType::Mesh:
//		//components.push_back(new Mesh());
//		break;
//	default:
//		return nullptr;
//	}
//
//	newComponent->owner = this;
//	components.push_back(newComponent);
//
//	return newComponent;
//
//}

update_status GameObject::Update() {
	return UPDATE_CONTINUE;
}
Component* GameObject::FindComponent(ComponentType type) {
	for (unsigned int i = 0; i < components.size(); i++) {
		if (components[i]->type == type) {
			return components[i];
		}
	}
	return nullptr;
}


void GameObject::ShowProperties() {
	ImGui::Begin("Properties Test");
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it) {
		(*it)->uuid = uuid;
		//ImGui::Begin("Component:%s:", (*it)->);
		(*it)->DrawView();
		//ImGui::End();
	}
	ImGui::End();
}



void GameObject::DeleteChild(const GameObject* child) {
	children.erase(std::remove(children.begin(), children.end(), child), children.end());

}
