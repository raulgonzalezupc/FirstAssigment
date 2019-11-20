#pragma once

#include<list>
#include "Globals.h"
#include "Module.h"

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleProgram;
class ModuleTexture;
class ModuleCamera;
class ModuleImgui;
class ModuleTimer;
class ModuleModelLoader;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

public:
	ModuleRender* renderer = nullptr;
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleProgram* program = nullptr;
	ModuleTexture* texture = nullptr;
	ModuleCamera* camera = nullptr;
	ModuleImgui* imgui = nullptr;
	ModuleTimer* timer = nullptr;
	ModuleModelLoader* modelLoader = nullptr;

private:

	std::list<Module*> modules;

};

extern Application* App;
