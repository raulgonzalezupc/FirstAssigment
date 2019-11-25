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


public: 
	int realTime = NULL;



};

#endif