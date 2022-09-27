#include "../include/Devendra_Win32_GL_EXT.h"
#include <stdio.h>

void *GetAnyGLFuncAddress(const char *name)             // Platform Specific Function Retrieval
{
  void *p = (void *)wglGetProcAddress(name);
  if(p == 0 ||
    (p == (void*)0x1) || (p == (void*)0x2) || (p == (void*)0x3) ||
    (p == (void*)-1) )
  {
    HMODULE module = LoadLibraryA("opengl32.dll");
    p = (void *)GetProcAddress(module, name);
  }

  return p;
}

bool32 IsExtensionSupported(const char *name)
{
    int32 n=0; 
    glGetIntegerv(GL_NUM_EXTENSIONS, &n); 
    for (int32 i=0; i<n; i++) 
    { 
        const char* extension = (const char*)glGetStringi(GL_EXTENSIONS, i);
        if (!strcmp(name, extension)) 
        {
            return true;
        }
    }
    return false;
}

void PrintSupportedOpenGLExtensions()
{
    int32 n=0; 
    glGetIntegerv(GL_NUM_EXTENSIONS, &n);
    glGetStringi = (PFNGLGETSTRINGIPROC)wglGetProcAddress("glGetStringi");
    char Buffer[250];
    
    sprintf_s(Buffer, 250, "\nOpengl Version: %s\n", glGetString(GL_VERSION));
    OutputDebugStringA(Buffer);
    
    int32 nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    sprintf_s(Buffer, 250, "\nMaximum %d no.s of vertex attributes supported.\n\n", nrAttributes);
    OutputDebugStringA(Buffer);

    for (int32 i=0; i<n; i++) 
    { 
        const char* extension = 
            (const char*)glGetStringi(GL_EXTENSIONS, i);
      // TODO: Optimize this wsprintf or replace it with something else
      sprintf_s(Buffer, 250, "Ext %d: %s\n", i, extension);
      OutputDebugStringA(Buffer);
    } 
}