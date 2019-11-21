#ifndef _MODULEMODELLOADER_H_
#define _MODULEMODELLOADER_H_

#include "Module.h"
#include "Globals.h"
#include "Mesh.h"
#include "assimp/include/assimp/scene.h"
#include <vector>

class ModuleModelLoader : public Module
{
public:
	std::vector<Texture> texturesLoaded;
	std::vector<Mesh*> meshes;
	std::string directory;
	ModuleModelLoader();
	~ModuleModelLoader();
	bool Init();
	bool CleanUp();
	void LoadModel(const char*);
	void Draw(unsigned int program);


	unsigned int numMeshes = 0;
	int numPolys=NULL;
	unsigned int numVertices = 0;
	int textureWidth;
	int textureHeight;
	char* textureType=nullptr;
	int textureId;
private:
	void processNode(aiNode*, const aiScene*);
	Mesh processMesh(aiMesh*, const aiScene*);
	std::vector<Texture> loadMaterialTextures(aiMaterial*, aiTextureType, char*);
};

#endif