#include "ModuleImporter.h"
#include "Application.h"
#include <filesystem>
#include "assimp/include/assimp/DefaultLogger.hpp"
#include "assimp/include/assimp/Logger.hpp"
#include "assimp/include/assimp/cimport.h"
#include "assimp/include/assimp/Importer.hpp"
#include "assimp/include/assimp/scene.h"
#include "assimp/include/assimp/postprocess.h"
#include "assimp/include/assimp/mesh.h"
#include "assimp/include/assimp/material.h"
#include "SDL.h"
#include "json.hpp"


using namespace Assimp;
using namespace std;
using namespace std::tr2::sys;

class myStream : public Assimp::LogStream {
public:
	// LOG assimp debug output to GUI console
	void write(const char *message) {
		LOG("%s", message);
	}
};

ModuleImporter::ModuleImporter()
{
}


ModuleImporter::~ModuleImporter()
{
}

bool ModuleImporter::Init()
{

	//Importing .fbx files from Assets folder
	string path = "./Assets";
	for (const auto& entry : recursive_directory_iterator(path))
	{
		string fullPath(entry.path().string());
		string fileExt(fullPath);
		size_t dotFound = fileExt.find_last_of(".");
		fileExt.erase(0, dotFound + 1);
		if (fileExt == "fbx")
		{
			string path(fullPath);
			string file = ComputeName(fullPath);


			size_t sizeFile = path.size() - file.size();
			path = path.substr(0, sizeFile);

			string s;
			LOG("Importing file %s", file);
			Import(path.c_str(), file.c_str(), s);

		}

	}

	return true;
}


std::string ModuleImporter::ComputeName(const std::string & path) const
{
	size_t simpleRightSlash = path.find_last_of('/');
	if (string::npos != simpleRightSlash)
	{
		LOG("Directory with simpleRightSlashes.")
			return path.substr(path.find_last_of('/') + 1, path.size() - 1);
	}
	size_t simpleLeftSlash = path.find_last_of('\\');
	if (string::npos != simpleLeftSlash)
	{
		LOG("Directory with simpleLeftSlashes.")
			return path.substr(path.find_last_of('\\') + 1, path.size() - 1);
	}
	size_t doubleRightSlash = path.find_last_of("//");
	if (string::npos != doubleRightSlash)
	{
		LOG("Directory with doubleRightSlashes.")
			return path.substr(path.find_last_of("//") + 1, path.size() - 1);
	}

	size_t doubleLeftSlash = path.find_last_of("\\\\");
	if (string::npos != doubleLeftSlash)
	{
		LOG("Directory with doubleLeftSlashes.")
			return path.substr(path.find_last_of("\\\\") + 1, path.size() - 1);
	}

	return path;
}

bool ModuleImporter::Import(const char * path, const char * file, string & output_file)
{
	string filepath = path; filepath += file;
	LOG("Importing mesh from %s.", filepath.c_str());
	
	// Assimp logger
	Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE);
	Assimp::DefaultLogger::get()->info("this is my info-call");
	const unsigned int severity = Assimp::Logger::Debugging | Assimp::Logger::Info | Assimp::Logger::Err | Assimp::Logger::Warn;
	Assimp::DefaultLogger::get()->attachStream(new myStream, severity);


	//Load info from model
	const aiScene* scene = aiImportFile(filepath.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		LOG("ERROR ASSIMP: %s \n", aiGetErrorString());
		LOG("Can't load model from %s.", filepath.c_str());
		return false;
	}

	LOG("For each mesh located on the current node, processing meshes.");
	
	modelName = file;
	size_t lastindex = modelName.find_last_of(".");
	modelName = modelName.substr(0, lastindex);
	directory = path;

	ProcessNode(scene->mRootNode, scene);

	SaveModelFile(output_file);


	return false;
}


bool ModuleImporter::Import(const char * file, const MeshData & mesh, string & output_file)
{
	unsigned int ranges[2] = { mesh.num_indices, mesh.num_vertices };

	unsigned int size = sizeof(ranges)					//ranges
		+ sizeof(unsigned int) * mesh.num_indices		//indices
		+ sizeof(float) * mesh.num_vertices * 3			//vertex positions
		+ sizeof(float) * mesh.num_vertices * 3			//vertex normals
		+ sizeof(float) * mesh.num_vertices * 2;		//vertex texture coord



	char* data = new char[size]; // Allocate
	char* cursor = data;

	unsigned int bytes = sizeof(ranges); // First store ranges
	memcpy(cursor, ranges, bytes);

	cursor += bytes; // Store indices
	bytes = sizeof(unsigned int) * mesh.num_indices;
	memcpy(cursor, mesh.indices, bytes);

	cursor += bytes; // Store vertex positions
	bytes = sizeof(float) * mesh.num_vertices * 3;
	memcpy(cursor, mesh.positions, bytes);

	cursor += bytes; // Store vertex normals
	bytes = sizeof(float) * mesh.num_vertices * 3;
	memcpy(cursor, mesh.normals, bytes);

	cursor += bytes; // Store vertex texture coords
	bytes = sizeof(float) * mesh.num_vertices * 2;
	memcpy(cursor, mesh.texture_coords, bytes);

	bool succes = Import(file, data, size, output_file);
	delete[] data;
	return succes;
}


bool ModuleImporter::Import(const char * file, const void * buffer, unsigned int size, string & output_file)
{
	if (!is_directory("../Library"));
	create_directory("../Library");
	if (!is_directory("../Library/Meshes"));
	create_directory("../Library/Meshes");

	json j;

	string filename = file; filename += ".nfbx";
	output_file = "../Library/Meshes/"; output_file += filename.c_str();

	return Save("../Library/Meshes/", filename.c_str(), buffer, size, false);
	return false;

}


void ModuleImporter::SaveModelFile(string & output_file)
{
	unsigned int ranges[3] = { modelData.meshes.size(), modelData.textures.size(), modelData.pairs.size() };

	//Set up mesh sizes
	unsigned int * meshNameSizes = new unsigned int[modelData.meshes.size()];
	unsigned int meshNamesSize = 0;
	for (unsigned int i = 0; i < modelData.meshes.size(); i++)
	{
		meshNameSizes[i] = modelData.meshes[i].size();
		meshNamesSize += meshNameSizes[i] + 1;
	}

	//Set up texture sizes
	unsigned int * textureNameSizes = new unsigned int[modelData.textures.size()];
	unsigned int textureNamesSize = 0;
	for (unsigned int i = 0; i < modelData.textures.size(); i++)
	{
		textureNameSizes[i] = modelData.textures[i].size();
		textureNamesSize += textureNameSizes[i] + 1;
	}

	//Set up pair sizes
	unsigned int * pairData = new unsigned int[modelData.pairs.size() * 2];
	for (unsigned int i = 0; i < modelData.pairs.size(); i++)
	{
		pairData[2 * i] = modelData.pairs[i].mesh;
		pairData[2 * i + 1] = modelData.pairs[i].tex;
	}


	//Calculate total size
	unsigned int size = sizeof(ranges)							//ranges
		+ sizeof(unsigned int) * modelData.meshes.size()		//mesh name sizes
		+ meshNamesSize											//mesh names
		+ sizeof(unsigned int) * modelData.textures.size()		//texture name sizes
		+ textureNamesSize										//texture names
		+ sizeof(unsigned int) * 2 * modelData.pairs.size();	//mesh - texture pairs

	char* data = new char[size];
	char* cursor = data;

	unsigned int bytes = sizeof(ranges); 
	memcpy(cursor, ranges, bytes);

	cursor += bytes;
	bytes = sizeof(unsigned int) * modelData.meshes.size();
	memcpy(cursor, meshNameSizes, bytes);

	for (unsigned int i = 0; i < modelData.meshes.size(); i++) 
	{
		cursor += bytes;
		bytes = modelData.meshes[i].size() + 1;
		memcpy(cursor, modelData.meshes[i].c_str(), bytes);
	}

	cursor += bytes; 
	bytes = sizeof(unsigned int) * modelData.textures.size();
	memcpy(cursor, textureNameSizes, bytes);

	for (unsigned int i = 0; i < modelData.textures.size(); i++) 
	{
		cursor += bytes;
		bytes = modelData.textures[i].size() + 1;
		memcpy(cursor, modelData.textures[i].c_str(), bytes);
	}

	cursor += bytes; 
	bytes = sizeof(unsigned int) * 2 * modelData.pairs.size();
	memcpy(cursor, pairData, bytes);

	Import(modelName.c_str(), data, size, output_file);
}


bool ModuleImporter::Save(const char * path, const char * file, const void * buffer, unsigned int size, bool append) const
{
	string filename = path; filename += file;

	SDL_RWops *rw;
	if (append)
		rw = SDL_RWFromFile(filename.c_str(), "a");
	else
		rw = SDL_RWFromFile(filename.c_str(), "w");

	if (rw == NULL) return NULL;

	Sint64 nb_write_total = SDL_RWwrite(rw, buffer, 1, size);
	SDL_RWclose(rw);

	if (nb_write_total != size)
		return NULL;

	return 1;
}


void ModuleImporter::ProcessNode(aiNode * node, const aiScene * scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh(mesh, scene);
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
	
}

void ModuleImporter::ProcessMesh(const aiMesh * mesh, const aiScene * scene)
{
	MeshData meshData;

	//Fill vertex data
	meshData.num_vertices = mesh->mNumVertices;

	meshData.positions = new float[meshData.num_vertices * 3];
	meshData.normals = new float[meshData.num_vertices * 3];
	meshData.texture_coords = new float[meshData.num_vertices * 2];

	for (unsigned int i = 0; i < meshData.num_vertices; i++)
	{
		meshData.positions[3 * i + 0] = mesh->mVertices[i].x;
		meshData.positions[3 * i + 1] = mesh->mVertices[i].y;
		meshData.positions[3 * i + 2] = mesh->mVertices[i].z;

		meshData.normals[3 * i + 0] = mesh->mNormals[i].x;
		meshData.normals[3 * i + 1] = mesh->mNormals[i].y;
		meshData.normals[3 * i + 2] = mesh->mNormals[i].z;

		if (mesh->mTextureCoords[0]) 
		{
			meshData.texture_coords[2 * i + 0] = mesh->mTextureCoords[0][i].x;
			meshData.texture_coords[2 * i + 1] = mesh->mTextureCoords[0][i].y;
		}
		else
		{
			meshData.texture_coords[2 * i + 0] = 0.0f;
			meshData.texture_coords[2 * i + 1] = 0.0f;
		}
	}

	//Fill indices data
	meshData.num_indices = mesh->mNumFaces * 3; 
	meshData.indices = new unsigned int[meshData.num_indices];

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			meshData.indices[3 * i + j] = face.mIndices[j];
	}

	//Import mesh into own filesystem
	string meshOutput;
	unsigned int currentMeshCount = modelData.meshes.size() + 1;
	string meshName = modelName;
	meshName += to_string(currentMeshCount);
	Import(meshName.c_str(), meshData, meshOutput);
	modelData.meshes.push_back(meshOutput);

	//Mesh data have to be deleted
	delete[] meshData.indices;
	delete[] meshData.normals;
	delete[] meshData.positions;
	delete[] meshData.texture_coords;

	//Process material
	/*
	string materialOutput;
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

		//TODO: chech this works correctly
		SearchTextureByType(material, aiTextureType_DIFFUSE, currentMeshCount, "_diffuse");
		SearchTextureByType(material, aiTextureType_SPECULAR, currentMeshCount, "_specular");
		SearchTextureByType(material, aiTextureType_AMBIENT, currentMeshCount, "_occlusive");
		SearchTextureByType(material, aiTextureType_EMISSIVE, currentMeshCount, "_emissive");
		SearchTextureByType(material, aiTextureType_NORMALS, currentMeshCount, "_normal");
		SearchTextureByType(material, aiTextureType_HEIGHT, currentMeshCount, "_height");
	}
	*/
}


