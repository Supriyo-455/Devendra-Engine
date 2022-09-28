#ifndef __DEVENDRA_WIN32_GL__
#define __DEVENDRA_WIN32_GL__

#include <windows.h>                              // Header File For Windows
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

#include "Devendra_Win32_GL_EXT.h"
#include "defines.h"
#include "Devendra_Utils.h"

#ifdef __DEVENDRA_WIN32_GL_EXPORTS__
#define DEVENDRA_WIN32_GL_API   __declspec(dllexport)
#else
#define DEVENDRA_WIN32_GL_API    __declspec(dllimport)
#endif

typedef struct Devendra_Window
{
    HGLRC           hRC=NULL;                                   // Permanent Rendering Context
    HDC             hDC=NULL;                                   // Private GDI Device Context
    HWND            hWnd=NULL;                                  // Holds Our Window Handle
    HINSTANCE       hInstance;                                  // Holds The Instance Of The Application
    bool32          active=TRUE;                                // Window Active Flag Set To TRUE By Default
    bool32          fullscreen=TRUE;                            // Fullscreen Flag Set To Fullscreen Mode By Default
    bool32          wireframe = false;                          // For wireframing
    char*           title; 
    int16           width; 
    int16           height; 
    int16           bits;
}Devendra_Window;

DEVENDRA_WIN32_GL_API void ReSizeGLScene(int width, int height);
DEVENDRA_WIN32_GL_API bool32 InitGL(void);
DEVENDRA_WIN32_GL_API int32 DrawGLScene(uint32 VAO, uint32 indiciesCount);
DEVENDRA_WIN32_GL_API GLvoid KillGLWindow(Devendra_Window* DWindow);
DEVENDRA_WIN32_GL_API bool32 CreateGLWindow(Devendra_Window* DWindow, WNDPROC WndProc);

typedef struct Devendra_Shader
{
    uint32 ShaderProgramID;
    uint32 vertexShaderID;
    uint32 fragmentShaderID;
    bool32 active;
}Devendra_Shader;

DEVENDRA_WIN32_GL_API void CompileFragmentShader(Devendra_Shader* shader, const char* fragmentShaderPath);
DEVENDRA_WIN32_GL_API void CompileVertexShader(Devendra_Shader* shader, const char* vertexShaderPath);
DEVENDRA_WIN32_GL_API void CompileShaderProgram(Devendra_Shader* shader);
DEVENDRA_WIN32_GL_API void useShader(Devendra_Shader* shader);

DEVENDRA_WIN32_GL_API void setUniform1i(Devendra_Shader* shader);
DEVENDRA_WIN32_GL_API void setUniform2i(Devendra_Shader* shader);
DEVENDRA_WIN32_GL_API void setUniform3i(Devendra_Shader* shader);
DEVENDRA_WIN32_GL_API void setUniform4i(Devendra_Shader* shader);

DEVENDRA_WIN32_GL_API void setUniform1f(Devendra_Shader* shader, const char* name, real32 value);
DEVENDRA_WIN32_GL_API void setUniform2f(Devendra_Shader* shader);
DEVENDRA_WIN32_GL_API void setUniform3f(Devendra_Shader* shader);
DEVENDRA_WIN32_GL_API void setUniform4f(Devendra_Shader* shader);

DEVENDRA_WIN32_GL_API Devendra_Shader* createShaderFromHeap(Devendra_Shader* shader);

#endif