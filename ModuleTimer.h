#ifndef _MODULETIMER_H_
#define _MODULETIMER_H_

#include "Module.h"
#include "Globals.h"
#include "Application.h"

class ModuleTimer : public Module {

public:
	ModuleTimer();
	virtual ~ModuleTimer();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	
	float StartRealTimeClock();
	void ShowTimerUI();

	float Stop();

	void Resume();
	float Pause();
	bool IsPaused() const;

	float Read() const;

private:
	
	float pause_start_ticks = 0;
	float elapsed_while_paused_ticks = 0;

	bool running = false;
	bool paused = false;
	float end_time = NULL;

	int realTime = NULL;
	int gameTime = NULL;



};

#endif