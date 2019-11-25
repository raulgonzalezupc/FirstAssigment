#ifndef _MODULERENDER_H_
#define _MODULERENDER_H_

#include "Module.h"
#include "Globals.h"

#include "MathGeoLib/include/Math/float4x4.h"
#include "SDL.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleRender : public Module
{
public:
	ModuleRender();
	virtual ~ModuleRender();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	void WindowResized(unsigned width, unsigned height);

public:
	void ShowGrid();
	void ShowAxis();
	void ShowRenderUI();
	

public:
	SDL_GLContext glcontext;
	int width, height;
	bool showGrid = true, showAxis = true;
private:
	void* context;

};

#endif