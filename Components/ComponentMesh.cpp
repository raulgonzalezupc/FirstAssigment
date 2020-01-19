#include "../Application.h"
#include "../ModuleModelLoader.h"
#include "Transform.h"
#include "../GameObject.h"
#include "ComponentMesh.h"
#include "../imgui/imgui.h"

using namespace std;

ComponentMesh::~ComponentMesh()
{
	delete myMesh;
}

void ComponentMesh::LoadMesh(Mesh* loadedMesh)
{
	myMesh = loadedMesh;
}

void ComponentMesh::Draw(const unsigned int program) const
{
	myMesh->Draw(program);
}
//
//float ComponentMesh::IsIntersectedByRay(const float3 &origin, const LineSegment & ray)
//{
//	float minDist = -1.0f;
//
//	for (unsigned int i = 0; i < myMesh->indices.size(); i += 3)
//	{
//
//		Triangle tri = Triangle(myMesh->vertices[myMesh->indices[i]].Position, mesh->vertices[mesh->indices[i + 1]].Position, myMesh->vertices[mesh->indices[i + 2]].Position);
//		if (tri.Intersects(tri))
//		{
//			minDist = tri.Distance(origin);
//		}
//	}
//
//
//	return minDist;
//}
void ComponentMesh::DrawView() {
	if (ImGui::TreeNode("Geometry")) {
		ImGui::Text("Indices:%d\nVertices:%d\nTextures:%d ", myMesh->indices.size(), myMesh->vertices.size(), myMesh->textures.size());
		ImGui::TreePop();
	}
}



void ComponentMesh::ProcessMeshData(const MeshData& data)
{
	for (unsigned int i = 0; i < data.num_vertices; i++)
	{
		Vertex vertex;
		float3 positions;
		positions.x = data.positions[i * 3];
		positions.y = data.positions[i * 3 + 1];
		positions.z = data.positions[i * 3 + 2];
		vertex.Position = positions;

		float3 normals;
		normals.x = data.normals[i * 3];
		normals.y = data.normals[i * 3 + 1];
		normals.z = data.normals[i * 3 + 2];
		vertex.Normal = normals;

		
		float2 texturesCoords;
		texturesCoords.x = data.texture_coords[i * 2];
		texturesCoords.y = data.texture_coords[i * 2 + 1];
		vertex.TexCoords = texturesCoords;

		myMesh->vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < data.num_indices; i++)
	{
		myMesh->indices.push_back(data.indices[i]);
	}

	myMesh->name = data.name;
}
void ComponentMesh::TransformAABB(float4x4* transform) {
	if (originalBox.IsDegenerate()) {
		originalBox = box;
	}
	box = originalBox;
	box.TransformAsAABB(*transform);
}