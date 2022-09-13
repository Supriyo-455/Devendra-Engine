#ifndef _WIN32_GL_H

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

const char* readFile(const char *filePath);
void *GetAnyGLFuncAddress(const char *name);
bool IsExtensionSupported(const char *extension_name);
void PrintSupportedOpenGLExtensions();
void InitGLFunctions(HDC hDC, HGLRC hRC);
GLvoid ReSizeGLScene(GLsizei width, GLsizei height);
int InitGL(GLvoid);
GLint CompileShaders();
int DrawGLScene(unsigned int* VBO);

#define _WIN32_GL_H
#endif