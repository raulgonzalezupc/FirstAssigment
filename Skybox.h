#ifndef __Skybox_H__
#define __Skybox_H__

#include "Globals.h"
#include <vector>
#include <string>

class Skybox
{
public:
	Skybox();
	~Skybox();

	unsigned int LoadCubeMap(const std::vector<std::string> &faces);
	void DrawSkybox() const;
	std::string directory = "";

	unsigned int cubemapTexture = 0;

	unsigned int skyboxVAO = 0;
	unsigned int skyboxVBO = 0;

};

#endif __Skybox_H__












//
//
//
//
////Creating shader program
//shader_program = glCreateProgram();
//
////Attaching vertex shader to shader program
//glAttachShader(shader_program, vertex_shader);
//
////Attaching fragment shader to shader program
//glAttachShader(shader_program, fragment_shader);
//
////Linking shader program
//glLinkProgram(shader_program);
//GLint statusP = NULL;
//char charP[512];
//glGetProgramiv(shader_program, GL_LINK_STATUS, &statusP);
//if (!statusP)
//{
//	glGetProgramInfoLog(shader_program, 512, NULL, charP);
//	App->imgui->AddLog("Program created wrong, code: %s\n", charP);
//}
//return shader_program;