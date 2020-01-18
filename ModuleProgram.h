#ifndef _MODULEPROGRAM_H_
#define _MODULEPROGRAM_H_

#include "Module.h"
#include "Application.h"

#include "glew/include/GL/glew.h"
enum class ProgramType
{
	Default = 0,
	Flat,
	Gouraud,
	Phong,
	Blinn,
	Count
};
class ModuleProgram : public Module {
	
public:
	ModuleProgram();
	virtual ~ModuleProgram();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	const char* loadFile(const char* file_name);
	GLuint CreateProgram(const unsigned int, const unsigned int);
	unsigned int createVertexShader(const char* filename);
	unsigned int createFragmentShader(const char* filename);
	void setUniformsBuffer();
	unsigned programs[int(ProgramType::Count)];
public:
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint shader_program;
	GLuint skyboxProgram;
	GLuint sceneProgram;
	unsigned int vs;
	unsigned int fs;
	unsigned int skyboxFragmentShader;
	unsigned int skyboxVertexShader;
	unsigned int uniformsBuffer;
	GLuint defaultProgram;
};



#endif