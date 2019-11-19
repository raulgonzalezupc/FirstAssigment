#include "ModuleTexture.h"
#include "Globals.h"
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
	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION) {
		printf("DevIL version is different...exiting!\n");
		return 1;
	}
	ilInit();
	iluInit();
	ilutInit();
	ImageName = ilGenImage();
	ilBindImage(ImageName);
	bool imageLoaded = ilLoadImage(IMAGE_NAME);
	ilEnable(IL_FILE_OVERWRITE);
	if (imageLoaded)
	{
		LOG("Image loaded");

	}
	else {
		LOG("Image not loaded");
		return false;
	}
	ILubyte *Data = ilGetData();
	ILuint Width, Height;
	Width = ilGetInteger(IL_IMAGE_WIDTH);
	Height = ilGetInteger(IL_IMAGE_HEIGHT);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Data);
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
	//ilDeleteImages(1, &ImageName);
	//glDeleteTextures(1, &ImageName);
	return true;
}

Texture ModuleTexture::LoadTexture(const char* path) {
	Texture texture;
	ilLoadImage(path);
	iluGetImageInfo(&imageInfo);
	if (imageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
	{
		iluFlipImage();
	}
	texture.id = ilutGLBindTexImage();
	texture.width = ilGetInteger(IL_IMAGE_WIDTH);
	texture.height = ilGetInteger(IL_IMAGE_HEIGHT);
	texture.data = ilGetData();
	texture.path = path;
	return texture;
}