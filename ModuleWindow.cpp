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
	width = DM.w;
	height = DM.h;
	
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

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width*0.95, height*0.95, flags);
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

	ImGui::Checkbox("Full screen", &fullScreen);
	if (fullScreen)
	{
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	}


	ImGui::Checkbox("Full screen desktop", &fullScreenDesk);
	if (fullScreenDesk)
	{
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	}

	ImGui::Checkbox("Borderless widow:", &bordered);
	if (bordered) {
		SDL_SetWindowBordered(window, SDL_TRUE);
	}
	else {
		SDL_SetWindowBordered(window, SDL_FALSE);
	}


	//Change window size
	ImGui::SliderFloat("Window Width ", &width, 0.01f, 1980.0F, "%.2f", 2.0f);
	ImGui::SliderFloat("Window Height ", &height, 0.01f, 1080.0F, "%.2f", 2.0f);
	SDL_SetWindowSize(window, width, height);
	

}