#ifndef _MODULEPROGRAM_H_
#define _MODULEPROGRAM_H_

#include "Module.h"
#include "Application.h"

class ModuleProgram : public Module {
	
public:
	ModuleProgram();
	~ModuleProgram();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

};



#endif