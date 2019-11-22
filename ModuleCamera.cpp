#include "ModuleCamera.h"
#include "Globals.h"
#include "ModuleWindow.h"
#include "ModuleImgui.h"
#include "ModuleInput.h"
#include "ModuleModelLoader.h"
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
	model = float4x4::FromTRS(float3(0.0F, -5.0F, 20.0F), float3x3::RotateY(0.0F), float3(1.0F, 1.0F, 1.0F));
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

	if (keyboard[SDL_SCANCODE_Q])
	{
		MoveUp();
	}
	if (keyboard[SDL_SCANCODE_E])
	{
		MoveDown();
	}
	if (keyboard[SDL_SCANCODE_W])
	{
		MoveForward();
	}
	if (keyboard[SDL_SCANCODE_S])
	{
		MoveBackwards();
	}
	if (keyboard[SDL_SCANCODE_A])
	{
		MoveLeft();
	}
	if (keyboard[SDL_SCANCODE_D])
	{
		MoveRight();
	}
	if (keyboard[SDL_SCANCODE_F])
	{
		App->modelLoader->computeModelBoundingBox();
		focusCameraToNewPoint(App->modelLoader->correctCameraPositionForModel);
	}
	if (keyboard[SDL_SCANCODE_UP])
	{
		Rotate('X', 1.0f);
	}
	if (keyboard[SDL_SCANCODE_DOWN])
	{
		Rotate('X', -1.0f);
	}
	if (keyboard[SDL_SCANCODE_LEFT])
	{
		Rotate('Y', -1.0f);
	}
	if (keyboard[SDL_SCANCODE_RIGHT])
	{
		Rotate('Y', 1.0f);
	}
	if (keyboard[SDL_SCANCODE_LSHIFT])
	{
		multSpeed();
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


void ModuleCamera::SetNearPlaneDistance(const float nearDist)
{
	frustum.nearPlaneDistance = nearDist;
}

void ModuleCamera::SetFarPlaneDistance(const float farDist)
{
	frustum.farPlaneDistance = farDist;
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
void ModuleCamera::Rotate(char axis, float movement)
{
	if (axis=='X')
	{
		rotation_matrix = float3x3::RotateY(movement * rot_speed);
		frustum.up = rotation_matrix * frustum.up;
		frustum.front = rotation_matrix * frustum.front;

	}
	if (axis == 'Y')
	{
		const float current_angle = asinf(frustum.front.y / frustum.front.Length());
		if (abs(current_angle + movement * rot_speed) >= math::pi / 2) {
			return;
		}
		rotation_matrix = float3x3::identity;
		rotation_matrix.SetRotatePart(frustum.WorldRight(), movement * rot_speed);
		frustum.up = rotation_matrix * frustum.up;
		frustum.front = rotation_matrix * frustum.front;
	}
	generateMatrices();
}

void ModuleCamera::LookAt(const float3 focus)
{
	float3 look_direction = (focus - frustum.pos).Normalized();
	SetOrientation(look_direction);

	generateMatrices();
}
void ModuleCamera::LookAt(const float x, const float y, const float z)
{
	LookAt(float3(x, y, z));
}
void ModuleCamera::SetOrientation(const float3 orientation)
{
	float3 direction = (orientation - frustum.pos).Normalized();
	float3x3 rotation = float3x3::LookAt(frustum.front, direction, frustum.up, float3::unitY);
	frustum.front = rotation.Transform(frustum.front).Normalized();
	frustum.up = rotation.Transform(frustum.up).Normalized();
	generateMatrices();
}


void ModuleCamera::multSpeed()
{
	speed = 2.0f;
}

void ModuleCamera::generateMatrices()
{
	proj = frustum.ProjectionMatrix();
	view = frustum.ViewMatrix();
}


void ModuleCamera::focusCameraToNewPoint(const float3 & newPos)
{
	frustum.pos = newPos;
	LookAt(App->modelLoader->modelCenter);

	//in case the orbit is going, set up the old model
	model = float4x4::FromTRS(float3(0.0F, -5.0F, 20.0F), float3x3::RotateY(0.0F), float3(1.0F, 1.0F, 1.0F)); 
	generateMatrices();

}


void ModuleCamera::Orbit(char axis, float movement) {
	

	if (axis == 'X') {
		pitch = 0;
		yaw = rot_speed * speed;

		if (movement < 0) {
			model = model * math::float3x3::RotateY(-yaw);
			frustum.pos = frustum.pos * math::float3x3::RotateY(-yaw);
		}
		else {
			model = model * math::float3x3::RotateY(yaw);
			frustum.pos = frustum.pos * math::float3x3::RotateY(yaw);
		}
	}
	else if (axis == 'Y') {
		yaw = 0;
		pitch = rot_speed * speed;

		if (movement < 0) {
			model = model * math::float3x3::RotateAxisAngle(frustum.WorldRight(), pitch);
			frustum.pos = frustum.pos *math::float3x3::RotateAxisAngle(frustum.WorldRight(), pitch);
		
		}
		else {
			model = model * math::float3x3::RotateAxisAngle(frustum.WorldRight(), -pitch);
			frustum.pos = frustum.pos * math::float3x3::RotateAxisAngle(frustum.WorldRight(), -pitch);
		}	
	}
	generateMatrices();
}