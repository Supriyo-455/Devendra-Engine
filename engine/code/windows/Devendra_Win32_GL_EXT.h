#pragma once

#include "defines.h"
#include <windows.h>     // Header File For Windows
#include <gl/gl.h>       // Header File For The OpenGL32 Library
#include <gl/glu.h>      // Header File For The GLu32 Library
#include "libs/glext.h"  // OpenGL 1.2 and above compatibility profile and extension interfaces
#include "libs/wglext.h" // WGL extension interfaces.
#include <stdio.h>

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
PFNGLUNIFORM1IPROC glUniform1i;
PFNGLUNIFORM2IPROC glUniform2i;
PFNGLUNIFORM3IPROC glUniform3i;
PFNGLUNIFORM4IPROC glUniform4i;
PFNGLUNIFORM1FPROC glUniform1f;
PFNGLUNIFORM2FPROC glUniform2f;
PFNGLUNIFORM3FPROC glUniform3f;
PFNGLUNIFORM4FPROC glUniform4f;
PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv;
PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;
PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT;
PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
PFNGLACTIVETEXTUREPROC glActiveTexture;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
PFNGLDELETEBUFFERSPROC glDeleteBuffers;
PFNGLDELETEPROGRAMPROC glDeleteProgram;

void *GetAnyGLFuncAddress(const char *name) // Platform Specific Function Retrieval
{
  void *p = (void *)wglGetProcAddress(name);
  if (p == 0 ||
      (p == (void *)0x1) || (p == (void *)0x2) || (p == (void *)0x3) ||
      (p == (void *)-1))
  {
    HMODULE module = LoadLibraryA("opengl32.dll");
    p = (void *)GetProcAddress(module, name);
  }

  return p;
}

bool32 IsExtensionSupported(const char *name)
{
  int32 n = 0;
  glGetIntegerv(GL_NUM_EXTENSIONS, &n);
  for (int32 i = 0; i < n; i++)
  {
    const char *extension = (const char *)glGetStringi(GL_EXTENSIONS, i);
    if (!strcmp(name, extension))
    {
      return true;
    }
  }
  return false;
}

void PrintSupportedOpenGLExtensions()
{
  int32 n = 0;
  glGetIntegerv(GL_NUM_EXTENSIONS, &n);
  glGetStringi = (PFNGLGETSTRINGIPROC)wglGetProcAddress("glGetStringi");
  char Buffer[250];

  sprintf_s(Buffer, 250, "\nOpengl Version: %s\n", glGetString(GL_VERSION));
  OutputDebugStringA(Buffer);

  int32 nrAttributes;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
  sprintf_s(Buffer, 250, "\nMaximum %d no.s of vertex attributes supported.\n\n", nrAttributes);
  OutputDebugStringA(Buffer);

  for (int32 i = 0; i < n; i++)
  {
    const char *extension =
        (const char *)glGetStringi(GL_EXTENSIONS, i);
    // TODO: Optimize this wsprintf or replace it with something else
    sprintf_s(Buffer, 250, "Ext %d: %s\n", i, extension);
    OutputDebugStringA(Buffer);
  }
}

void InitGLFunctions(HDC hDC, HGLRC hRC)
{
  wglCreateContext(hDC);
  wglMakeCurrent(hDC, hRC);

  glUseProgram = (PFNGLUSEPROGRAMPROC)GetAnyGLFuncAddress("glUseProgram");
  glGenBuffers = (PFNGLGENBUFFERSPROC)GetAnyGLFuncAddress("glGenBuffers");
  glBindBuffer = (PFNGLBINDBUFFERPROC)GetAnyGLFuncAddress("glBindBuffer");
  glBufferData = (PFNGLBUFFERDATAPROC)GetAnyGLFuncAddress("glBufferData");
  glCreateShader = (PFNGLCREATESHADERPROC)GetAnyGLFuncAddress("glCreateShader");
  glShaderSource = (PFNGLSHADERSOURCEPROC)GetAnyGLFuncAddress("glShaderSource");
  glCompileShader = (PFNGLCOMPILESHADERPROC)GetAnyGLFuncAddress("glCompileShader");
  glGetShaderiv = (PFNGLGETSHADERIVPROC)GetAnyGLFuncAddress("glGetShaderiv");
  glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)GetAnyGLFuncAddress("glGetShaderInfoLog");
  glCreateProgram = (PFNGLCREATEPROGRAMPROC)GetAnyGLFuncAddress("glCreateProgram");
  glAttachShader = (PFNGLATTACHSHADERPROC)GetAnyGLFuncAddress("glAttachShader");
  glLinkProgram = (PFNGLLINKPROGRAMPROC)GetAnyGLFuncAddress("glLinkProgram");
  glGetProgramiv = (PFNGLGETPROGRAMIVPROC)GetAnyGLFuncAddress("glGetProgramiv");
  glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)GetAnyGLFuncAddress("glGetProgramInfoLog");
  glDeleteShader = (PFNGLDELETESHADERPROC)GetAnyGLFuncAddress("glDeleteShader");
  glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)GetAnyGLFuncAddress("glVertexAttribPointer");
  glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)GetAnyGLFuncAddress("glEnableVertexAttribArray");
  glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)GetAnyGLFuncAddress("glDisableVertexAttribArray");
  glGetStringi = (PFNGLGETSTRINGIPROC)GetAnyGLFuncAddress("glGetStringi");
  glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)GetAnyGLFuncAddress("glGenVertexArrays");
  glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)GetAnyGLFuncAddress("glBindVertexArray");
  glUniform1i = (PFNGLUNIFORM1IPROC)GetAnyGLFuncAddress("glUniform1i");
  glUniform2i = (PFNGLUNIFORM2IPROC)GetAnyGLFuncAddress("glUniform2i");
  glUniform3i = (PFNGLUNIFORM3IPROC)GetAnyGLFuncAddress("glUniform3i");
  glUniform4i = (PFNGLUNIFORM4IPROC)GetAnyGLFuncAddress("glUniform4i");
  glUniform1f = (PFNGLUNIFORM1FPROC)GetAnyGLFuncAddress("glUniform1f");
  glUniform2f = (PFNGLUNIFORM2FPROC)GetAnyGLFuncAddress("glUniform2f");
  glUniform3f = (PFNGLUNIFORM3FPROC)GetAnyGLFuncAddress("glUniform3f");
  glUniform4f = (PFNGLUNIFORM4FPROC)GetAnyGLFuncAddress("glUniform4f");
  glUniformMatrix2fv = (PFNGLUNIFORMMATRIX2FVPROC)GetAnyGLFuncAddress("glUniformMatrix2fv");
  glUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVPROC)GetAnyGLFuncAddress("glUniformMatrix3fv");
  glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)GetAnyGLFuncAddress("glUniformMatrix4fv");
  glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)GetAnyGLFuncAddress("glGetUniformLocation");
  wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)GetAnyGLFuncAddress("wglSwapIntervalEXT");
  wglGetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC)GetAnyGLFuncAddress("wglGetSwapIntervalEXT");
  glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)GetAnyGLFuncAddress("glGenerateMipmap");
  glActiveTexture = (PFNGLACTIVETEXTUREPROC)GetAnyGLFuncAddress("glActiveTexture");
  glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)GetAnyGLFuncAddress("glDeleteVertexArrays");
  glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)GetAnyGLFuncAddress("glDeleteBuffers");
  glDeleteProgram = (PFNGLDELETEPROGRAMPROC)GetAnyGLFuncAddress("glDeleteProgram");
}