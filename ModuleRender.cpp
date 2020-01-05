#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleProgram.h"
#include "ModuleImgui.h"
#include "ModuleModelLoader.h"
#include "ModuleCamera.h"
#include "Skybox.h"
#include <set>

#include "glew/include/GL/glew.h"
#include "SDL.h"
#include "stbi/stb_image.h"


static void APIENTRY openglCallbackFunction(
	GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam
) {
	(void)source; (void)type; (void)id;
	(void)severity; (void)length; (void)userParam;

	char error_source[256];
	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             sprintf_s(error_source, "Source: API"); break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   sprintf_s(error_source, "Source: Window System"); break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: sprintf_s(error_source, "Source: Shader Compiler"); break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     sprintf_s(error_source, "Source: Third Party"); break;
	case GL_DEBUG_SOURCE_APPLICATION:     sprintf_s(error_source, "Source: Application"); break;
	case GL_DEBUG_SOURCE_OTHER:           sprintf_s(error_source, "Source: Other"); break;
	}

	char error_type[256];
	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               sprintf_s(error_type, "Type: Error"); break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: sprintf_s(error_type, "Type: Deprecated Behaviour"); break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  sprintf_s(error_type, "Type: Undefined Behaviour"); break;
	case GL_DEBUG_TYPE_PORTABILITY:         sprintf_s(error_type, "Type: Portability"); break;
	case GL_DEBUG_TYPE_PERFORMANCE:         sprintf_s(error_type, "Type: Performance"); break;
	case GL_DEBUG_TYPE_MARKER:              sprintf_s(error_type, "Type: Marker"); break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          sprintf_s(error_type, "Type: Push Group"); break;
	case GL_DEBUG_TYPE_POP_GROUP:           sprintf_s(error_type, "Type: Pop Group"); break;
	case GL_DEBUG_TYPE_OTHER:               sprintf_s(error_type, "Type: Other"); break;
	}

	char error_message[4096];
	sprintf_s(error_message, "%s %s %s", error_source, error_type, message);
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:
		LOG(error_message);
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		LOG(error_message); // Actually not an itialization entry, I use this type of entry because the yellow color
		break;
	case GL_DEBUG_SEVERITY_LOW:
		// OPENGL_LOG_INFO(error_message); Too many messages in update
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		return;
	}
}

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
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

	

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glClearDepth(1.0f);
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glFrontFace(GL_CCW);
	
	App->imgui->AddLog("Creating Renderer context\n");
	glcontext = SDL_GL_CreateContext(App->window->window);

	GLenum err = glewInit();

	//DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(openglCallbackFunction, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	//glEnable(GL_BLEND);

	test = new GameObject("test");
	test2 = new GameObject("test2");
	cam = new Camera(test,1);
	cam2 = new Camera(test2,2);
	cam2->frustum->pos = float3{ 0.0F,0.0F, 120.0F };
	test->components.push_back(cam);
	test2->components.push_back(cam2);

	skybox = new Skybox();

	
	CatchFrameBufferErrors();
	


	App->imgui->AddLog("Using Glew %s\n", glewGetString(GLEW_VERSION));

	App->modelLoader->LoadModel("BakerHouse.fbx");
	
	


	return true;
}

update_status ModuleRender::PreUpdate()
{
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update()
{
	
	DrawGame();
	
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
	/*glDeleteFramebuffers(1, &fbo);
	glDeleteTextures(1, &texture);
	glDeleteRenderbuffers(1, &rbo);*/
	App->imgui->AddLog("Destroying renderer\n");
	SDL_GL_DeleteContext(glcontext);
	//Destroy window

	return true;
}


void ModuleRender::GenerateBuffers(int width, int height)
{
	
	if (texture) {
		glDeleteTextures(1, &texture);
	}
	else {
		glGenTextures(1, &texture);
	}

	if (rbo) {
		glDeleteRenderbuffers(1, &rbo);
	}else{
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

void ModuleRender::DrawScene(int width, int height)
{
	/*GenerateBuffers(width, height);*/
	
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);


	glUseProgram(App->program->defaultProgram);
	glUniformMatrix4fv(glGetUniformLocation(App->program->defaultProgram, "model"), 1, GL_TRUE, &(cam->model[0][0]));
	glUniformMatrix4fv(glGetUniformLocation(App->program->defaultProgram, "view"), 1, GL_TRUE, &(cam->view[0][0]));
	glUniformMatrix4fv(glGetUniformLocation(App->program->defaultProgram, "proj"), 1, GL_TRUE, &(cam->proj[0][0]));
	//glDrawBuffer(GL_COLOR_ATTACHMENT0);
	
	glViewport(0, 0, width, height);

	// second pass

	glClearColor(0.51f, 0.51f, 0.51f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	if (showGrid)
		{
			ShowGrid();
		}
	if (showAxis) 
		{
			ShowAxis();
		}
	App->modelLoader->Draw(App->program->shader_program);

	
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default

	SDL_GL_MakeCurrent(App->window->window, glcontext);
}

void ModuleRender::ShowGrid()
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
}

void ModuleRender::ShowAxis()
{
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
}

void ModuleRender::ShowRenderUI()
{
	ImGui::Checkbox("Show Grid", &showGrid);
	ImGui::Checkbox("Show Axis", &showAxis);
}

void ModuleRender::CatchFrameBufferErrors()
{
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
	{
		App->imgui->AddLog("--FrameBuffer loaded correctly\n");
	}
	else if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_UNDEFINED)
	{
		App->imgui->AddLog("--The default framebuffer does not exist. \n");
	}
	else if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)
	{
		App->imgui->AddLog("--The framebuffer attachment points are framebuffer incomplete.  \n");
	}
	else if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT)
	{
		App->imgui->AddLog("--The framebuffer does not have at least one image attached to it.  \n");
	}
	else if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT)
	{
		App->imgui->AddLog("--The framebuffer does not have at least one image attached to it.  \n");
	}
	else if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER)
	{
		App->imgui->AddLog("--The value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for any color attachment point(s) named by GL_DRAW_BUFFERi. \n");
	}
	else if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_UNSUPPORTED)
	{
		App->imgui->AddLog("--The combination of internal formats of the attached images violates an implementation-dependent set of restrictions. \n");
	}
	
}

int ModuleRender::loadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}






void ModuleRender::DrawGame()
{
	unsigned int progModel = App->program->defaultProgram;
	glUseProgram(progModel);

	glUniformMatrix4fv(glGetUniformLocation(progModel,
		"proj"), 1, GL_TRUE, &cam->proj[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(progModel,
		"view"), 1, GL_TRUE, &cam->view[0][0]);


	std::set<GameObject*> staticGO;
	std::set<GameObject*> dynamicGO;


	
	std::set<GameObject*> onCameraGO = staticGO;
	onCameraGO.insert(dynamicGO.begin(), dynamicGO.end());



	glUseProgram(0);
}





void ModuleRender::CreateFrameBuffer(int width, int height, bool scene)
{
	if (scene)
	{

		if (width != widthScene || height != heightScene || firstTimeCreatingBuffer)
		{
			if (firstTimeCreatingBuffer)
				firstTimeCreatingBuffer = false;

			if (frameBufferObject == 0)
			{
				//Generate FrameBuffer if necessary
				glCreateFramebuffers(1, &frameBufferObject);
			}

			glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);

			if (sceneTexture != 0)
			{
				glDeleteTextures(1, &sceneTexture);
			}

			if (renderBufferObject != 0)
			{
				glDeleteRenderbuffers(1, &renderBufferObject);
			}

			glGenTextures(1, &sceneTexture);
			glBindTexture(GL_TEXTURE_2D, sceneTexture);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, sceneTexture, 0);

			//Generate RenderBuffers
			glGenRenderbuffers(1, &renderBufferObject);
			glBindRenderbuffer(GL_RENDERBUFFER, renderBufferObject);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);

			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferObject);


			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				LOG("ERROR: Cannot create or render Scene framebuffer.");

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}
	else
	{

		if (width != widthGame || height != heightGame || firstTimeCreatingBuffer)
		{
			if (frameBufferObjectGame == 0)
			{
				//Generate FrameBuffer if necessary
				glCreateFramebuffers(1, &frameBufferObjectGame);
			}

			glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObjectGame);

			if (gameTexture != 0)
			{
				glDeleteTextures(1, &gameTexture);
			}

			if (renderBufferObjectGame != 0)
			{
				glDeleteRenderbuffers(1, &renderBufferObjectGame);
			}

			glGenTextures(1, &gameTexture);
			glBindTexture(GL_TEXTURE_2D, gameTexture);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gameTexture, 0);

			//Generate RenderBuffers
			glGenRenderbuffers(1, &renderBufferObjectGame);
			glBindRenderbuffer(GL_RENDERBUFFER, renderBufferObjectGame);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);

			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferObjectGame);


			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				LOG("ERROR: Cannot create or render Scene framebuffer.");

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}


}

void ModuleRender::GenerateTexture(int width, int height)
{
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);
	glViewport(0, 0, width, height);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Draw all scene
	
	if (skybox != nullptr)
		skybox->DrawSkybox();


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//Why outside of framebuffer?
	//App->debugDraw->Draw(App->camera, frameBufferObject, height, width);
}

void ModuleRender::GenerateTextureGame(int width, int height)
{
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObjectGame);
	glViewport(0, 0, width, height);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DrawGame();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



void ModuleRender::DrawSceneBuffer()
{
	bool isEnabled = true;
	//First Scene window is created
	
	ImGui::Begin("Scene", &isEnabled, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	bool is_focused = ImGui::IsWindowFocused();

	ImVec2 wSize = ImGui::GetWindowSize();
	App->camera->SetAspectRatio(800/900);

	

	CreateFrameBuffer((int)wSize.x, (int)wSize.y);
	GenerateTexture((int)wSize.x, (int)wSize.y);

	widthScene = (int)wSize.x;
	heightScene = (int)wSize.y;

	ImGui::GetWindowDrawList()->AddImage(
		(void *)sceneTexture,
		ImVec2(ImGui::GetCursorScreenPos()),
		ImVec2(
			ImGui::GetCursorScreenPos().x + wSize.x,
			ImGui::GetCursorScreenPos().y + wSize.y
		),
		ImVec2(0, 1),
		ImVec2(1, 0)
	);

	


	ImGui::End();

	return;
}

void ModuleRender::DrawGameBuffer()
{
	bool gameIsEnabled = true;

	
	//Game Window
	ImGui::Begin(" Game", &gameIsEnabled, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	ImVec2 wSizeGame = ImGui::GetWindowSize();

	


	CreateFrameBuffer((int)wSizeGame.x, (int)wSizeGame.y, false);
	GenerateTextureGame((int)wSizeGame.x, (int)wSizeGame.y);

	widthGame = (int)wSizeGame.x;
	heightGame = (int)wSizeGame.y;

	ImGui::GetWindowDrawList()->AddImage(
		(void *)gameTexture,
		ImVec2(ImGui::GetCursorScreenPos()),
		ImVec2(
			ImGui::GetCursorScreenPos().x + wSizeGame.x,
			ImGui::GetCursorScreenPos().y + wSizeGame.y
		),
		ImVec2(0, 1),
		ImVec2(1, 0)
	);

	ImGui::End();

	return;
}




