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
	gameTime = SDL_GetTicks();
	running = true;
	return realTime;
}



float ModuleTimer::Read() const
{
	float current_time;

	if (running)
	{
		if (paused)
		{
			current_time = (pause_start_ticks - gameTime) - elapsed_while_paused_ticks;
		}
		else
		{
			current_time = (SDL_GetTicks() - gameTime) - elapsed_while_paused_ticks;
		}

	}
	else
	{
		current_time = end_time;
	}

	return current_time;
}

void ModuleTimer::Resume()
{
	if (paused)
	{
		elapsed_while_paused_ticks += SDL_GetTicks() - pause_start_ticks;
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
	return pause_start_ticks - gameTime - elapsed_while_paused_ticks;
}

bool ModuleTimer::IsPaused() const
{
	return paused;
}

float ModuleTimer::Stop()
{
	end_time = SDL_GetTicks() - gameTime - elapsed_while_paused_ticks;
	running = false;

	return end_time;
}



void ModuleTimer::ShowTimerUI()
{
	StartRealTimeClock();
	ImGui::Begin("Clock");
	ImGui::Text("Real time:  %d:%d:%d", (realTime / (1000 * 60 * 60)) % 24, (realTime / (1000 * 60)) % 60, (realTime / 1000) % 60);
	ImGui::End();
}