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
	void GenerateBuffers(int width, int height);
	void DrawScene(int width, int height);
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






public: 
	void DrawGame();

	//If scene create buffer for scene else create buffer for game window
	void CreateFrameBuffer(int width, int height, bool scene = true);
	void GenerateTexture(int width, int height);
	void GenerateTextureGame(int width, int height);
	void DrawSceneBuffer();
	void DrawGameBuffer();
	//Quadtree variables
	bool showQuadTree = false;
	bool showAABBTree = true;
	bool showFrustum = true;
	bool antialiasing = false;

	//Windows size
	int heightScene, widthScene;
	int heightGame, widthGame;
	bool firstTimeCreatingBuffer = true;

	//Framebuffer windows variables
	unsigned int frameBufferObject = 0; // FBO
	unsigned int renderBufferObject = 0; // RBO

	//Framebuffer windows variables
	unsigned int frameBufferObjectGame = 0; // FBO
	unsigned int renderBufferObjectGame = 0; // RBO

	unsigned int multiSampledAntiAliasingDepth = 0; //MSAAD
	unsigned int multiSampledAntiAliasingColor = 0; //MSAAC

	unsigned int sceneTexture = 0;
	unsigned int gameTexture = 0;





};

#endif