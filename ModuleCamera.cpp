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

void ModuleCamera::SetAspectRatio(const float aspect_ratio, Camera* cam)
{
	cam->frustum.horizontalFov = 2.0F*atanf(tanf(cam->frustum.verticalFov*0.5F)*aspect_ratio);
	cam->proj = cam->frustum.ProjectionMatrix();
}

void ModuleCamera::setFOV(const float fov, Camera* cam)
{
	cam->frustum.verticalFov = fov;
	cam->frustum.horizontalFov = 2.0F*atanf(tanf(cam->frustum.verticalFov*0.5F)*aspect);
	generateMatrices(cam);

}

void ModuleCamera::Position(const float3 position, Camera* cam)
{
	cam->frustum.pos = position;
	cam->proj = cam->frustum.ProjectionMatrix();
}


void ModuleCamera::SetNearPlaneDistance(const float nearDist, Camera* cam)
{
	cam->frustum.nearPlaneDistance = nearDist;
}

void ModuleCamera::SetFarPlaneDistance(const float farDist, Camera* cam)
{
	cam->frustum.farPlaneDistance = farDist;
}

void ModuleCamera::MoveUp(Camera* cam)
{
	cam->frustum.pos.y += distance * speed;
	cam->view = cam->frustum.ViewMatrix();
}
void ModuleCamera::MoveDown(Camera* cam)
{
	cam->frustum.pos.y -= distance * speed;
	cam->view = cam->frustum.ViewMatrix();
}
void ModuleCamera::MoveForward(Camera* cam)
{
	cam->frustum.pos += cam->frustum.front.ScaledToLength(distance)*speed;
	cam->view = cam->frustum.ViewMatrix();
}
void ModuleCamera::MoveBackwards(Camera* cam)
{
	cam->frustum.pos -= cam->frustum.front.ScaledToLength(distance)*speed;
	cam->view = cam->frustum.ViewMatrix();
}

void ModuleCamera::MoveLeft(Camera* cam)
{
	cam->frustum.pos -= cam->frustum.WorldRight().ScaledToLength(distance)*speed;
	cam->view = cam->frustum.ViewMatrix();
}

void ModuleCamera::MoveRight(Camera* cam)
{
	cam->frustum.pos += cam->frustum.WorldRight().ScaledToLength(distance*speed);
	cam->view = cam->frustum.ViewMatrix();

}
void ModuleCamera::Rotate(char axis, float movement, Camera* cam)
{
	if (axis=='X')
	{
		rotation_matrix = float3x3::RotateY(movement * rot_speed);
		cam->frustum.up = rotation_matrix * cam->frustum.up;
		cam->frustum.front = rotation_matrix * cam->frustum.front;

	}
	if (axis == 'Y')
	{
		const float current_angle = asinf(cam->frustum.front.y / cam->frustum.front.Length());
		if (abs(current_angle + movement * rot_speed) >= math::pi / 2) {
			return;
		}
		rotation_matrix = float3x3::identity;
		rotation_matrix.SetRotatePart(cam->frustum.WorldRight(), movement * rot_speed);
		cam->frustum.up = rotation_matrix * cam->frustum.up;
		cam->frustum.front = rotation_matrix * cam->frustum.front;
	}
	generateMatrices(cam);
}

void ModuleCamera::LookAt(const float3 focus, Camera* cam)
{
	float3 look_direction = (focus - cam->frustum.pos).Normalized();
	SetOrientation(look_direction,cam);

	generateMatrices(cam);
}
void ModuleCamera::LookAt(const float x, const float y, const float z,Camera* cam)
{
	LookAt(float3(x, y, z), cam);
}
void ModuleCamera::SetOrientation(const float3 orientation, Camera* cam)
{
	float3 direction = (orientation - cam->frustum.pos).Normalized();
	float3x3 rotation = float3x3::LookAt(cam->frustum.front, direction, cam->frustum.up, float3::unitY);
	cam->frustum.front = rotation.Transform(cam->frustum.front).Normalized();
	cam->frustum.up = rotation.Transform(cam->frustum.up).Normalized();
	generateMatrices(cam);
}


void ModuleCamera::multSpeed(Camera* cam)
{
	speed = 2.0f;
}

void ModuleCamera::generateMatrices(Camera* cam)
{
	cam->proj = cam->frustum.ProjectionMatrix();
	cam->view = cam->frustum.ViewMatrix();
}


void ModuleCamera::focusCameraToNewPoint(const float3 & newPos, Camera* cam)
{
	cam->frustum.pos = newPos;
	LookAt(App->modelLoader->modelCenter,cam);
	generateMatrices(cam);

}


void ModuleCamera::OrbitX(float angle, Camera* cam)
{
	float3x3 rotation_matrix;

	rotation_matrix = float3x3::RotateY(-angle * rot_speed);
	cam->frustum.pos = rotation_matrix * cam->frustum.pos;
	//TODO: FocusAT should focus at the transform of the active GameObject
	FocusAt((float3::zero - cam->frustum.pos).Normalized(), cam);
	cam->view = LookAt(cam->frustum.pos, cam->frustum.pos + cam->frustum.front, cam->frustum.up);

}
void ModuleCamera::OrbitY(float angle, Camera* cam){
	float3x3 rotation_matrix;
		const float current_angle = asinf(cam->frustum.front.y / cam->frustum.front.Length());
		if (abs(current_angle + angle * rot_speed) >= math::pi / 2) {
			return;
		}
		rotation_matrix = float3x3::identity;
		rotation_matrix.SetRotatePart(cam->frustum.WorldRight(), -angle * rot_speed);

		cam->frustum.pos = rotation_matrix * cam->frustum.pos;
		FocusAt((float3::zero - cam->frustum.pos).Normalized(), cam);
		//TODO: FocusAT should focus at the transform of the active GameObject
		view = LookAt(cam->frustum.pos, cam->frustum.pos + cam->frustum.front, cam->frustum.up);



}

float4x4 ModuleCamera::LookAt(float3 eye, float3 target, float3 up) {
	float4x4 matrix;
	math::float3 f(target - eye);
	f.Normalize();
	math::float3 s(f.Cross(up));
	s.Normalize();
	math::float3 u(s.Cross(f));
	matrix[0][0] = s.x;
	matrix[0][1] = s.y;
	matrix[0][2] = s.z;
	matrix[1][0] = u.x;
	matrix[1][1] = u.y;
	matrix[1][2] = u.z;
	matrix[2][0] = -f.x;
	matrix[2][1] = -f.y;
	matrix[2][2] = -f.z;
	matrix[0][3] = -s.Dot(eye);
	matrix[1][3] = -u.Dot(eye);
	matrix[2][3] = f.Dot(eye);
	matrix[3][0] = 0.0f;
	matrix[3][1] = 0.0f;
	matrix[3][2] = 0.0f;
	matrix[3][3] = 1.0f;
	return matrix;
}


void ModuleCamera::FocusAt(float3 target, Camera* cam)
{
	float3x3 rotation_matrix = float3x3::LookAt(cam->frustum.front, target, cam->frustum.up, float3::unitY);
	cam->frustum.front = rotation_matrix * cam->frustum.front;
	cam->frustum.up = rotation_matrix * cam->frustum.up;
}

void ModuleCamera::Orbit(char axis, float movement, Camera* cam) {
	

	if (axis == 'X') {
		yaw = rot_speed * speed;

		if (movement < 0) {
			model = model * math::float3x3::RotateY(-yaw);
			cam->frustum.pos = cam->frustum.pos * math::float3x3::RotateY(-yaw);
		}
		else {
			cam->model = cam->model * math::float3x3::RotateY(yaw);
			cam->frustum.pos = cam->frustum.pos * math::float3x3::RotateY(yaw);
		}
	}
	else if (axis == 'Y') {
		pitch = rot_speed * speed;

		if (movement < 0) {
			cam->model = cam->model * math::float3x3::RotateAxisAngle(cam->frustum.WorldRight(), pitch);
			cam->frustum.pos = cam->frustum.pos *math::float3x3::RotateAxisAngle(cam->frustum.WorldRight(), pitch);
		
		}
		else {
			cam->model = cam->model * math::float3x3::RotateAxisAngle(cam->frustum.WorldRight(), -pitch);
			cam->frustum.pos = cam->frustum.pos * math::float3x3::RotateAxisAngle(cam->frustum.WorldRight(), -pitch);
		}	
	}
	generateMatrices(cam);
}

void ModuleCamera::ShowCameraUI()
{
	//change FOV
	float f2 = frustum.verticalFov;
	ImGui::SliderFloat("Fov ", &f2, 0.01f, 3.12F, "%.2f", 2.0f);
	//App->camera->setFOV(f2,cam);

	//camera position
	ImGui::Text("Camera X position: %.2f", frustum.pos[0]);
	ImGui::Text("Camera Y position: %.2f", frustum.pos[1]);
	ImGui::Text("Camera Z position: %.2f", frustum.pos[2]);
}