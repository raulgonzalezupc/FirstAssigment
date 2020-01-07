#ifndef _MODULEMODELLOADER_H_
#define _MODULEMODELLOADER_H_

#include "Module.h"
#include "Globals.h"
#include "Mesh.h"
#include "assimp/include/assimp/scene.h"
#include <vector>
#include <string>

class ModuleModelLoader : public Module
{
public:
	std::vector<Texture> texturesLoaded;
	std::vector<Mesh*> meshes;
	std::string directory;

//texture properties 
public:
	unsigned int numMeshes = NULL;
	unsigned int numVertices = NULL;
	int textureWidth =	NULL;
	int textureHeight = NULL;
	int textureId = NULL;

public:

	ModuleModelLoader();
	virtual ~ModuleModelLoader();
	bool Init();
	bool CleanUp();

public:
	void LoadModel(const char*);
	void Draw(unsigned int program);
	void computeModelBoundingBox();
	void ChangeModel(const char * path);
	void ShowModelUI();
	bool FileExists(const char* path);

public:
	std::vector<float3> modelBox;
	float3 newCameraPos = float3(0.0f, 0.0f, 0.0f);
	float3 modelCenter = float3(0.0f, 0.0f, 0.0f);
	
private:
	bool meshed = false;
	std::string modelPath = "Models/";
	std::string myTexturesPath = "Textures/";
	std::string finalPath = "";
	void processNode(aiNode*, const aiScene*, GameObject* parent);
	Mesh* processMesh(aiMesh*, const aiScene*);
	std::vector<Texture> loadMaterialTextures(aiMaterial*, aiTextureType, char*);
	float* onething;
};

#endif