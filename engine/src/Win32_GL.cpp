#include "include/Win32_GL.h"

const char* readFile(const char *filePath)                          // For shader loading
{
    FILE* shaderFile;
    fopen_s(&shaderFile, filePath, "rb");
    uint32 fileSize = 0;
    char* shaderCode = NULL;

    if(!shaderFile)
    {
        printf("Could not read file! %s. File doesn't exist!", filePath);
        return shaderCode;
    }

    //Getting File Size
    fseek(shaderFile, 0, SEEK_END);
    fileSize = ftell(shaderFile);
    rewind(shaderFile); 

    //Reading From File
    shaderCode = (char*)malloc(sizeof(char) * (fileSize+1));
    fread(shaderCode, sizeof(char), fileSize, shaderFile);
    fclose(shaderFile);
    shaderCode[fileSize] = '\0';
    return shaderCode;
}

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
}

void ReSizeGLScene(int32 width, int32 height)                 // Resize And Initialize The GL Window
{
    if (height==0)                              // Prevent A Divide By Zero By
    {
        height=1;                               // Making Height Equal One
    }
 
    glViewport(0, 0, width, height);                    // Reset The Current Viewport
    glMatrixMode(GL_PROJECTION);                        // Select The Projection Matrix
    glLoadIdentity();                                   // Reset The Projection Matrix
 
    // Calculate The Aspect Ratio Of The Window with 45deg viewing angle
    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
 
    glMatrixMode(GL_MODELVIEW);                     // Select The Modelview Matrix
    glLoadIdentity();                               // Reset The Modelview Matrix
}

bool32 InitGL(void)                              // All Setup For OpenGL Goes Here
{
    glShadeModel(GL_SMOOTH);                                // Enables Smooth Shading
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);                   
    glClearDepth(1.0f);                                     // Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);                                // Enables Depth Testing
    glDepthFunc(GL_LEQUAL);                                 // The Type Of Depth Test To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);      // Really Nice Perspective Calculations
    return TRUE;                                            // Initialization Went OK
}

int32 CompileShaders(const char *vertexShaderSource, const char *fragmentShaderSource) 
{
    // Compiling a fragment shader
    if(vertexShaderSource == NULL)      // Vertex Shader loading from file failed!
    {
        char Buffer[512];
        sprintf_s(Buffer, 512, "\nERROR::SHADER::VERTEX::LOADING_FAILED_FROM_FILE\n");
        OutputDebugStringA(Buffer);
        return FALSE;
    }

    uint32 vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);   

    // Checking vertex shader compilation check
    bool32  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        char Buffer[512];
        sprintf_s(Buffer, 512,"\nERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s", infoLog);
        OutputDebugStringA(Buffer);
        return FALSE;
    }

    // Compiling fragment shader
    if(fragmentShaderSource == NULL)        // fragment Shader loading from file failed!
    {
        char Buffer[512];
        sprintf_s(Buffer, 512, "\nERROR::SHADER::FRAGMENT::LOADING_FAILED_FROM_FILE\n");
        OutputDebugStringA(Buffer);
        return FALSE;
    }

    uint32 fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Checking fragment shader compilation check
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        char Buffer[512];
        sprintf_s(Buffer, 512, "\nERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s", infoLog);
        OutputDebugStringA(Buffer);
        return FALSE;
    }

    // Binding vertex and fragment shader in shader program
    uint32 shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // check for shader program errors!
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        char Buffer[512];
        sprintf_s(Buffer, 512, "\nERROR::SHADER::PROGRAM::COMPILATION_FAILED\n%s", infoLog);
        OutputDebugStringA(Buffer);
        return FALSE;
    }      

    // Delete the shader objects from cpu memory
    glDeleteShader(vertexShader);                               
    glDeleteShader(fragmentShader);

    return shaderProgram; 
}

int32 DrawGLScene(uint32 ShaderProgram, uint32 VAO, uint32 indiciesCount, real32 timeValue) // Here's Where We Do All The Drawing
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);         // Clear The Screen And The Depth Buffer
    glLoadIdentity();                                           // Reset The Current Modelview Matrix
    int32 Uniform_time_Location = glGetUniformLocation(ShaderProgram, "time");
    
    glUseProgram(ShaderProgram);
    glUniform1f(Uniform_time_Location, timeValue);

    // Linking Vertex Attributes
    glBindVertexArray(VAO);
    
    // Drawing
    glDrawElements(GL_TRIANGLES, indiciesCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    return TRUE;                                                // Everything Went OK
}