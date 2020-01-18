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
	void DrawGameObject(GameObject * parent, Camera * cam);
	void DrawMesh(Camera * cam, Transform * trans, Mesh * mesh, Material * material);
	int loadCubemap(std::vector<std::string> faces);
public:
	void ShowGrid();
	void ShowAxis();
	void ShowRenderUI();
	void CatchFrameBufferErrors();

public:
	GameObject* game;
	GameObject* scene;
	GameObject* bakerHouse;
	Camera* camScene;
	Camera* camGame;
	Skybox* skybox;

public:
	SDL_GLContext glcontext;
	int width, height;
	bool showAxis, showGrid;
private:
	void* context;





};

#endif