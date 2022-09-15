#ifndef _DEVENDRA_SHADER_H
#define _DEVENDRA_SHADER_H

typedef struct Devendra_Shader
{
    uint32 ShaderProgramID;
    uint32 vertexShaderID;
    uint32 fragmentShaderID;
    bool32 active;
}Devendra_Shader;

const char* __readFile(const char *filePath);

void CompileFragmentShader(Devendra_Shader* shader, const char* fragmentShaderPath);
void CompileVertexShader(Devendra_Shader* shader, const char* vertexShaderPath);
void CompileShaderProgram(Devendra_Shader* shader);
void useShader(Devendra_Shader* shader);

void setUniform1i(Devendra_Shader* shader);
void setUniform2i(Devendra_Shader* shader);
void setUniform3i(Devendra_Shader* shader);
void setUniform4i(Devendra_Shader* shader);

void setUniform1f(Devendra_Shader* shader, const char* name, real32 value);
void setUniform2f(Devendra_Shader* shader);
void setUniform3f(Devendra_Shader* shader);
void setUniform4f(Devendra_Shader* shader);

Devendra_Shader* createShaderFromHeap(Devendra_Shader* shader);

#endif