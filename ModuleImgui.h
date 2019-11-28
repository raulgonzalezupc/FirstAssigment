#ifndef _MODULEIMGUI_H_
#define _MODULEIMGUI_H_

#include "Module.h"
#include "Globals.h"
#include "Application.h"
#include "SDL.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl.h"
#include <vector>
using namespace std;


class ModuleImgui : public Module
{
public:
	ModuleImgui();
	virtual ~ModuleImgui();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void AddLog(const char*, ...);
	void ShowAboutUI();
	void ShowConfigurationUI();
	void ShowTimerController();
public: 
	SDL_GLContext imguiglcontext;

	//buttons
	bool menuButton = false;
	bool consoleButton = true;
	bool aboutButton = false;
	bool configButton = false;
	bool windowButton = false;
	bool propertiesButton = false;
	bool google = false;
	bool quit = false;


	int vram_budget, vram_available;


	SDL_version compiled;
	SDL_version linked;
	
	ImGuiTextBuffer buffer;
	bool scrollToBottom;
	vector<float> fps;
	vector<float> fpsms;

};

#endif