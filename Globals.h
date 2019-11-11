#pragma once
#include <windows.h>
#include <stdio.h>

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// Configuration -----------
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define FULLSCREEN false
#define VERTEX_SHADER_PATH "./default.vs"
#define FRAGMENT_SHADER_PATH "./default.fs"
#define VSYNC true
#define TITLE "Super Awesome Engine"
#define PI 3.1415
#define IMAGE_NAME "lena.png"