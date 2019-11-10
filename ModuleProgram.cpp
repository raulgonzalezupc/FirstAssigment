#include "Globals.h"
#include "Application.h"
#include "ModuleProgram.h"

ModuleProgram::ModuleProgram()
{
}

// Destructor
ModuleProgram::~ModuleProgram()
{
}

// Called before render is available
bool ModuleProgram::Init()
{
	return true;
}

update_status ModuleProgram::PreUpdate()
{
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleProgram::Update()
{
	return UPDATE_CONTINUE;
}

update_status ModuleProgram::PostUpdate()
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleProgram::CleanUp()
{
	return true;
}
