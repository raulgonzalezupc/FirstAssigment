#ifndef __COMPONENTMESH_H__
#define __COMPONENTMESH_H__

#include "Component.h"
#include "../Mesh.h"

class ComponentMesh : public Component
{
public:
	ComponentMesh(GameObject* owner, Mesh* mesh) : Component(owner, ComponentType::Mesh), myMesh(mesh) {}
	virtual ~ComponentMesh();

	void LoadMesh(Mesh* mesh);
	void Draw(const unsigned int program) const;

	/*float IsIntersectedByRay(const float3 &origin, const LineSegment &ray);*/

	Mesh* myMesh;

	void DrawView();

	void ProcessMeshData(const MeshData & data);
};

#endif 