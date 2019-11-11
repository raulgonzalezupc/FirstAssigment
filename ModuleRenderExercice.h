#ifndef _MODULERENDEREXERCICE_H_
#define _MODULERENDEREXERCICE_H_
#include "Module.h"
#include "Globals.h"

#include "MathGeoLib/include/Math/float4x4.h"
#include "glew/include/GL/glew.h"
#include "SDL.h"


class ModuleRenderExercice : public Module
{
public:
	ModuleRenderExercice();
	~ModuleRenderExercice();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
public:

	GLuint vbo;
	GLuint ebo;
	

};

#endif