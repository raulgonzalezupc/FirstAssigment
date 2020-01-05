#include "ModuleProgram.h"
#include "Application.h"
#include "ModuleImgui.h"
#include "Globals.h"
#include "MathGeoLib/include/Math/float4x4.h"
#include "SDL.h"

ModuleProgram::ModuleProgram()
{

}

ModuleProgram::~ModuleProgram()
{

}

bool ModuleProgram::Init()
{
	App->imgui->AddLog("\nSHADERS.\n\n");
	//init vertex shader
	
	vs = createVertexShader(VERTEX_SHADER_PATH);
	fs = createFragmentShader(FRAGMENT_SHADER_PATH);

	skyboxVertexShader = App->program->createVertexShader("Game/Skybox.vs");
	skyboxFragmentShader = App->program->createFragmentShader("Game/Skybox.fs");

	skyboxProgram = CreateProgram(skyboxVertexShader, skyboxFragmentShader);
	defaultProgram = CreateProgram(vs, fs);
	setUniformsBuffer();
	return true;
}

update_status ModuleProgram::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleProgram::Update()
{
	return UPDATE_CONTINUE;
}

update_status ModuleProgram::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleProgram::CleanUp()
{
	App->imgui->AddLog("Deleting vertex shader\n");
	glDeleteShader(vertex_shader);

	App->imgui->AddLog("Deleting fragment shader\n");

	glDeleteShader(fragment_shader);
	return true;
}
GLuint ModuleProgram::CreateProgram(unsigned int vs, unsigned int fs)
{
	//Creating shader program
	shader_program = glCreateProgram();

	//Attaching vertex shader to shader program
	glAttachShader(shader_program, vertex_shader);

	//Attaching fragment shader to shader program
	glAttachShader(shader_program, fragment_shader);

	//Linking shader program
	glLinkProgram(shader_program);
	GLint statusP = NULL;
	char charP[512];
	glGetProgramiv(shader_program, GL_LINK_STATUS, &statusP);
	if (!statusP)
	{
		glGetProgramInfoLog(shader_program, 512, NULL, charP);
		App->imgui->AddLog("Program created wrong, code: %s\n", charP);
	}
	return shader_program;
}
const char* ModuleProgram::loadFile(const char* file_name)
{
	char* data = nullptr;
	FILE* file = nullptr;
	fopen_s(&file, file_name, "rb");
	if (file)
	{
		fseek(file, 0, SEEK_END);
		int size = ftell(file);
		rewind(file);
		data = (char*)malloc(size + 1);
		fread(data, 1, size, file);
		data[size] = 0;
		fclose(file);
	}

	return data;
}


unsigned int ModuleProgram::createVertexShader(const char * filename)
{
	assert(filename != NULL);
	const char *vertex_shader_file = loadFile(VERTEX_SHADER_PATH);
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_file, NULL);
	delete vertex_shader_file;
	glCompileShader(vertex_shader);
	GLint statusV = NULL;
	char charV[512];
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &statusV);
	if(!statusV)
	{
		glGetShaderInfoLog(vertex_shader, 512, NULL, charV);
		App->imgui->AddLog("Vertex shader created wrong, code: %s\n", charV);
	}
	else {
		App->imgui->AddLog("Vertex shader created correctly.\n");
	}


	return vertex_shader;
}

unsigned int ModuleProgram::createFragmentShader(const char * filename)
{
	assert(filename != NULL);
	const char *fragment_shader_file = loadFile(FRAGMENT_SHADER_PATH);
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_file, NULL);
	delete fragment_shader_file;
	glCompileShader(fragment_shader);
	GLint statusF = NULL;
	char charF[512];
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &statusF);
	if (!statusF)
	{
		glGetShaderInfoLog(fragment_shader, 512, NULL, charF);
		App->imgui->AddLog("Fragment shader created wrong, code: %s\n", charF);
	}
	else {
		App->imgui->AddLog("Fragment shader created correctly.\n");

	}

	return fragment_shader;
}


void ModuleProgram::setUniformsBuffer()
{
	
	unsigned int uniformBlockIndexDefault = glGetUniformBlockIndex(defaultProgram, "Matrices");
	glUniformBlockBinding(defaultProgram, uniformBlockIndexDefault, 0);

	unsigned int uniformSkybox = glGetUniformBlockIndex(skyboxProgram, "Skybox");
	glUniformBlockBinding(skyboxProgram, uniformSkybox, 0);

	glGenBuffers(1, &uniformsBuffer);

	glBindBuffer(GL_UNIFORM_BUFFER, uniformsBuffer);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(float4x4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uniformsBuffer, 0, 2 * sizeof(float4x4));

}