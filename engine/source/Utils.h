#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL2/SOIL2.h>
#include <iostream>

#include "defines.h"

namespace Utils
{
	uint32 CreateShaderProgram(const char* vp, const char* fp);
	uint32 CreateShaderProgram(const char* vp, const char* gp, const char* fp);
	uint32 CreateShaderProgram(const char* vp, const char* tCS, const char* tES, const char* fp);
	uint32 CreateShaderProgram(const char* vp, const char* tCS, const char* tES, const char* gp, const char* fp);

	char* readShaderSource(const char* filePath);
	void printShaderLog(uint32 shader);
	void printProgramLog(int prog);
	bool checkOpenGLError();
	uint32 createShaderProgram();
	uint32 loadTexture(const char* texImagePath);
}