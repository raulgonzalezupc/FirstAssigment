#include "ModuleCamera.h"
#include "Globals.h"

#include "MathGeoLib/include/Math/float4.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/float4x4.h"
#include "MathGeoLib/include/Math/float3x3.h"
#include "MathGeoLib/include/Geometry/Frustum.h"
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
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;

	frustum.nearPlaneDistance = 1.0F;
	frustum.farPlaneDistance = 100.0F;
	frustum.verticalFov = PI / 4.0F;
	frustum.horizontalFov = 2.0F*atanf(tanf(frustum.verticalFov*0.5F)*aspect);

	//setting up our proj
	proj = frustum.ProjectionMatrix();
	model = float4x4::FromTRS(float3(0.0F, 0.0F, -4.0F), float3x3::RotateX(0.0F), float3(1.0F, 1.0F, 1.0F));
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
	aspect = aspect_ratio;
	LOG("Aspect ratio: %f",aspect);
	frustum.horizontalFov = 2.0F*atanf(tanf(frustum.verticalFov*0.5F)*aspect);
	proj = frustum.ProjectionMatrix();
}

void ModuleCamera::setFOV(const float fov)
{
	frustum.verticalFov = fov;
	frustum.horizontalFov = 2.0F*atanf(tanf(frustum.verticalFov*0.5F)*aspect);
}

void ModuleCamera::Position(const float3 position)
{
	frustum.pos = position;
	proj = frustum.ProjectionMatrix();
}



void ModuleCamera::MoveUp()
{
	new_camera_pos = frustum.pos;
	new_camera_pos.y = new_camera_pos.y + distance;
	frustum.pos = new_camera_pos;
	proj = frustum.ProjectionMatrix();
	view = frustum.ViewMatrix();
}
void ModuleCamera::MoveDown()
{
	new_camera_pos = frustum.pos;
	new_camera_pos.y = new_camera_pos.y - distance;
	frustum.pos = new_camera_pos;
	proj = frustum.ProjectionMatrix();
	view = frustum.ViewMatrix();
}
void ModuleCamera::MoveForward()
{
	new_camera_pos = frustum.front;
	frustum.pos += frustum.front.ScaledToLength(distance);
	proj = frustum.ProjectionMatrix();
	view = frustum.ViewMatrix();
}
void ModuleCamera::MoveBackwards()
{
	new_camera_pos = frustum.front;
	frustum.pos -= frustum.front.ScaledToLength(distance);
	proj = frustum.ProjectionMatrix();
	view = frustum.ViewMatrix();
}

void ModuleCamera::MoveLeft()
{
	frustum.pos -= frustum.WorldRight().ScaledToLength(distance);
	proj = frustum.ProjectionMatrix();
	view = frustum.ViewMatrix();
}

void ModuleCamera::MoveRight()
{
	frustum.pos = frustum.pos + frustum.WorldRight().ScaledToLength(distance);
	proj = frustum.ProjectionMatrix();
	view = frustum.ViewMatrix();

}

void ModuleCamera::MouseXMotion(const float x_motion)
{
	bool correct = false;
	if (!correct)
	{
		const float adjusted_angle = x_motion * -0.01f;
		float3x3 rotation_matrix = float3x3::RotateY(adjusted_angle);
		frustum.up = rotation_matrix * frustum.up;
		frustum.front = rotation_matrix * frustum.front;
		frustum.pos = rotation_matrix * frustum.pos;
		proj = frustum.ProjectionMatrix();
		view = frustum.ViewMatrix();
	}
	else {
		const float adjusted_angle = x_motion * -0.01f;
		float3x3 rotation_matrix = float3x3::RotateY(adjusted_angle);
		frustum.up = rotation_matrix * frustum.up;
		frustum.front = rotation_matrix * frustum.front;
		proj = frustum.ProjectionMatrix();
		view = frustum.ViewMatrix();
	}
}
void ModuleCamera::MouseYMotion(const float y_motion)
{
	bool correct = false;
	if (correct)
	{
		const float adjusted_angle = y_motion * -0.1f;
		float3x3 rotation_matrix = float3x3::RotateX(adjusted_angle);
		frustum.up = rotation_matrix * frustum.up;
		frustum.front = rotation_matrix * frustum.front;
		frustum.pos = rotation_matrix * frustum.pos;
		proj = frustum.ProjectionMatrix();
		view = frustum.ViewMatrix();
	}
	else {
		const float adjusted_angle = y_motion * -0.1F;
		float3x3 rotation_matrix = float3x3::RotateX(adjusted_angle);
		frustum.up = rotation_matrix * frustum.up;
		frustum.front = rotation_matrix * frustum.front;
		proj = frustum.ProjectionMatrix();
		view = frustum.ViewMatrix();

	}
}

