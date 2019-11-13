#include "ModuleImgui.h"
#include "Application.h"
#include "Globals.h"
#include "ModuleWindow.h"
#include "glew/include/GL/glew.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl.h"
#include "SDL.h"

ModuleImgui::ModuleImgui()
{
}

// Destructor
ModuleImgui::~ModuleImgui()
{
}

// Called before render is available
bool ModuleImgui::Init()
{

	//glew init

	imguiglcontext = ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, imguiglcontext);
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGui::StyleColorsDark();

	return true;
}

update_status ModuleImgui::PreUpdate()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleImgui::Update()
{
	//Menu
	if (ImGui::BeginMainMenuBar())
	{
		ImGui::MenuItem("Menu", (const char*)0, &menuButton);
		ImGui::MenuItem("Window", (const char*)0, &windowButton);
		ImGui::MenuItem("Console", (const char*)0, &consoleButton);
		ImGui::MenuItem("About", (const char*)0, &aboutButton);
		ImGui::MenuItem("FPS", (const char*)0, &fpsButton);
		ImGui::EndMainMenuBar();
	}
	if (fpsButton)
	{
		char title[25];
		char titleMs[25];
		ImGuiIO& io = ImGui::GetIO();
		fps.push_back(io.Framerate);
		fpsms.push_back(1000.0f / io.Framerate);
		sprintf_s(title, 25, "Framerate %.1f", fps[fps.size() - 1]);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::PlotHistogram("##Framerate", &fps[0], fps.size(), 0, title, 0.0F, 100.0F, ImVec2(310, 100) );
		sprintf_s(title, 25, "Miliseconds %.1f", fpsms[fpsms.size() - 1]);
		ImGui::PlotHistogram("##Miliseconds", &fpsms[0], fpsms.size(), 0, title, 0.0F, 50.0F, ImVec2(310, 100));

		//ImGui::ShowMetricsWindow();
	}
	if (windowButton)
	{
		App->window->ShowWindowUI();
	}
	return UPDATE_CONTINUE;
}

update_status ModuleImgui::PostUpdate()
{
	//update a window with OpenGL rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleImgui::CleanUp()
{
	//Destroy wndow
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	return true;
}
