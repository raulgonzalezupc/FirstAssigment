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
	std::vector<float3> modelBox;
	float3 correctCameraPositionForModel = float3(0.0f, 0.0f, 0.0f);
	float3 modelCenter = float3(0.0f, 0.0f, 0.0f);
	void computeModelBoundingBox();
	void ChangeModel(const char * path);
private:
	void processNode(aiNode*, const aiScene*);
	Mesh processMesh(aiMesh*, const aiScene*);
	std::vector<Texture> loadMaterialTextures(aiMaterial*, aiTextureType, char*);
	
};

#endif