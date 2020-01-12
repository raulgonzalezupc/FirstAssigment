#ifndef __COMPONENTMESH_H__
#define __COMPONENTMESH_H__

#include "Component.h"
#include "../Mesh.h"

class ComponentMesh : public Component
{
public:
	ComponentMesh(GameObject* owner) : Component(owner, ComponentType::Mesh) {}
	virtual ~ComponentMesh();

	void LoadMesh(Mesh* mesh);
	void Draw(const unsigned int program) const;

	float IsIntersectedByRay(const float3 &origin, const LineSegment &ray);


	Mesh* mesh;
	void ProcessMeshData(const MeshData & data, Mesh & mesh);
};

#endif 