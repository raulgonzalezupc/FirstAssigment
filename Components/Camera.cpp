#include "Camera.h"
#include "../GameObject.h"
#include "Component.h"
#include "../Application.h"
#include "../ModuleRender.h"
#include "../ModuleWindow.h"
#include "../ModuleCamera.h"
#include "../ModuleScene.h"
#include "../ModuleProgram.h"
#include "../ModuleModelLoader.h"
#include <math.h>
#include "../imgui/imgui.h"


Camera::Camera(GameObject* owner, int number) : Component(owner, ComponentType::Camera)
{
	
	aspect = App->window->width / App->window->height;

	frustum.type = FrustumType::PerspectiveFrustum;
	if (number == 1) {
		frustum.pos = float3::unitX;
	}
	else {
		frustum.pos = float3{ 6.0F, 0.0F, -10.0F };
	}
	//skybox = new Skybox();

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

	glGenFramebuffers(1, &fbo);


}
Camera::~Camera()
{
	glDeleteFramebuffers(1, &fbo);
	glDeleteFramebuffers(1, &fb_depth);
}

int Camera::isCollidingFrustum(const AABB& aabb) const
{
	float3 edges[8];
	int totalPointsIn = 0;
	aabb.GetCornerPoints(edges);
	Plane viewportPlanes[6];
	frustum.GetPlanes(viewportPlanes);

	for (int pl = 0; pl < 6; pl++)
	{
		int isInPlane = 0;
		for (int p = 0; p < 8; p++)
		{
			if (viewportPlanes[pl].IsOnPositiveSide(edges[p]))
			{
				isInPlane = 1;
				++totalPointsIn;
			}
		}
	}
	if (totalPointsIn == 6)
	{
		return IS_IN;
	}

	if (totalPointsIn == 0)
	{
		return IS_OUT;
	}
	return INTERSECT;
}

void Camera::SetFrustum()
{
	camPos = frustum.pos;

	Hnear = 2 * tan(frustum.verticalFov / 2) * frustum.nearPlaneDistance;
	Wnear = Hnear * aspect;

	Hfar = 2 * tan(frustum.verticalFov / 2) * frustum.farPlaneDistance;
	Wfar = Hfar * aspect;


	nCenter = frustum.pos + frustum.front * frustum.nearPlaneDistance;
	fCenter = frustum.pos + frustum.front * frustum.farPlaneDistance;

	ftl = fCenter + (frustum.up * (Hfar / 2)) - (frustum.WorldRight()* (Wfar / 2));
	ftr = fCenter + (frustum.up * (Hfar / 2)) + (frustum.WorldRight()* (Wfar / 2));

	fbl = fCenter - (frustum.up * (Hfar / 2)) - (frustum.WorldRight()* (Wfar / 2));
	fbr = fCenter - (frustum.up * (Hfar / 2)) + (frustum.WorldRight()* (Wfar / 2));

	ntl = nCenter + (frustum.up * (Hnear / 2)) - (frustum.WorldRight()* (Wnear / 2));
	ntr = nCenter + (frustum.up * (Hnear / 2)) + (frustum.WorldRight()* (Wnear / 2));

	nbl = nCenter - (frustum.up * (Hnear / 2)) - (frustum.WorldRight()* (Wnear / 2));
	nbr = nCenter - (frustum.up * (Hnear / 2)) + (frustum.WorldRight()* (Wnear / 2));
}

void Camera::Draw(const char* name)
{
	
	ImGui::Begin(name);
	if (ImGui::IsWindowHovered())
	{
		isHovered = true;
	}
	else
	{
		isHovered = false;
	}
	width = ImGui::GetWindowContentRegionWidth();
	height = ImGui::GetContentRegionAvail().y;

	ImGui::SetNextWindowPos(ImVec2(256.0f, 0.0f), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(800.0f, 600.0f), ImGuiCond_FirstUseEver);
	BindBuffers(width, height);
	App->camera->SetAspectRatio(width/height, this);
	ImGui::GetWindowDrawList()->AddImage(
		(void*)fb_tex,
		ImVec2(ImGui::GetCursorScreenPos()),
		ImVec2(ImGui::GetCursorScreenPos().x + fb_width,
			ImGui::GetCursorScreenPos().y + fb_height),
		ImVec2(0, 1), ImVec2(1, 0));

	//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}

void Camera::BindBuffers(unsigned w, unsigned  h)
{
	if (fb_tex)
	{
		glDeleteTextures(1, &fb_tex);
	}
	else {
		glGenTextures(1, &fb_tex);
	}
	if (fb_depth)
	{
		glDeleteRenderbuffers(1, &fb_depth);
	}
	else {
		glGenRenderbuffers(1, &fb_depth);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glBindRenderbuffer(GL_RENDERBUFFER, fb_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);


	glBindTexture(GL_TEXTURE_2D, fb_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb_tex, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fb_depth);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	

	fb_width = w;
	fb_height = h;

	

	//glDrawBuffer(GL_COLOR_ATTACHMENT0);

}
void Camera::GenerateFBOTexture(unsigned w, unsigned h)
{
	BindBuffers(w, h);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	
	unsigned int program = App->program->defaultProgram;
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, &(model[0][0]));
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, &(view[0][0]));
	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, &(proj[0][0]));
	glViewport(0, 0, width, height);
	glClearColor(0.2f, 0.2f, 0.2f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
		App->renderer->ShowAxis();
		

	
		App->renderer->ShowGrid();
	
	App->modelLoader->Draw(program);


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
