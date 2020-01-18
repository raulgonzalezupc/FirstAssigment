#ifndef _MODULIMPORTER_H_
#define _MODULEIMPORTER_H_

#include "Module.h"


struct aiNode;
struct aiMesh;
struct aiScene;
struct aiMaterial;

struct MeshTexPair
{
	unsigned int mesh;
	unsigned int tex;
};

struct ModelData
{
	std::vector<std::string> meshes;
	std::vector<std::string> textures;

	std::vector<MeshTexPair> pairs;
};


class ModuleImporter :
	public Module
{
public:
	ModuleImporter();
	~ModuleImporter();

	
public:
	bool Init();

	std::string ComputeName(const std::string & path) const;

	bool Import(const char * path, const char * file, std::string & output_file);

	void SaveModelFile(std::string & output_file);
	bool Import(const char * file, const void * buffer, unsigned int size, std::string & output_file);
	bool Save(const char * path, const char * file, const void * buffer, unsigned int size, bool append) const;
	void ProcessNode(aiNode * node, const aiScene * scene);
	void ProcessMesh(const aiMesh * mesh, const aiScene * scene);
	ModelData modelData;
	std::string modelName;
	std::string directory;

};

#endif
