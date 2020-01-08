#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleCamera.h"
#include "ModuleImgui.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleModelLoader.h"
#include "MathGeoLib.h"
#include "SDL/include/SDL.h"

ModuleInput::ModuleInput()
{}

// Destructor
ModuleInput::~ModuleInput()
{}

// Called before render is available
bool ModuleInput::Init()
{
	App->imgui->AddLog("Init SDL input event system\n");
	camHovered = new Camera(nullptr,1);
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		App->imgui->AddLog("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());

		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::Update()
{
	SDL_PumpEvents();

	keyboard = SDL_GetKeyboardState(NULL);
	if (keyboard[SDL_SCANCODE_ESCAPE])
	{
		return UPDATE_STOP;
	}
	if (App->renderer->camGame->isHovered) {
		update_status resCam = SetInputsWithCam(App->renderer->camGame);
		if (resCam == UPDATE_STOP) 
		{
			return UPDATE_STOP;
		}
	}
	if (App->renderer->camScene->isHovered) {
		update_status resScene = SetInputsWithCam(App->renderer->camScene);
		if (resScene == UPDATE_STOP)
		{
			return UPDATE_STOP;
		}
	}
	
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	App->imgui->AddLog("Quitting SDL input event subsystem\n");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

update_status ModuleInput::SetInputsWithCam(Camera* cam)
{
	
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_WINDOWEVENT:
				switch (event.window.event) {
				case SDL_WINDOWEVENT_SIZE_CHANGED:
					App->window->width = event.window.data1;	App->window->height = event.window.data2;
					App->camera->SetAspectRatio(App->window->width*0.8 / App->window->height*0.8, cam);
					break;
				case SDL_WINDOWEVENT_CLOSE:
					return UPDATE_STOP;
					break;
				}
				break;

			case SDL_MOUSEMOTION:
				if (event.motion.state & SDL_BUTTON_RMASK) {
					if (math::Abs(event.motion.xrel) > 1.5) {
						App->camera->Rotate('X', event.motion.xrel * 0.03, cam);
					}

					if (math::Abs(event.motion.yrel) > 1.5) {
						App->camera->Rotate('Y', event.motion.yrel * 0.03, cam);
					}

				}
				if ((event.motion.state & SDL_BUTTON_LMASK) && keyboard[SDL_SCANCODE_LALT]) {
					if (math::Abs(event.motion.xrel) > 1.5) {
						App->camera->Orbit('X', event.motion.xrel * 0.03, cam);
					}

					if (math::Abs(event.motion.yrel) > 1.5) {
						App->camera->Orbit('Y', event.motion.yrel * 0.03, cam);
					}
				}
				break;

			case SDL_MOUSEWHEEL:
				if (event.wheel.y > 0)
				{
					App->camera->MoveForward(cam);
				}
				else if (event.wheel.y < 0)
				{
					App->camera->MoveBackwards(cam);
				}
				break;

			case SDL_DROPFILE:
				std::string path = event.drop.file;
				std::string extension = path.substr(path.size() - 3, path.size());

				if (extension == "fbx")
				{
					App->modelLoader->ChangeModel(event.drop.file);
					App->modelLoader->computeModelBoundingBox();
					App->camera->focusCameraToNewPoint(App->modelLoader->newCameraPos,cam);
				}
				else if (extension == "jpg" || extension == "png" || extension == "dds" || extension == "jpeg" || extension == "tga")
				{
					App->texture->LoadTexture(path.c_str());
					App->modelLoader->texturesLoaded.insert(App->modelLoader->texturesLoaded.begin(), App->texture->LoadTexture(path.c_str()));

				}
				SDL_free(event.drop.file);
				break;
			}

		}

	

	//keyboard inputs
	if (keyboard[SDL_SCANCODE_ESCAPE])
	{
		return UPDATE_STOP;
	}
	if (keyboard[SDL_SCANCODE_Q])
	{
		App->camera->MoveUp(cam);
	}
	if (keyboard[SDL_SCANCODE_E])
	{
		App->camera->MoveDown(cam);
	}
	if (keyboard[SDL_SCANCODE_W])
	{
		App->camera->MoveForward(cam);
	}
	if (keyboard[SDL_SCANCODE_S])
	{
		App->camera->MoveBackwards(cam);
	}
	if (keyboard[SDL_SCANCODE_A])
	{
		App->camera->MoveLeft(cam);
	}
	if (keyboard[SDL_SCANCODE_D])
	{
		App->camera->MoveRight(cam);
	}
	if (keyboard[SDL_SCANCODE_F])
	{
		App->modelLoader->computeModelBoundingBox();
		App->camera->focusCameraToNewPoint(App->modelLoader->newCameraPos, cam);
	}
	if (keyboard[SDL_SCANCODE_UP])
	{
		App->camera->Rotate('X', 1.0f, cam);
	}
	if (keyboard[SDL_SCANCODE_DOWN])
	{
		App->camera->Rotate('X', -1.0f, cam);
	}
	if (keyboard[SDL_SCANCODE_LEFT])
	{
		App->camera->Rotate('Y', -1.0f, cam);
	}
	if (keyboard[SDL_SCANCODE_RIGHT])
	{
		App->camera->Rotate('Y', 1.0f, cam);
	}
	if (keyboard[SDL_SCANCODE_LSHIFT])
	{
		App->camera->multSpeed(cam);
	}
	else {
		App->camera->speed = 1.0f;
	}
}