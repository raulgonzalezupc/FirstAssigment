#include "ModuleRenderExercice.h"
#include "Application.h"
#include "Globals.h"
#include "ModuleProgram.h"
#include "ModuleWindow.h"
#include "ModuleTexture.h"
#include "ModuleCamera.h"
#include "glew/include/GL/glew.h"
#include "DevIL/include/IL/il.h"
#include "SDL.h"

ModuleRenderExercice::ModuleRenderExercice()
{

}

ModuleRenderExercice::~ModuleRenderExercice()
{

}

bool ModuleRenderExercice::Init()
{
	
	
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(App->camera->buffer_data), App->camera->buffer_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(App->camera->indices), App->camera->indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); 
	return true;
}

update_status ModuleRenderExercice::PreUpdate()
{
	glUseProgram(App->program->shader_program);
	glUniformMatrix4fv(glGetUniformLocation(App->program->shader_program,
		"model"), 1, GL_TRUE, &App->camera->model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->program->shader_program,
		"view"), 1, GL_TRUE, &App->camera->view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->program->shader_program,
		"proj"), 1, GL_TRUE, &App->camera->proj[0][0]);
	return UPDATE_CONTINUE;
}

update_status ModuleRenderExercice::Update()
{
	glLineWidth(1.0f);
	float d = 200.0f;
	glBegin(GL_LINES);
	for (float i = -d; i <= d; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}
	glEnd();
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	// red X
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.1f, 0.0f); glVertex3f(1.1f, -0.1f, 0.0f);
	glVertex3f(1.1f, 0.1f, 0.0f); glVertex3f(1.0f, -0.1f, 0.0f);
	// green Y
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
	glVertex3f(0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
	glVertex3f(0.0f, 1.15f, 0.0f); glVertex3f(0.0f, 1.05f, 0.0f);
	// blue Z
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-0.05f, 0.1f, 1.05f); glVertex3f(0.05f, 0.1f, 1.05f);
	glVertex3f(0.05f, 0.1f, 1.05f); glVertex3f(-0.05f, -0.1f, 1.05f);
	glVertex3f(-0.05f, -0.1f, 1.05f); glVertex3f(0.05f, -0.1f, 1.05f);
	glEnd();
	glLineWidth(1.0f);

	glEnableVertexAttribArray(0); // attribute 0  
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //change if triangle
	glEnableVertexAttribArray(1); // attribute 0  
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, App->texture->texture);
	glUniform1i(glGetUniformLocation(App->program->shader_program, "texture0"), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * 4));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	return UPDATE_CONTINUE;
}

update_status ModuleRenderExercice::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleRenderExercice::CleanUp()
{
	glDeleteBuffers(1, &vbo);
	return true;
}