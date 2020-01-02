#include "ModuleTexture.h"
#include "ModuleModelLoader.h"
#include "ModuleImgui.h"
#include "Application.h"
#include "DevIL/include/IL/il.h"
#include "DevIL/include/IL/ilu.h"
#include "DevIL/include/IL/ilut.h"

ModuleTexture::ModuleTexture()
{

}

ModuleTexture::~ModuleTexture()
{

}

bool ModuleTexture::Init()
{
	ilInit();
	iluInit();
	ilutInit();
	App->imgui->AddLog("Initialization of Il.\n");
	ilutRenderer(ILUT_OPENGL);
	ilGenImages(1, &imageName);
	ilBindImage(imageName);
	return true;
}

update_status ModuleTexture::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleTexture::Update()
{
	return UPDATE_CONTINUE;
}

update_status ModuleTexture::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleTexture::CleanUp()
{
	ilDeleteImages(1, &imageName);
	glDeleteTextures(1, &imageName);
	return true;
}

Texture ModuleTexture::LoadTexture(const char* path) 
{
	
	ilLoadImage(path);
	iluGetImageInfo(&imageInfo);
	if (imageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
	{
		iluFlipImage();
	}
	Texture.id = ilutGLBindTexImage();
	Texture.width = ilGetInteger(IL_IMAGE_WIDTH);
	Texture.height = ilGetInteger(IL_IMAGE_HEIGHT);
	Texture.data = ilGetData();
	Texture.path = path;
	
	App->modelLoader->textureId = Texture.id;
	App->modelLoader->textureWidth = Texture.width;
	App->modelLoader->textureHeight = Texture.height;

	return Texture;
}

void ModuleTexture::DrawTexture(unsigned& id) {
	if (drawSelector) {
		DrawTextureSelector(id);
	}
	for (unsigned int i = 0; i < loadedTextures.size(); i++) {
		if (loadedTextures[i].id == id) {
			//DrawTexture(loadedTextures[i]);
		}
	}
	if (ImGui::ImageButton((void*)(intptr_t)id, ImVec2(128, 128))) {
		drawSelector = true;
	}

}


void ModuleTexture::DrawTextureSelector(unsigned& id) {
	ImGui::Begin("Textures");
	for (unsigned int i = 0; i < loadedTextures.size(); i++) {
		//DrawTexture(loadedTextures[i]);
		if (ImGui::ImageButton((void*)(intptr_t)loadedTextures[i].id, ImVec2(128, 128))) {
			id = loadedTextures[i].id;
			drawSelector = false;
		}
	}
	ImGui::End();
}
Material* ModuleTexture::CreateMaterial() {
	return new Material(nullptr);
}