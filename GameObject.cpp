#include "GameObject.h"
#include "Components/Component.h"
#include "Components/Transform.h"
#include "imgui/imgui.h"
#include <algorithm>
#include "glew/include/GL/glew.h"
#include "DebugDraw.h"

Component* GameObject::CreateComponent(ComponentType type) {
	if (type == ComponentType::Transform) {
		components.push_back(new Transform(this));
	}

	Component *newComponent;
	switch (type)
	{
	case ComponentType::Camera:
		//components.push_back(new Camera());
		break;

	case ComponentType::Material:
		//components.push_back(new Material());
		break;

	case ComponentType::Mesh:
		//components.push_back(new Mesh());
		break;
	default:
		return nullptr;
	}

	newComponent->owner = this;
	components.push_back(newComponent);

	return newComponent;

}

update_status GameObject::Update() {
	return UPDATE_CONTINUE;
}

Component* GameObject::FindComponent(ComponentType type) {

	for (unsigned int i = 0; i < components.size(); i++) {
		if (components[i]->type == type) {
			Component* res = components[i];
			return res;
		}
	}
	return nullptr;
}

GameObject* GameObject::FindChild(const char* childName) 
{
	
	for (unsigned int i = 0; i < children.size(); i++) {
		
		if (strcmp(children[i]->name, childName)) {
			GameObject* res = children[i];
			return res;
		}
	}
	return nullptr;
}

void GameObject::ShowProperties() {
	ImGui::Begin("Properties PopUp");
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


void GameObject::ComputeAABB()
{
	float3 min = float3::zero;
	float3 max = float3::zero;

	if (myMesh == nullptr)
	{
		LOG("This gameObject does not have a Mesh thus we compute the AABB from his childs. code 928340");

		if (children.size() == 0)
		{
			LOG("Cannot compute the AABB because gameObject does not have children.code 834839");
			return;	//leave at this point
		}

		for (auto child : children)
		{
			if (child->boundingBox != nullptr)
			{
				//Min vertex
				if (child->boundingBox->minPoint.x < min.x)
					min.x = child->boundingBox->minPoint.x;
				if (child->boundingBox->minPoint.y < min.y)
					min.y = child->boundingBox->minPoint.y;
				if (child->boundingBox->minPoint.z < min.z)
					min.z = child->boundingBox->minPoint.z;
				//Max vertex
				if (child->boundingBox->maxPoint.x > max.x)
					max.x = child->boundingBox->maxPoint.x;
				if (child->boundingBox->maxPoint.y > max.y)
					max.y = child->boundingBox->maxPoint.y;
				if (child->boundingBox->maxPoint.z > max.z)
					max.z = child->boundingBox->maxPoint.z;
			}
		}

		boundingBox = new AABB(min, max);
		//Compute globalBoundingBox
		float3 globalPos, globalScale;
		float3x3 globalRot;
		myTransform->worldTransform.Decompose(globalPos, globalRot, globalScale);
		globalBoundingBox = new AABB(min + globalPos, max + globalPos);
	}


	for (auto vertex : myMesh->myMesh->vertices)
	{
		//Min vertex
		if (vertex.Position.x < min.x)
			min.x = vertex.Position.x;
		if (vertex.Position.y < min.y)
			min.y = vertex.Position.y;
		if (vertex.Position.z < min.z)
			min.z = vertex.Position.z;
		//Max vertex
		if (vertex.Position.x > max.x)
			max.x = vertex.Position.x;
		if (vertex.Position.y > max.y)
			max.y = vertex.Position.y;
		if (vertex.Position.z > max.z)
			max.z = vertex.Position.z;
	}

	boundingBox = new AABB(min, max);

	//Compute globalBoundingBox
	float3 globalPos, globalScale;
	float3x3 globalRot;
	myTransform->worldTransform.Decompose(globalPos, globalRot, globalScale);
	globalBoundingBox = new AABB(min + globalPos, max + globalPos);
}

void GameObject::DrawAABB()
{
	//dd::aabb(globalBoundingBox->minPoint, globalBoundingBox->maxPoint, float3(0, 1, 0));
}


void GameObject::CreateTransform(const float3& pos, const Quat& rot) {
	components.push_back(new Transform(this, pos, rot));
}