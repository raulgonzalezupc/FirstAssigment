#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "Application.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:
	float width;
	float height;
	ModuleWindow();
	
	// Destructor
	virtual ~ModuleWindow();

	// Called before quitting
	bool Init();
	update_status Update();

	// Called before quitting
	bool CleanUp();

public:
	//The window we'll be rendering to
	SDL_Window* window = NULL;
	Uint32 windowID;
	
	//The surface contained by the window
	SDL_Surface* screen_surface = NULL;
};

#endif // __ModuleWindow_H__