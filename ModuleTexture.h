#ifndef _MODULETEXTURE_H_
#define _MODULETEXTURE_H_

#include "Module.h"

#include "glew/include/GL/glew.h"
#include "DevIL/include/IL/il.h"


class ModuleTexture : public Module {

public:
	ModuleTexture();
	~ModuleTexture();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

public:
	ILuint ImageName;
	GLuint texture;
};

#endif