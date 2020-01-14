#include "ModuleImporter.h"
#include <filesystem>
#include "assimp/include/assimp/DefaultLogger.hpp"
#include "assimp/include/assimp/cimport.h"
#include "assimp/include/assimp/Logger.hpp"
#include "Application.h"
#include "assimp/include/assimp/cimport.h"
#include "assimp/include/assimp/Importer.hpp"
#include "assimp/include/assimp/scene.h"
#include "assimp/include/assimp/postprocess.h"

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


	//Clear assimp debugger
	//DefaultLogger::kill();

	SaveModelFile(output_file);


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

	unsigned int bytes = sizeof(ranges); // First store ranges
	memcpy(cursor, ranges, bytes);

	cursor += bytes; // Store mesh name sizes
	bytes = sizeof(unsigned int) * modelData.meshes.size();
	memcpy(cursor, meshNameSizes, bytes);

	for (unsigned int i = 0; i < modelData.meshes.size(); i++) // Store mesh names (strings)
	{
		cursor += bytes;
		bytes = modelData.meshes[i].size() + 1;
		memcpy(cursor, modelData.meshes[i].c_str(), bytes);
	}

	cursor += bytes; // Store texture name sizes
	bytes = sizeof(unsigned int) * modelData.textures.size();
	memcpy(cursor, textureNameSizes, bytes);

	for (unsigned int i = 0; i < modelData.textures.size(); i++) // Store texture names (strings)
	{
		cursor += bytes;
		bytes = modelData.textures[i].size() + 1;
		memcpy(cursor, modelData.textures[i].c_str(), bytes);
	}

	cursor += bytes; // Store mesh - texture pairs
	bytes = sizeof(unsigned int) * 2 * modelData.pairs.size();
	memcpy(cursor, pairData, bytes);

	Import(modelName.c_str(), data, size, output_file);
}

bool ModuleImporter::Import(const char * file, const void * buffer, unsigned int size, string & output_file)
{/*
	//TODO:inlcude filesystem in application
	if (!App->filesystem->IsDirectory("../Library"))
		App->filesystem->MakeDirectory("../Library");
	if (!App->filesystem->IsDirectory("../Library/Meshes"))
		App->filesystem->MakeDirectory("../Library/Meshes");

	string filename = file; filename += ".notfbx";
	output_file = "../Library/Meshes/"; output_file += filename.c_str();

	return App->filesystem->Save("../Library/Meshes/", filename.c_str(), buffer, size, false);*/
	return false;
}

void ModuleImporter::ProcessNode(aiNode * node, const aiScene * scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		//ProcessMesh(mesh, scene);
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
	
}

