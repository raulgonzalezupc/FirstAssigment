#ifndef _MODULETEXTURE_H_
#define _MODULETEXTURE_H_

#include "Module.h"
#include "Mesh.h"
#include "glew/include/GL/glew.h"
#include "DevIL/include/IL/il.h"
#include "DevIL/include/IL/ilu.h"

class ModuleTexture : public Module {

public:
	ModuleTexture();
	~ModuleTexture();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	Texture LoadTexture(const char* path);

public:
	ILuint ImageName;
	GLuint texture;
	ILinfo imageInfo;
};

#endif