#ifndef __DEVENDRA_WIN32_GL_EXT_H__
#define __DEVENDRA_WIN32_GL_EXT_H__

#include "defines.h"
#include <windows.h>                              // Header File For Windows
#include <gl/gl.h>                                // Header File For The OpenGL32 Library
#include <gl/glu.h>                               // Header File For The GLu32 Library
#include "../libs/glext.h"                             // OpenGL 1.2 and above compatibility profile and extension interfaces
#include "../libs/wglext.h"                            // WGL extension interfaces.

#ifdef __DEVENDRA_EXPORTS__
#define DEVENDRA_GL_EXT   __declspec(dllexport)
#else
   #define DEVENDRA_GL_EXT    __declspec(dllimport)
#endif

PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLGENBUFFERSPROC glGenBuffers;
PFNGLBINDBUFFERPROC glBindBuffer;
PFNGLBUFFERDATAPROC glBufferData;
PFNGLCREATESHADERPROC glCreateShader;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLCOMPILESHADERPROC glCompileShader; 
PFNGLGETSHADERIVPROC glGetShaderiv;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLATTACHSHADERPROC glAttachShader;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLGETPROGRAMIVPROC glGetProgramiv;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
PFNGLDELETESHADERPROC glDeleteShader;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray; 
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
PFNGLGETSTRINGIPROC glGetStringi;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
PFNGLUNIFORM1FPROC glUniform1f;
PFNGLUNIFORM2FPROC glUniform2f;
PFNGLUNIFORM3FPROC glUniform3f;
PFNGLUNIFORM4FPROC glUniform4f;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;
PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT;
PFNGLGENERATEMIPMAPPROC glGenerateMipmap;

DEVENDRA_GL_EXT void *GetAnyGLFuncAddress(const char *name);
DEVENDRA_GL_EXT bool32 IsExtensionSupported(const char *extension_name);
DEVENDRA_GL_EXT void PrintSupportedOpenGLExtensions();
void InitGLFunctions(HDC hDC, HGLRC hRC) 
{
    wglCreateContext(hDC);
    wglMakeCurrent(hDC, hRC);

    glUseProgram = (PFNGLUSEPROGRAMPROC) GetAnyGLFuncAddress("glUseProgram");
    glGenBuffers = (PFNGLGENBUFFERSPROC) GetAnyGLFuncAddress("glGenBuffers");
    glBindBuffer = (PFNGLBINDBUFFERPROC) GetAnyGLFuncAddress("glBindBuffer");
    glBufferData = (PFNGLBUFFERDATAPROC) GetAnyGLFuncAddress("glBufferData");
    glCreateShader = (PFNGLCREATESHADERPROC) GetAnyGLFuncAddress("glCreateShader");
    glShaderSource = (PFNGLSHADERSOURCEPROC) GetAnyGLFuncAddress("glShaderSource");
    glCompileShader = (PFNGLCOMPILESHADERPROC) GetAnyGLFuncAddress("glCompileShader");
    glGetShaderiv = (PFNGLGETSHADERIVPROC) GetAnyGLFuncAddress("glGetShaderiv");
    glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC) GetAnyGLFuncAddress("glGetShaderInfoLog");
    glCreateProgram = (PFNGLCREATEPROGRAMPROC) GetAnyGLFuncAddress("glCreateProgram");
    glAttachShader = (PFNGLATTACHSHADERPROC) GetAnyGLFuncAddress("glAttachShader");
    glLinkProgram = (PFNGLLINKPROGRAMPROC) GetAnyGLFuncAddress("glLinkProgram");
    glGetProgramiv = (PFNGLGETPROGRAMIVPROC) GetAnyGLFuncAddress("glGetProgramiv");
    glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC) GetAnyGLFuncAddress("glGetProgramInfoLog");
    glDeleteShader = (PFNGLDELETESHADERPROC) GetAnyGLFuncAddress("glDeleteShader");
    glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC) GetAnyGLFuncAddress("glVertexAttribPointer");
    glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC) GetAnyGLFuncAddress("glEnableVertexAttribArray");
    glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC) GetAnyGLFuncAddress("glDisableVertexAttribArray");
    glGetStringi = (PFNGLGETSTRINGIPROC) GetAnyGLFuncAddress("glGetStringi");
    glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC) GetAnyGLFuncAddress("glGenVertexArrays");
    glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC) GetAnyGLFuncAddress("glBindVertexArray");
    glUniform1f = (PFNGLUNIFORM1FPROC) GetAnyGLFuncAddress("glUniform1f");
    glUniform2f = (PFNGLUNIFORM2FPROC) GetAnyGLFuncAddress("glUniform2f");
    glUniform3f = (PFNGLUNIFORM3FPROC) GetAnyGLFuncAddress("glUniform3f");
    glUniform4f = (PFNGLUNIFORM4FPROC) GetAnyGLFuncAddress("glUniform4f");
    glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC) GetAnyGLFuncAddress("glGetUniformLocation");
    wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC) GetAnyGLFuncAddress("wglSwapIntervalEXT");
    wglGetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC) GetAnyGLFuncAddress("wglGetSwapIntervalEXT");
    glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC) GetAnyGLFuncAddress("glGenerateMipmap");
}

#endif