#include "ModuleImporter.h"
#include <filesystem>

using namespace std;
using namespace std::tr2::sys;

ModuleImporter::ModuleImporter()
{
}


ModuleImporter::~ModuleImporter()
{
}

bool ModuleImporter::Init()
{
	LOG("Import all files on directory assets into own binary file");
	//Import all files on directory assets into own binary file
	string path = "../Assets";
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
			//Importer->ImportModel(path.c_str(), file.c_str(), s);

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


