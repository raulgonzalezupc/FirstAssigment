#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleProgram.h"
#include "glew/include/GL/glew.h"
#include "MathGeoLib/include/Math/float4.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/float4x4.h"
#include "MathGeoLib/include/Math/float3x3.h"
#include "MathGeoLib/include/Geometry/Frustum.h"
#include "SDL.h"

ModuleRender::ModuleRender()
{
}

// Destructor
ModuleRender::~ModuleRender()
{
}

// Called before render is available
bool ModuleRender::Init()
{
	LOG("Creating Renderer context");
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glClearDepth(1.0f);
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);


	glcontext = SDL_GL_CreateContext(App->window->window);

	glewInit();
	LOG("Using Glew %s", glewGetString(GLEW_VERSION));

	//our triangle
	float buffer_data[] = {
	 0.5f,  0.5f, 0.0f,  // top right
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f,   // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	//setting up our aspect
	float aspect = SCREEN_WIDTH / SCREEN_HEIGHT;

	Frustum frustum;
	frustum.type = FrustumType::PerspectiveFrustum;

	frustum.pos = float3::zero;
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;

	frustum.nearPlaneDistance = 0.1F;
	frustum.farPlaneDistance = 100.0F;
	frustum.verticalFov = PI / 4.0F;
	frustum.horizontalFov = 2.0F*atanf(tanf(frustum.verticalFov*0.5F)*aspect);

	//setting up our proj
	proj = frustum.ProjectionMatrix();
	model = float4x4::FromTRS(float3(0.0F, 0.0F, -4.0F), float3x3::RotateY(PI / 4.0F), float3(1.0F, 1.0F, 1.0F));
	view = float4x4::LookAt(float3(0.0F, 0.0F, -1.0F), float3(0.0F, 0.0F, -1.0F), float3(0.0F, 1.0F, 0.0F), float3(0.0F, 1.0F, 0.0F));
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

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(buffer_data), buffer_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	return true;
}

update_status ModuleRender::PreUpdate()
{
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(App->program->shader_program);
	glUniformMatrix4fv(glGetUniformLocation(App->program->shader_program,
		"model"), 1, GL_TRUE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->program->shader_program,
		"view"), 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->program->shader_program,
		"proj"), 1, GL_TRUE, &proj[0][0]);
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update()
{

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //change if triangle
	glEnableVertexAttribArray(0); // attribute 0      
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * 4));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	LOG("Destroying renderer");
	glDeleteBuffers(1, &vbo);
	SDL_GL_DeleteContext(glcontext);
	//Destroy window

	return true;
}

void ModuleRender::WindowResized(unsigned width, unsigned height)
{
}

