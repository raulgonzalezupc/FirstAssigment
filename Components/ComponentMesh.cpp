#include "../Application.h"
#include "../ModuleModelLoader.h"
#include "Transform.h"
#include "../GameObject.h"
#include "ComponentMesh.h"

using namespace std;

ComponentMesh::~ComponentMesh()
{
	delete mesh;
}

void ComponentMesh::LoadMesh(Mesh* loadedMesh)
{
	mesh = loadedMesh;
}

void ComponentMesh::Draw(const unsigned int program) const
{
	mesh->Draw(program);
}

float ComponentMesh::IsIntersectedByRay(const float3 &origin, const LineSegment & ray)
{
	float minDist = -1.0f;

	for (unsigned int i = 0; i < mesh->indices.size(); i += 3)
	{

		Triangle tri = Triangle(mesh->vertices[mesh->indices[i]].Position, mesh->vertices[mesh->indices[i + 1]].Position, mesh->vertices[mesh->indices[i + 2]].Position);
		if (tri.Intersects(tri))
		{
			minDist = tri.Distance(origin);
		}
	}


	return minDist;
}



void ComponentMesh::ProcessMeshData(const MeshData& data, Mesh& mesh)
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

		//TODO: check if mesh contains texture coords or not
		float2 texturesCoords;
		texturesCoords.x = data.texture_coords[i * 2];
		texturesCoords.y = data.texture_coords[i * 2 + 1];
		vertex.TexCoords = texturesCoords;

		mesh.vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < data.num_indices; i++)
	{
		mesh.indices.push_back(data.indices[i]);
	}

	mesh.name = data.name;
}
