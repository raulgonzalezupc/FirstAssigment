#include "ModuleCamera.h"
#include "Globals.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "MathGeoLib/include/Math/float4.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/float4x4.h"
#include "MathGeoLib/include/Math/float3x3.h"
#include "MathGeoLib/include/Geometry/Frustum.h"
#include "MathGeoLib.h"



ModuleCamera::ModuleCamera()
{

}

ModuleCamera::~ModuleCamera()
{

}

bool ModuleCamera::Init()
{

	frustum.type = FrustumType::PerspectiveFrustum;
	aspect = SCREEN_WIDTH / SCREEN_HEIGHT;
	frustum.pos = float3::unitX;
	frustum.front = float3::unitZ;
	frustum.up = float3::unitY;

	frustum.nearPlaneDistance = 1.0F;
	frustum.farPlaneDistance = 100.0F;
	frustum.verticalFov = PI / 4.0F;
	frustum.horizontalFov = 2.0F*atanf(tanf(frustum.verticalFov*0.5F)*aspect);

	//setting up our proj
	proj = frustum.ProjectionMatrix();
	model = float4x4::FromTRS(float3(0.0F, 0.0F, 0.0F), float3x3::RotateX(0.0F), float3(1.0F, 1.0F, 1.0F));
	view = frustum.ViewMatrix();
	float4x4 transform = proj * view * float4x4(model);


	//translation
	float4 vertex0(buffer_data[0], buffer_data[1], buffer_data[2], 1.0F);
	float4 tmp0 = transform * vertex0;
	float3 finalVertex0(tmp0.x / tmp0.w, tmp0.y / tmp0.w, tmp0.z / tmp0.w);
	buffer_data[0] = finalVertex0.x;
	buffer_data[1] = finalVertex0.y;
	buffer_data[2] = finalVertex0.z;

	float4 vertex1(buffer_data[3], buffer_data[4], buffer_data[5], 1.0F);
	float4 tmp1 = transform * vertex1;
	float3 finalVertex1(tmp1.x / tmp1.w, tmp1.y / tmp1.w, tmp1.z / tmp1.w);
	buffer_data[3] = finalVertex1.x;
	buffer_data[4] = finalVertex1.y;
	buffer_data[5] = finalVertex1.z;

	float4 vertex2(buffer_data[6], buffer_data[7], buffer_data[8], 1.0F);
	float4 tmp2 = transform * vertex2;
	float3 finalVertex2(tmp2.x / tmp2.w, tmp2.y / tmp2.w, tmp2.z / tmp2.w);
	buffer_data[6] = finalVertex2.x;
	buffer_data[7] = finalVertex2.y;
	buffer_data[8] = finalVertex2.z;

	float4 vertex3(buffer_data[9], buffer_data[10], buffer_data[11], 1.0F);
	float4 tmp3 = transform * vertex3;
	float3 finalVertex3(tmp3.x / tmp3.w, tmp3.y / tmp3.w, tmp3.z / tmp3.w);
	buffer_data[9] = finalVertex3.x;
	buffer_data[10] = finalVertex3.y;
	buffer_data[11] = finalVertex3.z;

	return true;
}

update_status ModuleCamera::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleCamera::Update()
{
	SDL_PumpEvents();
	keyboard = SDL_GetKeyboardState(NULL);
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if(event.type == SDL_MOUSEMOTION) {
			if (event.motion.state & SDL_BUTTON_RMASK) { //if user press right click
				if (math::Abs(event.motion.xrel) > 1.5) {
					App->camera->MouseXMotion(event.motion.xrel); //passing the relative motion in the X direction
				}
				if (math::Abs(event.motion.yrel) > 1.5) {
					App->camera->MouseYMotion(event.motion.yrel); //passing the relative motion in the Y direction
				}
			}
		}
		if (event.type == SDL_MOUSEWHEEL) {
			if (event.wheel.y > 0) // scroll up
			{
				MoveForward();
				LOG("up");
			}
			else if (event.wheel.y < 0) // scroll down
			{
				MoveBackwards();
				LOG("up");
			}
		}
	}
/*
			case SDL_KEYDOWN:
				if (keyboard[SDL_SCANCODE_Q])
				{
					App->camera->MoveUp();
				}
				if (keyboard[SDL_SCANCODE_E])
				{
					App->camera->MoveDown();
				}
				if (keyboard[SDL_SCANCODE_W])
				{
					App->camera->MoveForward();
				}
				if (keyboard[SDL_SCANCODE_S])
				{
					App->camera->MoveBackwards();
				}
				if (keyboard[SDL_SCANCODE_A])
				{
					App->camera->MoveLeft();
				}
				if (keyboard[SDL_SCANCODE_D])
				{
					App->camera->MoveRight();
				}
			break;
		}

	}
	*/
	if (keyboard[SDL_SCANCODE_Q])
	{
		App->camera->MoveUp();
	}
	if (keyboard[SDL_SCANCODE_E])
	{
		App->camera->MoveDown();
	}
	if (keyboard[SDL_SCANCODE_W])
	{
		App->camera->MoveForward();
	}
	if (keyboard[SDL_SCANCODE_S])
	{
		App->camera->MoveBackwards();
	}
	if (keyboard[SDL_SCANCODE_A])
	{
		App->camera->MoveLeft();
	}
	if (keyboard[SDL_SCANCODE_D])
	{
		App->camera->MoveRight();
	}
	if (keyboard[SDL_SCANCODE_LSHIFT])
	{
		App->camera->multSpeed();
	}
	else {
		speed = 1.0f;
	}
	
	return UPDATE_CONTINUE;
}

update_status ModuleCamera::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleCamera::CleanUp()
{
	return true;
}

void ModuleCamera::SetAspectRatio(const float aspect_ratio)
{
	frustum.horizontalFov = 2.0F*atanf(tanf(frustum.verticalFov*0.5F)*aspect_ratio);
	proj = frustum.ProjectionMatrix();
}

void ModuleCamera::setFOV(const float fov)
{
	frustum.verticalFov = fov;
	frustum.horizontalFov = 2.0F*atanf(tanf(frustum.verticalFov*0.5F)*aspect);
	proj = frustum.ProjectionMatrix();
}

void ModuleCamera::Position(const float3 position)
{
	frustum.pos = position;
	proj = frustum.ProjectionMatrix();
}



void ModuleCamera::MoveUp()
{
	frustum.pos.y += distance;
	view = frustum.ViewMatrix();
}
void ModuleCamera::MoveDown()
{
	frustum.pos.y -= distance;
	view = frustum.ViewMatrix();
}
void ModuleCamera::MoveForward()
{
	frustum.pos += frustum.front.ScaledToLength(distance)*speed;
	view = frustum.ViewMatrix();
}
void ModuleCamera::MoveBackwards()
{
	frustum.pos -= frustum.front.ScaledToLength(distance)*speed;
	view = frustum.ViewMatrix();
}

void ModuleCamera::MoveLeft()
{
	frustum.pos -= frustum.WorldRight().ScaledToLength(distance);
	view = frustum.ViewMatrix();
}

void ModuleCamera::MoveRight()
{
	frustum.pos += frustum.WorldRight().ScaledToLength(distance);
	view = frustum.ViewMatrix();

}

void ModuleCamera::MouseXMotion(const float x_motion)
{
	bool correct = false;
	const float angle = x_motion * -0.01f;
	if (!correct)
	{
		
		float3x3 rotation_matrix = float3x3::RotateY(angle);
		frustum.up = rotation_matrix * frustum.up;
		frustum.front = rotation_matrix * frustum.front;
		frustum.pos = rotation_matrix * frustum.pos;
		proj = frustum.ProjectionMatrix();
		view = frustum.ViewMatrix();
	}
	else {
		float3x3 rotation_matrix = float3x3::RotateY(angle);
		frustum.up = rotation_matrix * frustum.up;
		frustum.front = rotation_matrix * frustum.front;
		proj = frustum.ProjectionMatrix();
		view = frustum.ViewMatrix();
	}
}
void ModuleCamera::MouseYMotion(const float y_motion)
{
	bool correct = false;
	const float angle = y_motion * -0.1f;
	if (correct)
	{
		
		float3x3 rotation_matrix = float3x3::RotateX(angle);
		frustum.up = rotation_matrix * frustum.up;
		frustum.front = rotation_matrix * frustum.front;
		frustum.pos = rotation_matrix * frustum.pos;
		proj = frustum.ProjectionMatrix();
		view = frustum.ViewMatrix();
	}
	else {
		float3x3 rotation_matrix = float3x3::RotateX(angle);
		frustum.up = rotation_matrix * frustum.up;
		frustum.front = rotation_matrix * frustum.front;
		proj = frustum.ProjectionMatrix();
		view = frustum.ViewMatrix();

	}
}

void ModuleCamera::multSpeed()
{
	speed = 5.0f;
}

