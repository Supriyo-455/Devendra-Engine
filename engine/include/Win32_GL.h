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
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
PFNGLUNIFORM1FPROC glUniform1f;
PFNGLUNIFORM2FPROC glUniform2f;
PFNGLUNIFORM3FPROC glUniform3f;
PFNGLUNIFORM4FPROC glUniform4f;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;
PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT;

const char* readFile(const char *filePath);
void *GetAnyGLFuncAddress(const char *name);
bool32 IsExtensionSupported(const char *extension_name);
void PrintSupportedOpenGLExtensions();
void InitGLFunctions(HDC hDC, HGLRC hRC);
void ReSizeGLScene(uint32 width, uint32 height);
bool32 InitGL(void);
int32 CompileShaders(const char *vertexShader, const char *fragmentShader);
int32 DrawGLScene(uint32 ShaderProgram, uint32 VAO, uint32 indiciesCount, real32 time);

#define _WIN32_GL_H
#endif