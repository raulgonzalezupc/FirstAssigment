#ifndef _MODULERENDER_H_
#define _MODULERENDER_H_
#include "Module.h"
#include "Globals.h"

#include "MathGeoLib/include/Math/float4x4.h"
#include "glew/include/GL/glew.h"
#include "SDL.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleRender : public Module
{
public:
	ModuleRender();
	~ModuleRender();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	void WindowResized(unsigned width, unsigned height);

public:
	SDL_GLContext glcontext;
	GLuint vbo;
	GLuint ebo;
	float4x4 proj;
	float4x4 view;
	float4x4 model;
private:
	void* context;

};

#endif