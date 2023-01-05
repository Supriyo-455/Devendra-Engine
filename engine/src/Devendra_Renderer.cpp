#include "../include/Devendra_Renderer.h"

void RendererInit(Devendra_Renderer* renderer, Devendra_Window* window)
{
    renderer->DWindow = window;
   
    // Init the opengl functions
    InitGLFunctions(renderer->DWindow->hDC, renderer->DWindow->hRC);
    InitGL();
}

void RendererBindBuffers(Devendra_Renderer* renderer, real32 vertices[], uint32 vertexCount, uint32 indices[], uint32 indiciesCount, uint32 stride)
{
    renderer->vertexCount = vertexCount;
    renderer->indiciesCount = indiciesCount;
    renderer->stride = stride;
    
    // Vertex Array Object
    glGenVertexArrays(1, &renderer->VAO);
    glBindVertexArray(renderer->VAO);
    
    // Vertex Buffer Object
    glGenBuffers(1, &renderer->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(real32), vertices, GL_STATIC_DRAW);
    
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    // Element Buffer Object
    glGenBuffers(1, &renderer->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indiciesCount * sizeof(uint32), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);
    glVertexAttribPointer(0, vertexCount, GL_FLOAT, GL_FALSE, vertexCount * sizeof(real32), (void*)0);
    glEnableVertexAttribArray(0);
}

void RendererDraw(Devendra_Renderer* renderer)
{
    if(renderer->DWindow->wireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    glBindVertexArray(renderer->VAO);
    glDrawElements(GL_TRIANGLES, renderer->indiciesCount, GL_UNSIGNED_INT, 0);
    SwapBuffers(renderer->DWindow->hDC);
    glBindVertexArray(0);
}

bool32 RendererLoadShader(Devendra_Renderer* renderer, Devendra_Shader* shader)
{
    renderer->DShader = shader;
    renderer->DShader->active = true;
    const char* vertexShaderSource = readFile(renderer->DShader->vertexShaderPath);
    const char* fragmentShaderSource = readFile(renderer->DShader->fragmentShaderPath);


    renderer->DShader->vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(renderer->DShader->vertexShaderID, 1, &vertexShaderSource, NULL);
    glCompileShader(renderer->DShader->vertexShaderID);
    bool32 success = false;
    glGetShaderiv(renderer->DShader->vertexShaderID, GL_COMPILE_STATUS, &success);
    if(!success) return false;

    renderer->DShader->fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(renderer->DShader->fragmentShaderID, 1, &fragmentShaderSource, NULL);
    glCompileShader(renderer->DShader->fragmentShaderID);
    glGetShaderiv(renderer->DShader->fragmentShaderID, GL_COMPILE_STATUS, &success);
    if(!success) return false;
    
    renderer->DShader->ShaderProgramID = glCreateProgram();
    glAttachShader(renderer->DShader->ShaderProgramID, renderer->DShader->vertexShaderID);
    glAttachShader(renderer->DShader->ShaderProgramID, renderer->DShader->fragmentShaderID);
    
    glUseProgram(renderer->DShader->ShaderProgramID);
    glLinkProgram(renderer->DShader->ShaderProgramID);
    
    glDeleteShader(renderer->DShader->vertexShaderID);
    glDeleteShader(renderer->DShader->fragmentShaderID);

    return true;
}

void RendererUseShader(Devendra_Renderer* renderer)
{
    
}

void RendererUnloadShader(Devendra_Renderer* renderer)
{
    glDeleteShader(renderer->DShader->vertexShaderID);
    glDeleteShader(renderer->DShader->fragmentShaderID);
    glDeleteProgram(renderer->DShader->ShaderProgramID);
    renderer->DShader->active = false;
}

// TODO: Decouple hardcoded texture attributes
Devendra_Texture CreateTexture(const char* path)
{
    Devendra_Texture texture = {};
    texture.pixels = stbi_load(path, &texture.width, &texture.height, &texture.channels, 0);
    if (texture.pixels)
    {
        texture.format = GL_RGB;
        texture.type = GL_UNSIGNED_BYTE;
        texture.wrapS = GL_REPEAT;
        texture.wrapT = GL_REPEAT;
        texture.filterMin = GL_LINEAR;
        texture.filterMax = GL_LINEAR;
    }
    return texture;
}

bool32 LoadTexture(Devendra_Texture* texture)
{   
    bool32 flag = texture->pixels != NULL;
    if(flag)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, texture->format, texture->width, texture->height, 0, texture->format, texture->type, texture->pixels);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture->wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture->wrapT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture->filterMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture->filterMax);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    stbi_image_free(texture->pixels);
    return !flag;
}

void BindTexture(Devendra_Texture* texture, uint32 textureUnit)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->textureID);
    texture->active = true;
}

void UnbindBuffers(Devendra_Renderer* renderer)
{
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void RendererClear(Devendra_Renderer* renderer)
{
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void SetVSync(int32 vsync)
{
    wglSwapIntervalEXT(vsync);
}

void RendererDispose(Devendra_Renderer* renderer)
{
    glDeleteVertexArrays(1, &renderer->VAO);
    glDeleteBuffers(1, &renderer->VBO);
    glDeleteBuffers(1, &renderer->EBO);
    free(renderer);
}

void ShaderSetFloat(Devendra_Shader* shader, const char* name, float value)
{
    glUniform1f(glGetUniformLocation(shader->ShaderProgramID, name), value);
}

void ShaderSetInt(Devendra_Shader* shader, const char* name, int value)
{
    glUniform1i(glGetUniformLocation(shader->ShaderProgramID, name), value);
}

uint32 ShaderGetUniformLocation(Devendra_Shader* shader, const char* name)
{
    return glGetUniformLocation(shader->ShaderProgramID, name);
}

#if 0
void ShaderSetVec3(Devendra_Shader* shader, const char* name, float x, float y, float z)
{
    glUniform3f(glGetUniformLocation(shader->shaderProgram, name), x, y, z);
}

void ShaderSetVec3(Devendra_Shader* shader, const char* name, glm::vec3 value)
{
    glUniform3f(glGetUniformLocation(shader->shaderProgram, name), value.x, value.y, value.z);
}

void ShaderSetMat4(Devendra_Shader* shader, const char* name, glm::mat4 value)
{
    glUniformMatrix4fv(glGetUniformLocation(shader->shaderProgram, name), 1, GL_FALSE, glm::value_ptr(value));
}
#else
#endif

void ShaderDispose(Devendra_Shader* shader)
{
    glDeleteProgram(shader->ShaderProgramID);
    free(shader);
}