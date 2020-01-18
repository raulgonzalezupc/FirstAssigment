#ifndef __MODULIMPORTER_H__
#define __MODULEIMPORTER_H__

#include "Module.h"
#include "Mesh.h"


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

struct aiNode;
struct aiMesh;
struct aiScene;
struct aiMaterial;



class ModuleImporter :
	public Module
{
public:
	ModuleImporter();
	~ModuleImporter();

	
public:
	bool Init();

	std::string ComputeName(const std::string & path) const;

	void SaveModelFile(std::string & output_file);
	bool Import(const char * file, const void * buffer, unsigned int size, std::string & output_file);
	bool Import(const char * path, const char * file, std::string & output_file);
	bool Import(const char * file, const MeshData & mesh, std::string & output_file);
	bool Save(const char * path, const char * file, const void * buffer, unsigned int size, bool append) const;
	void ProcessNode(aiNode * node, const aiScene * scene);
	void ProcessMesh(const aiMesh * mesh, const aiScene * scene);

	ModelData modelData;
	std::string modelName;
	std::string directory;

};

#endif //__MODULEIMPORTER_H__
