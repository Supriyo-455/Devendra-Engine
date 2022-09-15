#include "include/Devendra_Shader.h"

void CompileFragmentShader(Devendra_Shader* shader, const char* fragmentShaderPath)
{
    const char* fragmentShaderSource = readFile(fragmentShaderPath);
    // Compiling fragment shader
    if(fragmentShaderSource == NULL)        // fragment Shader loading from file failed!
    {
        char Buffer[512];
        sprintf_s(Buffer, 512, "\nERROR::SHADER::FRAGMENT::LOADING_FAILED_FROM_FILE\n");
        OutputDebugStringA(Buffer);
        shader->fragmentShaderID = NULL;
        return;
    }

    uint32 fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    bool32 success = false;
    char infoLog[512];
    // Checking fragment shader compilation check
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        char Buffer[512];
        sprintf_s(Buffer, 512, "\nERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s", infoLog);
        OutputDebugStringA(Buffer);
        shader->fragmentShaderID = NULL;
        return;
    }

    shader->fragmentShaderID = fragmentShader;
}

void CompileVertexShader(Devendra_Shader* shader, const char* vertexShaderPath)
{   
    const char* vertexShaderSource = readFile(vertexShaderPath);
    // Compiling a fragment shader
    if(vertexShaderSource == NULL)      // Vertex Shader loading from file failed!
    {
        char Buffer[512];
        sprintf_s(Buffer, 512, "\nERROR::SHADER::VERTEX::LOADING_FAILED_FROM_FILE\n");
        OutputDebugStringA(Buffer);
        shader->vertexShaderID = NULL;
        return;
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
        shader->vertexShaderID = NULL;
        return;
    }

    shader->vertexShaderID = vertexShader;
}

void CompileShaderProgram(Devendra_Shader* shader)
{
    shader->ShaderProgramID = glCreateProgram();

    glAttachShader(shader->ShaderProgramID, shader->vertexShaderID);
    glAttachShader(shader->ShaderProgramID, shader->fragmentShaderID);
    glLinkProgram(shader->ShaderProgramID);

    // check for shader program errors!
    bool32 success;
    char infoLog[512];
    glGetProgramiv(shader->ShaderProgramID, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shader->ShaderProgramID, 512, NULL, infoLog);
        char Buffer[512];
        sprintf_s(Buffer, 512, "\nERROR::SHADER::PROGRAM::COMPILATION_FAILED\n%s", infoLog);
        OutputDebugStringA(Buffer);
        shader->ShaderProgramID = NULL;
    }      

    // Delete the shader objects from cpu memory
    glDeleteShader(shader->vertexShaderID);                               
    glDeleteShader(shader->fragmentShaderID);
}

void useShader(Devendra_Shader* shader)
{
    glUseProgram(shader->ShaderProgramID);
    shader->active = false;
}

void setUniform1i(Devendra_Shader* shader);
void setUniform2i(Devendra_Shader* shader);
void setUniform3i(Devendra_Shader* shader);
void setUniform4i(Devendra_Shader* shader);

void setUniform1f(Devendra_Shader* shader, const char* name, real32 value)
{
    glUniform1f(glGetUniformLocation(shader->ShaderProgramID, name), value);
}
void setUniform2f(Devendra_Shader* shader);
void setUniform3f(Devendra_Shader* shader);
void setUniform4f(Devendra_Shader* shader);

Devendra_Shader* createShaderFromHeap(Devendra_Shader* shader);