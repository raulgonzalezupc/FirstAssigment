#include "ModuleProgram.h"
#include "Application.h"
#include "ModuleImgui.h"
#include "Globals.h"
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


	//init fragment shader
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

	return true;
}

update_status ModuleProgram::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleProgram::Update()
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