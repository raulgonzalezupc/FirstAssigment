#ifndef _MODULEIMGUI_H_
#define _MODULEIMGUI_H_

#include "Module.h"
#include "Globals.h"
#include "GameObject.h"
#include "Application.h"
#include "SDL.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl.h"
#include <vector>
using namespace std;
enum KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};
class ModuleImgui : public Module
{
public:
	ModuleImgui();
	virtual ~ModuleImgui();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	const void DrawHierarchy(const std::vector<GameObject*>& objects, int & index);
	update_status PostUpdate();
	bool CleanUp();

	void AddLog(const char*, ...);
	void ShowAboutUI();
	void ShowConfigurationUI();
public: 
	SDL_GLContext imguiglcontext;

	//buttons
	bool menuButton = false;
	bool consoleButton = true;
	bool aboutButton = false;
	bool configButton = false;
	bool windowButton = false;
	bool propertiesButton = true;
	bool google = false;
	GameObject* sourceGO = nullptr;
	bool quit = false;
	unsigned selected = 0;
	bool showHierarchy = true;
	int vram_budget, vram_available;

	KeyState GetMouseButtonDown(int id) const
	{
		return mouse_buttons[id - 1];
	}


	SDL_version compiled;
	SDL_version linked;

	bool MouseClick(SDL_MouseButtonEvent& b);

	KeyState	mouse_buttons[5];

	ImGuiTextBuffer buffer;
	bool scrollToBottom;
	vector<float> fps;
	vector<float> fpsms;

};

#endif