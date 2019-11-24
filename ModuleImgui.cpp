#include "ModuleImgui.h"
#include "Application.h"
#include "Globals.h"
#include "ModuleWindow.h"
#include "ModuleCamera.h"
#include "ModuleTexture.h"
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
		ImGui::TextWrapped("HARDWARE INFO \n");
		ImGui::Text("CPU cores: %d (Cache: %d Kb)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());
		ImGui::Text("System RAM: %d Mb", SDL_GetSystemRAM());
		ImGui::Text("GPU Vendor: %s", glGetString(GL_VENDOR));
		ImGui::Text("GPU Model: %s", glGetString(GL_RENDERER));
		ImGui::TextWrapped("---------------------------------");
		//mouse info
		ImGui::TextWrapped("MOUSE INFO \n");
		if (ImGui::IsMousePosValid())
			ImGui::Text("Mouse pos: (%g, %g)", io.MousePos.x, io.MousePos.y);
			ImGui::Text("Mouse delta: (%g, %g)", io.MouseDelta.x, io.MouseDelta.y);
			ImGui::Text("Mouse down:");     for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (io.MouseDownDuration[i] >= 0.0f) { ImGui::SameLine(); ImGui::Text("b%d (%.02f secs)", i, io.MouseDownDuration[i]); }
			ImGui::Text("Mouse clicked:");  for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseClicked(i)) { ImGui::SameLine(); ImGui::Text("b%d", i); }
			ImGui::Text("Mouse dbl-clicked:"); for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseDoubleClicked(i)) { ImGui::SameLine(); ImGui::Text("b%d", i); }
			ImGui::Text("Mouse released:"); for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseReleased(i)) { ImGui::SameLine(); ImGui::Text("b%d", i); }
			ImGui::Text("Mouse wheel: %.1f", io.MouseWheel);
		ImGui::End();
	}
	
	
	if (windowButton)
	{
		ImGui::Begin("Window Settings");
		App->window->ShowWindowUI();
		ImGui::Checkbox("Quit Game", &quit);
		if (quit)
		{
			return UPDATE_STOP;
		}
		ImGui::End();
	}
	if (propertiesButton)
	{
		ImGui::Begin("Properties");
		if (ImGui::CollapsingHeader("Transformation")) 
		{
			static float position[3] = { 0.0F, 0.0F, 0.0F };
			static float rotation[3] = { 0.0F, 0.0F, 0.0F };
			static float scale[3] = { 1.0F, 1.0F, 1.0F };
			ImGui::DragFloat3("Position", position);
			ImGui::DragFloat3("Rotation", rotation);
			ImGui::DragFloat3("Scale", scale);
		}
		if (ImGui::CollapsingHeader("Geometry")) 
		{
			ImGui::Text("Number of vertices: %d", App->modelLoader->numVertices);
			ImGui::Text("Number of meshes: %d", App->modelLoader->numMeshes);
		}
		if (ImGui::CollapsingHeader("Texture")) 
		{
			ImGui::Text("Texture id: %d", App->modelLoader->textureId);
			ImGui::Text("Texture width: %d", App->modelLoader->textureWidth);
			ImGui::Text("Texture height: %d", App->modelLoader->textureHeight);
			ImGui::Text("Texture type: %s", App->modelLoader->textureType);

			//Show the model with only the mesh
			ImGui::Checkbox("Show Mesh", &meshed);
			if (meshed) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}

			//texture image
			ImGui::Image((void*)(intptr_t)App->texture->Texture.id, ImVec2(200 * 0.5f, 200 * 0.5f), ImVec2(0, 1), ImVec2(1, 0));
			
			
			
		}
		
		ImGui::End();
	}
	if (consoleButton)
	{
		ImGui::Begin("CONSOLE");
		ImGui::TextUnformatted(buffer.begin());
		ImGui::End();
	}
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