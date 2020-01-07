#include "Camera.h"
#include "../GameObject.h"
#include "Component.h"
#include "../Application.h"
#include "../ModuleRender.h"
#include "../ModuleWindow.h"
#include "../ModuleProgram.h"
#include "../ModuleModelLoader.h"
#include <math.h>
#include "../imgui/imgui.h"


Camera::Camera(GameObject* owner, int number) : Component(owner, ComponentType::Camera)
{
	aspect = App->window->width / App->window->height;
	frustum = new Frustum();
		frustum->type = FrustumType::PerspectiveFrustum;
		

		frustum->pos = float3::unitX;
		skybox = new Skybox();

		frustum->front = float3::unitZ;
		frustum->up = float3::unitY;

		frustum->nearPlaneDistance = 1.0F;
		frustum->farPlaneDistance = 100.0F;
		frustum->verticalFov = PI / 4.0F;
		frustum->horizontalFov = 2.0F*atanf(tanf(frustum->verticalFov*0.5F)*aspect);

		//setting up our proj
		proj = frustum->ProjectionMatrix();
		model = float4x4::FromTRS(float3(0.0F, -5.0F, 20.0F), float3x3::RotateY(0.0F), float3(1.0F, 1.0F, 1.0F));
		view = frustum->ViewMatrix();
		float4x4 transform = proj * view * float4x4(model);
	glCreateFramebuffers(1, &fbo);
	glCreateFramebuffers(1, &fboGame);
}
Camera::~Camera()
{
	//glDeleteFramebuffers(1, &fbo);
	//glDeleteRenderbuffers(1, &rbo);
	glDeleteFramebuffers(1, &fbo);
	glDeleteTextures(1, &texture);
	glDeleteRenderbuffers(1, &rbo);
}

int Camera::isCollidingFrustum(const AABB& aabb) const
{
	float3 edges[8];
	int totalPointsIn = 0;
	aabb.GetCornerPoints(edges);
	Plane viewportPlanes[6];
	frustum->GetPlanes(viewportPlanes);

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
	
}

void Camera::Draw(const char* name, int type)
{
	

	ImGui::Begin(name);
	if (ImGui::IsItemHovered)
	{
		hovered = true;
	}
	else
	{
		hovered = false;
	}
	if (type == 1) {
		ImGui::SetNextWindowPos(ImVec2(256.0f, 0.0f), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(800.0f, 600.0f), ImGuiCond_FirstUseEver);
		width = ImGui::GetWindowContentRegionWidth();
		height = ImGui::GetContentRegionAvail().y;
		DrawScene(width, height,1);
		ImGui::GetWindowDrawList()->AddImage(
			(void*)texture,
			ImVec2(ImGui::GetCursorScreenPos()),
			ImVec2(ImGui::GetCursorScreenPos().x + width,
				ImGui::GetCursorScreenPos().y + height),
			ImVec2(0, 1), ImVec2(1, 0));


		ImGui::End();
	}
	else {
		DrawScene(width, height, 2);
		ImGui::SetNextWindowPos(ImVec2(256.0f, 0.0f), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(800.0f, 600.0f), ImGuiCond_FirstUseEver);
		width = ImGui::GetWindowContentRegionWidth();
		height = ImGui::GetContentRegionAvail().y;
		
		ImGui::GetWindowDrawList()->AddImage(
			(void*)textureGame,
			ImVec2(ImGui::GetCursorScreenPos()),
			ImVec2(ImGui::GetCursorScreenPos().x + width,
				ImGui::GetCursorScreenPos().y + height),
			ImVec2(0, 1), ImVec2(1, 0));


		ImGui::End();
	}
	
}

void Camera::GenerateFBOTexture(unsigned w, unsigned h)
{
	if (w != fb_width || h != fb_height)
	{
		if (fb_tex != 0)
		{
			glDeleteTextures(1, &fb_tex);
		}

		if (w != 0 && h != 0)
		{
			if (fbo == 0)
			{
				glGenFramebuffers(1, &fbo);
			}

			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			glGenTextures(1, &fb_tex);
			glBindTexture(GL_TEXTURE_2D, fb_tex);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glGenRenderbuffers(1, &fb_depth);
			glBindRenderbuffer(GL_RENDERBUFFER, fb_depth);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fb_depth);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb_tex, 0);

			glDrawBuffer(GL_COLOR_ATTACHMENT0);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		fb_width = w;
		fb_height = h;
	}
}


void Camera::GenerateBuffers(int width, int height, int type)
{
	if (type==1) {
		if (texture) {
			glDeleteTextures(1, &texture);
		}
		else {
			glGenTextures(1, &texture);
		}

		if (rbo) {
			glDeleteRenderbuffers(1, &rbo);
		}
		else {
			glGenRenderbuffers(1, &rbo);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		//textures
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

		//bind to 0
		glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
		
	}
	else {
		if (textureGame !=0) {
			glDeleteTextures(1, &textureGame);
		}
		else {
			glGenTextures(1, &textureGame);
		}

		if (rboGame) {
			glDeleteRenderbuffers(1, &rboGame);
		}
		else {
			glGenRenderbuffers(1, &rboGame);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, fboGame);
		glBindRenderbuffer(GL_RENDERBUFFER, rboGame);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		//textures
		glBindTexture(GL_TEXTURE_2D, textureGame);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureGame, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboGame);

		//bind to 0
		glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
	}
	
}

void Camera::DrawScene(int width, int height,int type)
{
	GenerateBuffers(width, height, type);
	if (type==1)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		program = App->program->defaultProgram;

		glUseProgram(program);
		glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, &model[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, &proj[0][0]);

		glViewport(0, 0, width, height);

		// second pass

		glClearColor(0.51f, 0.51f, 0.51f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		App->renderer->ShowGrid();

		App->renderer->ShowAxis();

		App->modelLoader->Draw(program);
		skybox->DrawSkybox();

		glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default

		SDL_GL_MakeCurrent(App->window->window, App->renderer->glcontext);
		
	}
	else {

		glBindFramebuffer(GL_FRAMEBUFFER, fboGame);

		
		sceneProgram = App->program->sceneProgram;
		glUseProgram(sceneProgram);
		glUniformMatrix4fv(glGetUniformLocation(sceneProgram, "model"), 1, GL_TRUE, &(model[0][0]));
		glUniformMatrix4fv(glGetUniformLocation(sceneProgram, "view"), 1, GL_TRUE, &(view[0][0]));
		glUniformMatrix4fv(glGetUniformLocation(sceneProgram, "proj"), 1, GL_TRUE, &(proj[0][0]));
		glViewport(0, 0, width, height);

		// second pass

		glClearColor(0.51f, 0.51f, 0.51f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		App->renderer->ShowGrid();

		App->renderer->ShowAxis();

		App->modelLoader->Draw(sceneProgram);
		skybox->DrawSkybox();

		glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default

		SDL_GL_MakeCurrent(App->window->window, App->renderer->glcontext);
	}
	

}
