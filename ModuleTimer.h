#ifndef _MODULETIMER_H_
#define _MODULETIMER_H_

#include "Module.h"
#include "Globals.h"
#include "Application.h"
#include "SDL.h"
#define FRAME_VALUES 10
class ModuleTimer : public Module {

public:
	ModuleTimer();
	virtual ~ModuleTimer();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	

	void StartRealTimeClock();
	void StartGameTimeClock();
	float Read();
	void Pause();
	void Stop();
	void ShowTimerUI();
	bool isStarted();
	bool isPaused();
	void Unpause();

public: 
	float fpsrate = 60.0f;


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

	Uint32 mStartTicks;

	//The ticks stored when the timer was paused
	Uint32 mPausedTicks;

	//The timer status
	bool mPaused;
	bool mStarted;

};

#endif