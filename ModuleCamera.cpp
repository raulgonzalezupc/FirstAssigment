#include "ModuleCamera.h"
#include "Globals.h"
#include "ModuleWindow.h"
#include "ModuleImgui.h"
#include "ModuleInput.h"
#include "MathGeoLib/include/Math/float4.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/float4x4.h"
#include "MathGeoLib/include/Math/float3x3.h"
#include "MathGeoLib/include/Geometry/Frustum.h"
#include "MathGeoLib.h"

#define zoom App->input->GetMouseWheel()->y > 0

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
	model = float4x4::FromTRS(float3(0.0F, 0.0F, 4.0F), float3x3::RotateX(0.0F), float3(1.0F, 1.0F, 1.0F));
	view = frustum.ViewMatrix();
	float4x4 transform = proj * view * float4x4(model);



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
	}

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
	frustum.pos.y += distance * speed;
	view = frustum.ViewMatrix();
}
void ModuleCamera::MoveDown()
{
	frustum.pos.y -= distance * speed;
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
	frustum.pos -= frustum.WorldRight().ScaledToLength(distance)*speed;
	view = frustum.ViewMatrix();
}

void ModuleCamera::MoveRight()
{
	frustum.pos += frustum.WorldRight().ScaledToLength(distance*speed);
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
	speed = 2.0f;
}

