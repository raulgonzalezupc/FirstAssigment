#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleCamera.h"
#include "ModuleRender.h"
#include "ModuleImgui.h"
#include "imgui/imgui.h"
#include "SDL.h"
#include "glew/include/GL/glew.h"
ModuleWindow::ModuleWindow()
{
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{

	resizable = true;
	App->imgui->AddLog("Init SDL window & surface\n");
	bool ret = true;

	SDL_DisplayMode DM;
	SDL_Init(SDL_INIT_VIDEO);
	int should_be_zero = SDL_GetCurrentDisplayMode(0, &DM);
	if (should_be_zero != 0)
	{
		App->imgui->AddLog("Error loading current display \n");
	}
	else {
		App->imgui->AddLog("Current display loaded correctly \n");
	}
	unsigned int width = DM.w;
	unsigned int height = DM.h;
	
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		App->imgui->AddLog("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		flags = SDL_WINDOW_SHOWN |  SDL_WINDOW_OPENGL;

		if(FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}
		if (resizable)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width*0.8, height*0.8, flags);
		windowID = SDL_GetWindowID(window);
		if(window == NULL)
		{
			App->imgui->AddLog("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}
update_status ModuleWindow::Update() 
{
	return UPDATE_CONTINUE;
}
// Called before quitting
bool ModuleWindow::CleanUp()
{
	App->imgui->AddLog("Destroying SDL window and quitting all SDL systems");
	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::ShowWindowUI()
{
	//ImGui::ShowTestWindow();
	ImGui::Text("Window Settings:");
	ImGui::Checkbox("Resizable off:", &resizable); 
	if (!resizable)
	{
		SDL_HideWindow(App->window->window);
	}


	float f2 = App->camera->frustum.verticalFov;
	ImGui::SliderFloat("Fov ", &f2, 0.01f, 3.12F, "%.2f", 2.0f);
	App->camera->setFOV(f2);
}