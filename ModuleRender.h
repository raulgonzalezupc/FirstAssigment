#ifndef _MODULERENDER_H_
#define _MODULERENDER_H_

#include "Module.h"
#include "Globals.h"
#include "Components/Camera.h"
#include "GameObject.h"
#include "Skybox.h"
#include "MathGeoLib/include/Math/float4x4.h"
#include "SDL.h"
#include <vector>

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
	int loadCubemap(std::vector<std::string> faces);
public:
	void ShowGrid();
	void ShowAxis();
	void ShowRenderUI();
	void CatchFrameBufferErrors();

public:
	GameObject* test;
	GameObject* test2;
	Camera* cam;
	Camera* cam2;
	Skybox* skybox;

public:
	SDL_GLContext glcontext;
	int width, height;
	bool showGrid = true, showAxis = true;
	unsigned int fbo;
	unsigned int msfb_depth;
	unsigned int msfb_color;
	unsigned int texture;
	unsigned int rbo;
	unsigned int framebuffer;



private:
	void* context;





};

#endif