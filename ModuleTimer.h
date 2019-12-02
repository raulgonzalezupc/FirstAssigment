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
	float StartGameTimeClock();
	float Read() const;
	float Pause();
	float Stop();
	void Resume();
	void ShowTimerUI();

public: 
	int gameTime = NULL;
	int realTime = NULL;
	bool running = true;
	float pauseTime = 0;
	float readTime = 0;
	float pause_start_ticks = 0;
	float start_ticks = 0;
	float timePassedWhilePaused = 0;
	float end_time =0 ;
	float timePaused = 0;
	bool paused = false;
	float timeStoped = NULL;
	bool stop = false;



};

#endif