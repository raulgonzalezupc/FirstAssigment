#ifndef __MODULEWINDOW_H__
#define __MODULEWINDOW_H__

#include "Module.h"
#include "Application.h"
#include "SDL.h"

class Application;

class ModuleWindow : public Module
{
public:


	ModuleWindow();
	virtual ~ModuleWindow();

	bool Init();
	update_status Update();
	bool CleanUp();

public: 
	void ShowWindowUI();

public:
	SDL_Window* window = NULL;
	Uint32 windowID;
	float width;
	float height;
	SDL_Surface* screen_surface = NULL;
	bool fullScreen = false;
	bool fullScreenDesk = false;
	bool resizable;
	bool bordered = true;
	Uint32 flags;
};

#endif 