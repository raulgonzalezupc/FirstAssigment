#include "ModuleImgui.h"
#include "Application.h"
#include "Globals.h"
#include "ModuleWindow.h"
#include "ModuleCamera.h"
#include "ModuleTexture.h"
#include "ModuleInput.h"
#include "GameObject.h"
#include "ModuleModelLoader.h"
#include "ModuleScene.h"
#include "ModuleRender.h"
#include "ModuleTimer.h"
#include "glew/include/GL/glew.h"
#include "Components/ComponentMesh.h"
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

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;


	SDL_VERSION(&compiled);
	SDL_GetVersion(&linked);
	
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
	
	App->timer->ShowTimerUI();

	
	GameObject* game = App->scene->root->FindChild("Game");
	GameObject* scene = App->scene->root->FindChild("Scene");

	

	App->scene->camGame->Draw(game->name);
	App->scene->camScene->Draw(scene->name);

	if (showHierarchy) {
		ImGui::Begin("GameObjects Hierarchy", &showHierarchy);
		if (ImGui::IsWindowHovered() && GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)
		{
			ImGui::OpenPopup("RightClick");
		}
		if (ImGui::BeginPopup("RightClick"))
		{
			if (ImGui::Selectable("Create Empty"))
			{
				GameObject* newGO = new GameObject("New game object");
				App->scene->root->children.push_back(newGO);
				newGO->parent = App->scene->root;
			}
			ImGui::Separator();

			if (ImGui::Selectable("Rename"))
			{
				//TODO: Rename gameobjects
			}
			
			ImGui::EndPopup();
			
			
		}
		if (ImGui::TreeNode(App->scene->root->name)) {
			int root = 0;
			if (ImGui::Button("Create Empty Game Object")) {
				GameObject* newGO = new GameObject("New game object");
				App->scene->root->children.push_back(newGO);
				newGO->parent = App->scene->root;
			}
			DrawHierarchy(App->scene->root->children, root);
			ImGui::TreePop();
		}
		
		ImGui::End();
	}
	

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
		ShowConfigurationUI();
	}
	
	
	if (windowButton)
	{
		ImGui::Begin("Window Settings");
		App->window->ShowWindowUI();
		
		ImGui::End();
	}
	if (propertiesButton)
	{
		App->modelLoader->ShowModelUI();
	}
	if (consoleButton)
	{
		ImGui::Begin("CONSOLE");
		ImGui::TextUnformatted(buffer.begin());
		ImGui::End();
	}
	if(aboutButton)
	{
		ShowAboutUI();
		if (quit)
		{
			return UPDATE_STOP;
		}
	}
	return UPDATE_CONTINUE;
}


const void ModuleImgui::DrawHierarchy(const std::vector<GameObject*>& objects, int& index) {
	for (unsigned i = 0; i < objects.size(); ++i)
	{
		++index;
		unsigned flags = ImGuiTreeNodeFlags_None;
		if (objects[i]->children.size() == 0) {
			flags = ImGuiTreeNodeFlags_Leaf;
		}
		flags |= index == selected ? ImGuiTreeNodeFlags_Selected : 0;
		if (ImGui::TreeNodeEx(objects[i]->name, flags)) {
			if (selected == index) {
				objects[i]->ShowProperties();
			}
			if (ImGui::IsItemClicked()) {
				selected = index;
			}						
			DrawHierarchy(objects[i]->children, index);
			if (ImGui::Button("Create Empty Game Object")) {
			GameObject* newGO = new GameObject("New game object");
			objects[i]->children.push_back(newGO);
			newGO->parent = objects[i];
		}
		if (ImGui::Button("Delete this game object")) {
			GameObject* parent = objects[i]->parent;
			parent->DeleteChild(objects[i]);
		}
			ImGui::TreePop();
		}
		
	}
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

void ModuleImgui::ShowAboutUI() 
{
	ImGui::Begin("About");
	

	//Github link
	ImGui::Text("GitHub link to the source code: ");
	if (ImGui::Button("GitHub")) {
		ShellExecuteA(nullptr, "open", "https://github.com/raulgonzalezupc/FirstAssigment", nullptr, nullptr, SW_SHOWNORMAL);
	}
	ImGui::TextWrapped("---------------------------------");


	//Engine info
	ImGui::Text("Information avout Engine: ");
	if (ImGui::Button("Engine info")) {
		ShellExecuteA(nullptr, "open", "https://github.com/raulgonzalezupc/FirstAssigment/blob/master/README.md", nullptr, nullptr, SW_SHOWNORMAL);
	}
	ImGui::TextWrapped("---------------------------------");

	//License
	ImGui::Text("Link to License of the project: ");
	if (ImGui::Button("License")) {
		ShellExecuteA(nullptr, "open", "https://github.com/raulgonzalezupc/FirstAssigment/blob/master/LICENSE", nullptr, nullptr, SW_SHOWNORMAL);
	}
	ImGui::TextWrapped("---------------------------------");

	//LinkedIn
	ImGui::Text("Link to LinkedIn of the coder: ");
	if (ImGui::Button("LinkedIn")) {
		ShellExecuteA(nullptr, "open", "https://www.linkedin.com/in/ra%C3%BAl-gonz%C3%A1lez-cebrian-a6a562180/", nullptr, nullptr, SW_SHOWNORMAL);
	}
	ImGui::TextWrapped("---------------------------------");





	ImGui::Checkbox("Quit Game", &quit);

	ImGui::End();
}

void ModuleImgui::ShowConfigurationUI()
{
	ImGui::Begin("Configuration");
	ImGuiIO& io = ImGui::GetIO();
	if (ImGui::CollapsingHeader("FPS"))
	{
		char title[25];
		char titleMs[25];
		
		fps.push_back(io.Framerate);

		fpsms.push_back(1000.0f / io.Framerate);
		sprintf_s(title, 25, "Framerate %.1f", fps[fps.size() - 1]);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::PlotHistogram("##Framerate", &fps[0], fps.size(), 0, title, 0.0F, 100.0F, ImVec2(310, 100));
		sprintf_s(title, 25, "Miliseconds %.1f", fpsms[fpsms.size() - 1]);
		ImGui::PlotHistogram("##Miliseconds", &fpsms[0], fpsms.size(), 0, title, 0.0F, 50.0F, ImVec2(310, 100));

		if (fps.size() > 50)
		{
			fps.erase(fps.begin());
		}
		if (fpsms.size() > 50)
		{
			fpsms.erase(fpsms.begin());
		}
		
	}
	
	// Hardware
	if (ImGui::CollapsingHeader("Hardware"))
	{
		ImGui::Text("CPU cores: %d (Cache: %d Kb)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());
		ImGui::Text("System RAM: %d Mb", SDL_GetSystemRAM());
		ImGui::Text("GPU Vendor: %s", glGetString(GL_VENDOR));
		ImGui::Text("GPU Model: %s", glGetString(GL_RENDERER));
	}
	
	
	//Software
	if (ImGui::CollapsingHeader("Software"))
	{
		ImGui::Text("IMGUI VERSION: 1.73 ");
		ImGui::Text("GLEW VERSION: 2.1.0 ");
		ImGui::Text("DEVIL VERSION: 1.8.0 ");
		ImGui::Text("We compiled against SDL version %d.%d.%d\n", compiled.major, compiled.minor, compiled.patch);
		ImGui::Text("But we are linking against SDL version %d.%d.%d.\n", linked.major, linked.minor, linked.patch);
	}
	
	//mouse info
	if (ImGui::CollapsingHeader("Mouse Info"))
	{
		if (ImGui::IsMousePosValid())
		ImGui::Text("Mouse pos: (%g, %g)", io.MousePos.x, io.MousePos.y);
		ImGui::Text("Mouse delta: (%g, %g)", io.MouseDelta.x, io.MouseDelta.y);
		ImGui::Text("Mouse down:");     for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (io.MouseDownDuration[i] >= 0.0f) { ImGui::SameLine(); ImGui::Text("b%d (%.02f secs)", i, io.MouseDownDuration[i]); }
		ImGui::Text("Mouse clicked:");  for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseClicked(i)) { ImGui::SameLine(); ImGui::Text("b%d", i); }
		ImGui::Text("Mouse dbl-clicked:"); for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseDoubleClicked(i)) { ImGui::SameLine(); ImGui::Text("b%d", i); }
		ImGui::Text("Mouse released:"); for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseReleased(i)) { ImGui::SameLine(); ImGui::Text("b%d", i); }
		ImGui::Text("Mouse wheel: %.1f", io.MouseWheel);
	}
	

	//Memory used
	if (ImGui::CollapsingHeader("Memory"))
	{
		glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &vram_budget);
		glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &vram_available);

		ImGui::Text("VRAM Budget:");
		ImGui::Text("%.2f", vram_budget / 1000.0f);


		ImGui::Text("VRAM Usage:");
		ImGui::Text("%.2f", (vram_budget - vram_available) / 1000.0f);


		ImGui::Text("VRAM Available:");
		ImGui::Text("%.2f", vram_available / 1000.0f);
	}

	if (ImGui::CollapsingHeader("Render"))
	{
		App->renderer->ShowRenderUI();
	}
	if (ImGui::CollapsingHeader("Camera"))
	{
		App->camera->ShowCameraUI();
	}
	ImGui::End();


}