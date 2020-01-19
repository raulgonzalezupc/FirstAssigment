#ifndef _MESH_H_
#define _MESH_H_	

#include "Globals.h"
#include "Module.h"
#include <string>
#include <vector>
#include "ModuleTexture.h"
#include "MathGeoLib/include/Geometry/AABB.h"

struct MeshData
{
	unsigned int num_indices;
	unsigned int num_vertices;

	unsigned int * indices = nullptr;
	float * positions = nullptr;
	float * normals = nullptr;
	float * texture_coords = nullptr;

	std::string name;
};

class Mesh
{
public:
	/*  Mesh Data  */
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	std::string name;
	int numVertices, numPrimitives;
	AABB box;


	~Mesh();
	Mesh();
	/*  Functions  */
	void Init();
	Mesh* myMesh = nullptr;
	void Draw(unsigned int program) const;
	void SetUpMesh();
private:
	/*  Render data  */
	unsigned int VAO, VBO, EBO;
	/*  Functions    */
	
};
#endif __Mesh_H__
