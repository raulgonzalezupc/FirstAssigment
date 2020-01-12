#ifndef _MODULIMPORTER_H_
#define _MODULEIMPORTER_H_

#include "Module.h"
class ModuleImporter :
	public Module
{
public:
	ModuleImporter();
	~ModuleImporter();

	
public:
	bool Init();

	std::string ComputeName(const std::string & path) const;

};

#endif
