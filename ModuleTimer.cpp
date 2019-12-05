#include "ModuleTimer.h"
#include "ModuleImgui.h"
#include "SDL.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl.h"

ModuleTimer::ModuleTimer()
{

}

ModuleTimer::~ModuleTimer()
{

}

bool ModuleTimer::Init()
{
	App->imgui->AddLog("------- Module Timer Init --------");
	
	return true;
}

update_status ModuleTimer::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleTimer::Update()
{
	return UPDATE_CONTINUE;
}

update_status ModuleTimer::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleTimer::CleanUp()
{
	return true;
}

float ModuleTimer::StartRealTimeClock()
{
	realTime=SDL_GetTicks();
	return realTime;
}
float ModuleTimer::StartGameTimeClock()
{
	gameTime = SDL_GetTicks();
	return gameTime;
}

float ModuleTimer::Read() 
{

	if (running)
	{
		if (paused)
		{
			gameTime += (pause_start_ticks - gameTime) - timePassedWhilePaused;
		}
		else
		{
			gameTime += (SDL_GetTicks() - gameTime) - timePassedWhilePaused;
		}
	}
	else
	{
		gameTime = end_time;
	}

	return gameTime;
}

void ModuleTimer::Resume()
{
	if (paused)
	{
		timePassedWhilePaused += SDL_GetTicks() - pause_start_ticks;
		paused = false;
	}
}

float ModuleTimer::Pause()
{
	if (!paused)
	{
		pause_start_ticks = SDL_GetTicks();
		paused = true;
	}
	else {
		paused = false;
	}
	return pause_start_ticks - gameTime - timePassedWhilePaused;
}

float ModuleTimer::Stop()
{
	end_time = SDL_GetTicks() - gameTime - timePassedWhilePaused;
	running = false;

	return end_time;
}

void ModuleTimer::ShowTimerUI()
{
	StartRealTimeClock();
	ImGui::Begin("Clock");
	if (!paused)
	{
		StartGameTimeClock(); 
		gameTime -= timePaused;
	}
	if (ImGui::Button("Pause"))
	{
		Pause();
		timePaused = Stop();
	}

	if (ImGui::Button("Stop"))
	{
		gameTime = 0;
		stop = true;
	}
	if (ImGui::Button("Advance")) {

	}
	
	
	ImGui::SliderFloat("Fps Rate ", &fpsrate, 10.0f, 60.0F, "%.2f", 1.0f);
	SDL_Delay(20); // 500 should make 2 frames per second.
	ImGui::Text("Real time:  %d:%d:%d", (realTime / (1000 * 60 * 60)) % 24, (realTime / (1000 * 60)) % 60, (realTime / 1000) % 60);
	ImGui::Text("Game time:  %d:%d:%d", (gameTime / (1000 * 60 * 60)) % 24, (gameTime / (1000 * 60)) % 60, (gameTime / 1000) % 60);
	
	
	ImGui::End();
}
