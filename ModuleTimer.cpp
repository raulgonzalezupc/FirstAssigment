#include "ModuleTimer.h"
#include "SDL.h"
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
	return realTime;
}