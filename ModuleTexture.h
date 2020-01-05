#ifndef _MODULETEXTURE_H_
#define _MODULETEXTURE_H_

#include "Module.h"
#include "Mesh.h"
#include "Globals.h"
#include "glew/include/GL/glew.h"
#include "DevIL/include/IL/il.h"
#include "DevIL/include/IL/ilu.h"
#include "Components/Material.h"


class ModuleTexture : public Module {

public:
	ModuleTexture();
	virtual ~ModuleTexture();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	Texture LoadTexture(const char* path);
	Material* CreateMaterial();
	void DrawTexture(unsigned&);
	void DrawTextureSelector(unsigned&);
	void LoadSkybox(const char* path, const std::string &directory, int index) const;
	bool isLoaded;
public:
	std::vector<Texture> loadedTextures;
	int newTexture;
	Texture Texture;
	ILuint imageName;
	GLuint texture;
	ILinfo imageInfo;
	bool drawSelector = false;
};

#endif