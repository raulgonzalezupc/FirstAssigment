#include <stdlib.h>
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleImgui.h"
#include "Globals.h"

#include "SDL/include/SDL.h"
#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )

enum main_states
{
	MAIN_CREATION,
	MAIN_START,
	MAIN_UPDATE,
	MAIN_FINISH,
	MAIN_EXIT
};

Application* App = NULL;

int main(int argc, char ** argv)
{
	int main_return = EXIT_FAILURE;
	main_states state = MAIN_CREATION;

	while (state != MAIN_EXIT)
	{
		switch (state)
		{
		case MAIN_CREATION:

			
			App = new Application();
			state = MAIN_START;
			App->imgui->AddLog("Application Creation --------------");
			break;

		case MAIN_START:

			App->imgui->AddLog("Application Init --------------");
			if (App->Init() == false)
			{
				App->imgui->AddLog("Application Init exits with error -----");
				state = MAIN_EXIT;
			}
			else
			{
				state = MAIN_UPDATE;
				App->imgui->AddLog("Application Update --------------");
			}

			break;

		case MAIN_UPDATE:
		{
			int update_return = App->Update();

			if (update_return == UPDATE_ERROR)
			{
				App->imgui->AddLog("Application Update exits with error -----");
				state = MAIN_EXIT;
			}

			if (update_return == UPDATE_STOP)
				state = MAIN_FINISH;
		}
			break;

		case MAIN_FINISH:

			App->imgui->AddLog("Application CleanUp --------------");
			if (App->CleanUp() == false)
			{
				App->imgui->AddLog("Application CleanUp exits with error -----");
			}
			else
				main_return = EXIT_SUCCESS;

			state = MAIN_EXIT;

			break;

		}

		
	}

	delete App;
	App->imgui->AddLog("Bye :)\n");
	return main_return;
}
