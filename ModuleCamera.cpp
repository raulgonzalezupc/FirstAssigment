#include "ModuleCamera.h"
#include "Globals.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleImgui.h"
#include "ModuleInput.h"
#include "ModuleModelLoader.h"
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
	aspect = App->window->width / App->window->height;
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
	App->imgui->AddLog("\n------ Module Camera Init ------\n ");
	App->imgui->AddLog("Setting up the projection, view and model. ");

	return true;
}

update_status ModuleCamera::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleCamera::Update()
{
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
	generateMatrices();

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
	generateMatrices();

}


void ModuleCamera::Orbit(char axis, float movement) {
	

	if (axis == 'X') {
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

void ModuleCamera::ShowCameraUI()
{
	//change FOV
	float f2 = App->camera->frustum.verticalFov;
	ImGui::SliderFloat("Fov ", &f2, 0.01f, 3.12F, "%.2f", 2.0f);
	App->camera->setFOV(f2);

	//camera position
	ImGui::Text("Camera X position: %.2f", App->renderer->cam->frustum.pos[0]);
	ImGui::Text("Camera Y position: %.2f", App->renderer->cam->frustum.pos[1]);
	ImGui::Text("Camera Z position: %.2f", App->renderer->cam->frustum.pos[2]);
}