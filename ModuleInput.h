#pragma once
#include "Module.h"
#include "Globals.h"
#include "SDL.h"
#include <SDL_scancode.h>
typedef unsigned __int8 Uint8;

class ModuleInput : public Module
{
public:
	
	ModuleInput();
	~ModuleInput();

	bool Init();
	update_status Update();
	bool CleanUp();
	SDL_MouseWheelEvent* GetMouseWheel() const;
	
public:	
	const Uint8 *keyboard = NULL;
	SDL_MouseWheelEvent* mouseWheel;
};