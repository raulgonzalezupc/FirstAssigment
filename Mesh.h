#ifndef _MESH_H_
#define _MESH_H_	

#include "Globals.h"
#include "Module.h"
#include <string>
#include <vector>
#include "ModuleTexture.h"



class Mesh
{
public:
	/*  Mesh Data  */
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	
	~Mesh();
	Mesh();
	/*  Functions  */
	void Init();
	
	void Draw(unsigned int program) const;
	void setupMesh();
private:
	/*  Render data  */
	unsigned int VAO, VBO, EBO;
	/*  Functions    */
	
};
#endif __Mesh_H__
