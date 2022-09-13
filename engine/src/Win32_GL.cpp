#include "include/Win32_GL.h"

const char* readFile(const char *filePath)                          // For shader loading
{
    FILE* shaderFile;
    fopen_s(&shaderFile, filePath, "r");
    int fileSize = 0;
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
    shaderCode[fileSize] = '\0';
    fclose(shaderFile);
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

bool IsExtensionSupported(const char *name)
{
    GLint n=0; 
    glGetIntegerv(GL_NUM_EXTENSIONS, &n); 
    for (GLint i=0; i<n; i++) 
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
    GLint n=0; 
    glGetIntegerv(GL_NUM_EXTENSIONS, &n);
    glGetStringi = (PFNGLGETSTRINGIPROC)wglGetProcAddress("glGetStringi");
    char Buffer[250];
    sprintf_s(Buffer, 250, "\nOpengl Version: %s\n", glGetString(GL_VERSION));
    OutputDebugStringA(Buffer);
    for (GLint i=0; i<n; i++) 
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
}

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)                 // Resize And Initialize The GL Window
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

int InitGL(GLvoid)                              // All Setup For OpenGL Goes Here
{
    glShadeModel(GL_SMOOTH);                                // Enables Smooth Shading
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);                   
    glClearDepth(1.0f);                                     // Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);                                // Enables Depth Testing
    glDepthFunc(GL_LEQUAL);                                 // The Type Of Depth Test To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);      // Really Nice Perspective Calculations
    return TRUE;                                            // Initialization Went OK
}

GLint CompileShaders() 
{
    // Compiling a fragment shader
    const char *vertexShaderSource = readFile("E:\\personal project\\Devendra-Engine\\engine\\misc\\shader\\defaultvertex.glsl");
    if(vertexShaderSource == NULL)      // Vertex Shader loading from file failed!
    {
        char Buffer[512];
        sprintf_s(Buffer, 512, "\nERROR::SHADER::VERTEX::LOADING_FAILED_FROM_FILE\n");
        OutputDebugStringA(Buffer);
        return FALSE;
    }

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);   

    // Checking vertex shader compilation check
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        char Buffer[512];
        sprintf_s(Buffer, 512,"\nERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s", infoLog);
        OutputDebugStringA(Buffer);
        return FALSE;
    }

    // Compiling fragment shader
    const char *fragmentShaderSource = readFile("E:\\personal project\\Devendra-Engine\\engine\\misc\\shader\\defaultfragment.glsl");
    if(fragmentShaderSource == NULL)        // fragment Shader loading from file failed!
    {
        char Buffer[512];
        sprintf_s(Buffer, 512, "\nERROR::SHADER::FRAGMENT::LOADING_FAILED_FROM_FILE\n");
        OutputDebugStringA(Buffer);
        return FALSE;
    }

    unsigned int fragmentShader;
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
    unsigned int shaderProgram;
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

int DrawGLScene(unsigned int* VBO)                                         // Here's Where We Do All The Drawing
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);         // Clear The Screen And The Depth Buffer
    glLoadIdentity();                                           // Reset The Current Modelview Matrix
    
    // Linking Vertex Attributes
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
    glDisableVertexAttribArray(0);

    return TRUE;                                                // Everything Went OK
}