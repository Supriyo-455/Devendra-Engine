#pragma once

#include <windows.h>                              // Header File For Windows
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

#include "Devendra_Win32_GL_EXT.h"
#include "defines.h"
#include "Devendra_Utils.h"

// TODO: Probably use different translational unit for this 

typedef struct Devendra_Window
{
    HGLRC           hRC;                                   // Permanent Rendering Context
    HDC             hDC;                                   // Private GDI Device Context
    HWND            hWnd;                                  // Holds Our Window Handle
    HINSTANCE       hInstance;                                  // Holds The Instance Of The Application
    bool32          active;                                // Window Active Flag Set To TRUE By Default
    bool32          fullscreen;                            // Fullscreen Flag Set To Fullscreen Mode By Default
    bool32          wireframe;                          // For wireframing
    char*           title; 
    int16           width; 
    int16           height; 
    int16           bits;
    WNDPROC         WndProc;
}Devendra_Window;

void ReSizeGLScene(int width, int height);
bool32 InitGL(void);
int32 DrawGLScene(uint32 VAO, uint32 indiciesCount);
GLvoid KillGLWindow(Devendra_Window* DWindow);
bool32 CreateGLWindow(Devendra_Window* DWindow);

typedef struct Devendra_Shader
{
    const char* vertexShaderPath;
    const char* fragmentShaderPath;
    uint32 ShaderProgramID;
    uint32 vertexShaderID;
    uint32 fragmentShaderID;
    bool32 active;
}Devendra_Shader;

void CompileFragmentShader(Devendra_Shader* shader, const char* fragmentShaderPath);
void CompileVertexShader(Devendra_Shader* shader, const char* vertexShaderPath);
void CompileShaderProgram(Devendra_Shader* shader);
void useShader(Devendra_Shader* shader);

void setUniform1i(Devendra_Shader* shader, const char* name, int32 value);
// void setUniform2i(Devendra_Shader* shader);
// void setUniform3i(Devendra_Shader* shader);
// void setUniform4i(Devendra_Shader* shader);

void setUniform1f(Devendra_Shader* shader, const char* name, real32 value);
// void setUniform2f(Devendra_Shader* shader);
// void setUniform3f(Devendra_Shader* shader);
// void setUniform4f(Devendra_Shader* shader);

// Devendra_Shader* createShaderFromHeap(Devendra_Shader* shader);

#include "Devendra_Win32_GL.cpp"