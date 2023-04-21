#include "Utils.h"

char* Utils::readShaderSource(const char* filePath) {
	FILE* shaderFile = fopen(filePath, "rb");
	uint64 fileSize = 0;
	char* shaderCode = NULL;

	if (!shaderFile) {
		printf("Could not read file! %s. File doesn't exist!", filePath);
		return shaderCode;
	}

	//Getting File Size
	fseek(shaderFile, 0, SEEK_END);
	fileSize = ftell(shaderFile);
	rewind(shaderFile);

	//Reading From File
	shaderCode = (char*)malloc(sizeof(char) * (fileSize + 1));
	if (shaderCode) {
		fread(shaderCode, sizeof(char), fileSize, shaderFile);
		fclose(shaderFile);
		shaderCode[fileSize] = '\0';
	}
	return shaderCode;
}

void Utils::printShaderLog(uint32 shader) {
	int len = 0;
	int chWrittn = 0;
	char* log;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
	if (len > 0) {
		log = (char*)malloc(len);
		glGetShaderInfoLog(shader, len, &chWrittn, log);
		std::cout << "Shader Info Log: " << log << std::endl;
		free(log);
	}
}

void Utils::printProgramLog(int prog) {
	int len = 0;
	int chWrittn = 0;
	char* log;
	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
	if (len > 0) {
		log = (char*)malloc(len);
		glGetProgramInfoLog(prog, len, &chWrittn, log);
		std::cout << "Program Info Log: " << log << std::endl;
		free(log);
	}
}

uint32 Utils::createShaderProgram() {
	bool32 vertCompiled;
	bool32 fragCompiled;
	bool32 linked;

	const char* vshaderSource = readShaderSource("assets\\glsl\\Vertex.glsl");
	const char* fshaderSource = readShaderSource("assets\\glsl\\Fragment.glsl");

	uint32 vShader = glCreateShader(GL_VERTEX_SHADER);
	uint32 fShader = glCreateShader(GL_FRAGMENT_SHADER);
	uint32 vfProgram = glCreateProgram();

	glShaderSource(vShader, 1, &vshaderSource, NULL);
	glShaderSource(fShader, 1, &fshaderSource, NULL);

	// catch errors while compiling shaders
	glCompileShader(vShader);
	checkOpenGLError();
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
	if (vertCompiled != 1) {
		std::cout << "vertex compilation failed" << std::endl;
		printShaderLog(vShader);
	}

	glCompileShader(fShader);
	checkOpenGLError();
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
	if (fragCompiled != 1) {
		std::cout << "fragment compilation failed" << std::endl;
		printShaderLog(fShader);
	}

	// catch errors while linking shaders
	glAttachShader(vfProgram, vShader);
	glAttachShader(vfProgram, fShader);
	glLinkProgram(vfProgram);
	checkOpenGLError();
	glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
	if (linked != 1) {
		std::cout << "linking failed" << std::endl;
		printProgramLog(vfProgram);
	}
	return vfProgram;
}

bool Utils::checkOpenGLError() {
	bool foundError = false;
	int glErr = glGetError();
	while (glErr != GL_NO_ERROR) {
		std::cout << "glError: " << glErr << std::endl;
		foundError = true;
		glErr = glGetError();
	}
	return foundError;
}

uint32 Utils::loadTexture(const char* texImagePath) {
	uint32 texID;
	texID = SOIL_load_OGL_texture(
		texImagePath,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
	);

	if (texID == 0)
		std::cout << "Could not load texture file. " << texImagePath << std::endl;
	return texID;
}


#ifdef  WINDOWS
#include <Windows.h>
void PWD() {
	TCHAR Buffer[MAX_PATH];
	if (GetCurrentDirectoryW(MAX_PATH, Buffer)) {
		// Note: Success!
	}
	else {
		printf("Error Occured while printing current working directory!\n");
	}
}
#endif