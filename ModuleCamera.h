#ifndef _MODULECAMERA_H_
#define _MODULECAMERA_H_

#include "Module.h"
#include "Globals.h"
#include "MathGeoLib/include/Math/float4x4.h"
#include "MathGeoLib/include/Geometry/Frustum.h"

class ModuleCamera : public Module
{
public:
	ModuleCamera();
	virtual ~ModuleCamera();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

public:
	void SetAspectRatio(const float aspect_ratio);
	void setFOV(const float fov);
	void SetPlaneDistances();
	void Position(const float3 position);
	void Orentiatin();
	void LookAt(const float x, const float y, const float z);
	void GetProjectionMatrix();
	void GetViewMatrix();
	void MoveUp();
	void MoveDown();
	void MoveForward();
	void MoveBackwards();
	void MoveLeft();
	void MoveRight();
	void MouseXMotion(const float x_motion);
	void MouseYMotion(const float y_motion);
public:
	Frustum frustum;
	float aspect;
	float4x4 proj;
	float4x4 view;
	float4x4 model;
	float yaw = -90.0f;
	float pitch = 0;

public:
	float3 new_camera_pos;
	const float distance = 0.25f * 1.0F;
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