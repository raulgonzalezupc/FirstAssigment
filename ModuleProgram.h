#ifndef _MODULEPROGRAM_H_
#define _MODULEPROGRAM_H_

#include "Module.h"
#include "Application.h"

#include "glew/include/GL/glew.h"

class ModuleProgram : public Module {
	
public:
	ModuleProgram();
	~ModuleProgram();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	const char* loadFile(const char* file_name);

public:
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint shader_program;

};



#endif