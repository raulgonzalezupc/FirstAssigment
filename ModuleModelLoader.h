#ifndef _MODULEMODELLOADER_H_
#define _MODULEMODELLOADER_H_

#include "Module.h"
#include "Globals.h"
#include "Mesh.h"
#define TEXTURE_PATH "Textures/"
#define DEFAULT_TEXTURE "Baker_house.png"
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
	GameObject* model;
	float ambient = 0.0f;
public:

	ModuleModelLoader();
	virtual ~ModuleModelLoader();
	bool Init();
	bool CleanUp();

public:
	void LoadModel(std::string&);
	void Draw(unsigned int program);
	void processMaterials(const aiMaterial * mat, GameObject * owner);
	void loadMaterialTextures(const aiMaterial * mat, aiTextureType type, const char* typeName, Material* material);
	void computeModelBoundingBox();
	void ChangeModel(const char * path);
	void ShowModelUI();
	bool FileExists(const char* path);

public:
	std::vector<float3> modelBox;
	float3 newCameraPos = float3(0.0f, 0.0f, 0.0f);
	float3 modelCenter = float3(0.0f, 0.0f, 0.0f);
	Mesh* newMesh;
private:
	bool meshed = false;
	std::string modelPath = "Models/";
	std::string myTexturesPath = "Textures/";
	std::string finalPath = "";
	void processNode(const aiNode *node, const aiScene *scene, GameObject* parent);
	void processMesh(const aiMesh* mesh, GameObject* owner);
	float* onething;
};

#endif