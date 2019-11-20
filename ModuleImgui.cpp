#include "ModuleImgui.h"
#include "Application.h"
#include "Globals.h"
#include "ModuleWindow.h"
#include "ModuleModelLoader.h"
#include "ModuleTimer.h"
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
	AddLog("Open GL version 330\n");
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
	ImGui::ShowDemoWindow();
	int timer = App->timer->StartRealTimeClock();
	ImGui::Begin("Clock");
	ImGui::Text("Real time:  %d:%d:%d", (timer / (1000 * 60 * 60)) % 24, (timer / (1000 * 60)) % 60, (timer / 1000) % 60);
	ImGui::End();
	//ImGui::ShowDemoWindow();
	//Menu
	if (ImGui::BeginMainMenuBar())
	{
		ImGui::MenuItem("Menu", (const char*)0, &menuButton);
		ImGui::MenuItem("Window", (const char*)0, &windowButton);
		ImGui::MenuItem("Console", (const char*)0, &consoleButton);
		ImGui::MenuItem("About", (const char*)0, &aboutButton);
		ImGui::MenuItem("Configuration", (const char*)0, &configButton);
		ImGui::MenuItem("Properties", (const char*)0, &propertiesButton);
		ImGui::EndMainMenuBar();
	}
	if (configButton)
	{
		ImGui::Begin("Configuration");
		ImGui::TextWrapped("FPS information:");
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
		
		ImGui::TextWrapped("---------------------------------");
		if (fps.size() > 50)
		{
			fps.erase(fps.begin());
		}
		if (fpsms.size() > 50)
		{
			fpsms.erase(fpsms.begin());
		}
		// Hardware
		ImGui::TextWrapped("Hardware information: ");
		ImGui::Text("CPU cores: %d (Cache: %d Kb)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());
		ImGui::Text("System RAM: %d Mb", SDL_GetSystemRAM());
		ImGui::Text("GPU Vendor: %s", glGetString(GL_VENDOR));
		ImGui::Text("GPU Model: %s", glGetString(GL_RENDERER));
		ImGui::End();
	}
	
	
	if (windowButton)
	{
		App->window->ShowWindowUI();
	}
	if (propertiesButton)
	{
		ImGui::Begin("Properties");
		if (ImGui::CollapsingHeader("Transformation")) 
		{
			
		}
		if (ImGui::CollapsingHeader("Geometry")) 
		{
			ImGui::TextWrapped("Geometry: ");
		}
		if (ImGui::CollapsingHeader("Texture")) 
		{
			ImGui::Text("Texture width: %d", App->modelLoader->width);
			ImGui::Text("Texture height %d", App->modelLoader->height);
		}
		
		ImGui::End();
	}
	ImGui::Begin("LOG");
	ImGui::TextUnformatted(buffer.begin());
	ImGui::End();
	return UPDATE_CONTINUE;
}

update_status ModuleImgui::PostUpdate()
{
	//update a window with OpenGL rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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
void ModuleImgui::AddLog(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	buffer.appendfv(fmt, args);
	va_end(args);
	scrollToBottom = true;
}