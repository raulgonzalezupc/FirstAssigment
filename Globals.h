#pragma once
#include <windows.h>
#include <stdio.h>
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/float2.h"

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

enum class ComponentType 
{ 
	Transform, 
	Mesh, 
	Material, 
	Camera 
};

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

struct Texture {
	unsigned int id;
	char*  type;
	const char* path;
	int width;
	int height;
	unsigned char *data;
};
struct Vertex {
	float3 Position;
	float3 Normal;
	float2 TexCoords;
	float3 Tangent;
	float3 Bitangent;
};


// Configuration -----------
#define SCREEN_WIDTH 1240
#define SCREEN_HEIGHT 720
#define FULLSCREEN false
#define VERTEX_SHADER_PATH "./default.vs"
#define FRAGMENT_SHADER_PATH "./default.fs"
#define VSYNC true
#define TITLE "Morgoth Engine"
#define PI 3.1415
#define IMAGE_NAME "lena.png"
#define RESIZE true
#define BAKER_HOUSE_FBX "./BakerHouse.fbx"
#define BAKER_HOUSE_PNG "./Baker_house.png"