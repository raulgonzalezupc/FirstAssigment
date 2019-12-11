#ifndef _MODULERENDER_H_
#define _MODULERENDER_H_

#include "Module.h"
#include "Globals.h"

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
	void WindowResized(unsigned width, unsigned height);
	void DrawScene(float width, float height);
	int loadCubemap(std::vector<std::string> faces);
public:
	void ShowGrid();
	void ShowAxis();
	void ShowRenderUI();
	void CatchFrameBufferErrors();

public:
	SDL_GLContext glcontext;
	int width, height;
	bool showGrid = true, showAxis = true;
	unsigned int fbo;
	unsigned int texture;
	unsigned int rbo;
	unsigned int framebuffer;
	unsigned int texColorBuffer;

	std::vector<std::string> faces
	{
			"right.jpg",
			"left.jpg",
			"top.jpg",
			"bottom.jpg",
			"front.jpg",
			"back.jpg"
	};
	 int cubemapTexture = loadCubemap(faces);

	 float skyboxVertices[108] = {
		 // positions          
		 -1.0f,  1.0f, -1.0f,
		 -1.0f, -1.0f, -1.0f,
		  1.0f, -1.0f, -1.0f,
		  1.0f, -1.0f, -1.0f,
		  1.0f,  1.0f, -1.0f,
		 -1.0f,  1.0f, -1.0f,

		 -1.0f, -1.0f,  1.0f,
		 -1.0f, -1.0f, -1.0f,
		 -1.0f,  1.0f, -1.0f,
		 -1.0f,  1.0f, -1.0f,
		 -1.0f,  1.0f,  1.0f,
		 -1.0f, -1.0f,  1.0f,

		  1.0f, -1.0f, -1.0f,
		  1.0f, -1.0f,  1.0f,
		  1.0f,  1.0f,  1.0f,
		  1.0f,  1.0f,  1.0f,
		  1.0f,  1.0f, -1.0f,
		  1.0f, -1.0f, -1.0f,

		 -1.0f, -1.0f,  1.0f,
		 -1.0f,  1.0f,  1.0f,
		  1.0f,  1.0f,  1.0f,
		  1.0f,  1.0f,  1.0f,
		  1.0f, -1.0f,  1.0f,
		 -1.0f, -1.0f,  1.0f,

		 -1.0f,  1.0f, -1.0f,
		  1.0f,  1.0f, -1.0f,
		  1.0f,  1.0f,  1.0f,
		  1.0f,  1.0f,  1.0f,
		 -1.0f,  1.0f,  1.0f,
		 -1.0f,  1.0f, -1.0f,

		 -1.0f, -1.0f, -1.0f,
		 -1.0f, -1.0f,  1.0f,
		  1.0f, -1.0f, -1.0f,
		  1.0f, -1.0f, -1.0f,
		 -1.0f, -1.0f,  1.0f,
		  1.0f, -1.0f,  1.0f
	 };
private:
	void* context;

};

#endif