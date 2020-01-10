#ifndef _MODULEINPUT_H_
#define _MODULEINPUT_H_

#include "Module.h"
#include "Globals.h"
#include "Components/Camera.h"
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
	update_status SetInputsWithoutCam();
	update_status SetInputsWithCam(Camera* cam);
	SDL_Event event;

	
public:	
	Camera* camHovered;
	const Uint8 *keyboard = NULL;
	SDL_MouseWheelEvent* mouseWheel;
};

#endif