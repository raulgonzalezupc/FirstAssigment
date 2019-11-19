#ifndef _MESH_H_
#define _MESH_H_	

#include "Globals.h"
#include "Module.h"
#include "MathGeoLib/include/Math/float3.h"
#include <string>
#include <vector>
#include "MathGeoLib/include/Math/float2.h"
#include "ModuleTexture.h"


struct Vertex {
	float3 Position;
	float3 Normal;
	float2 TexCoords;
	float3 Tangent;
	float3 Bitangent;
};
struct Texture {
	unsigned int id;
	std::string type;
};
class Mesh
{
public:
	/*  Mesh Data  */
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	/*  Functions  */
	void Init();
	Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures);
	~Mesh();
	void Draw(unsigned int program) const;
private:
	/*  Render data  */
	unsigned int VAO, VBO, EBO;
	/*  Functions    */
	void setupMesh();
};
#endif __Mesh_H__
