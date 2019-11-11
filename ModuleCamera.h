#ifndef _MODULECAMERA_H_
#define _MODULECAMERA_H_

#include "Module.h"
#include "Globals.h"
#include "MathGeoLib/include/Math/float4x4.h"

class ModuleCamera : public Module
{
public:
	ModuleCamera();
	~ModuleCamera();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

public:
	float aspect = SCREEN_WIDTH / SCREEN_HEIGHT;
	float4x4 proj;
	float4x4 view;
	float4x4 model;

public:
	float buffer_data[20] = {
	 0.5f,  0.5f, 0.0f,  // top right
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f,   // top left 
	 1.0F, 1.0F,
	 1.0F, 0.0F,
	 0.0F, 0.0F,
	 0.0F, 1.0F
	};
	unsigned int indices[6] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

};

#endif