#include "../include/Devendra_Renderer.h" 

/**
 * TODO:
 *  1) Method to delete the buffers from gpu memory
 *  2) Method to delete the shaders from gpu memory
 * 
*/

void RendererInit(Devendra_Renderer* renderer, Devendra_Window* window)
{
    renderer->DWindow = window;
    stbi_set_flip_vertically_on_load(true);
    InitGLFunctions(renderer->DWindow->hDC, renderer->DWindow->hRC);
}

void RendererBindBuffers(Devendra_Renderer* renderer, real32 vertices[], uint32 vertexCount, uint32 indices[], uint32 indiciesCount, uint32 stride)
{
    renderer->vertexCount = vertexCount;
    renderer->indiciesCount = indiciesCount;
    renderer->stride = stride;
    
     // Vertex Array Object
    uint32 VAO;
    glGenVertexArrays(1, &VAO); 
    glBindVertexArray(VAO);

    // Vertex Buffer Object
    uint32 VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3* sizeof(real32)));
    glEnableVertexAttribArray(1);
    // Texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(real32)));
    glEnableVertexAttribArray(2);  

    // Element Buffer Object
    uint32 EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);   

    glVertexAttribPointer(0, vertexCount, GL_FLOAT, GL_FALSE, vertexCount * sizeof(real32), (void*)0);
    glEnableVertexAttribArray(0);
    
    renderer->VAO = VAO;
    renderer->VBO = VBO;
    renderer->EBO = EBO;
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);         // Clear The Screen And The Depth Buffer
    glLoadIdentity();                                           // Reset The Current Modelview Matrix
    glBindVertexArray(renderer->VAO);
    glDrawElements(GL_TRIANGLES, renderer->indiciesCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    SwapBuffers(renderer->DWindow->hDC);
}

bool32 RendererLoadShader(Devendra_Renderer* renderer, Devendra_Shader* shader)
{
    renderer->DShader = shader;
    const char* vertexShaderSource = readFile(renderer->DShader->vertexShaderPath);
    const char* fragmentShaderSource = readFile(renderer->DShader->fragmentShaderPath);


    renderer->DShader->vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(renderer->DShader->vertexShaderID, 1, &vertexShaderSource, NULL);
    glCompileShader(renderer->DShader->vertexShaderID);
    bool32 success = false;
    char infoLog[512];
    glGetShaderiv(renderer->DShader->vertexShaderID, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(renderer->DShader->vertexShaderID, 512, NULL, infoLog);
        char Buffer[512];
        sprintf_s(Buffer, 512, "\nERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s", infoLog);
        OutputDebugStringA(Buffer);
        renderer->DShader->vertexShaderID = NULL;
        return false;
    }

    renderer->DShader->fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(renderer->DShader->fragmentShaderID, 1, &fragmentShaderSource, NULL);
    glCompileShader(renderer->DShader->fragmentShaderID);
    glGetShaderiv(renderer->DShader->fragmentShaderID, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(renderer->DShader->fragmentShaderID, 512, NULL, infoLog);
        char Buffer[512];
        sprintf_s(Buffer, 512, "\nERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s", infoLog);
        OutputDebugStringA(Buffer);
        renderer->DShader->fragmentShaderID = NULL;
        return false;
    }
    
    renderer->DShader->ShaderProgramID = glCreateProgram();
    glAttachShader(renderer->DShader->ShaderProgramID, renderer->DShader->vertexShaderID);
    glAttachShader(renderer->DShader->ShaderProgramID, renderer->DShader->fragmentShaderID);
    glLinkProgram(renderer->DShader->ShaderProgramID);
    glGetProgramiv(renderer->DShader->ShaderProgramID, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(renderer->DShader->ShaderProgramID, 512, NULL, infoLog);
        char Buffer[512];
        sprintf_s(Buffer, 512, "\nERROR::SHADER::PROGRAM::COMPILATION_FAILED\n%s", infoLog);
        OutputDebugStringA(Buffer);
        renderer->DShader->ShaderProgramID = NULL;
        return false;
    } 

    
     // Delete the shader objects from cpu memory
    glDeleteShader(renderer->DShader->vertexShaderID);                               
    glDeleteShader(renderer->DShader->fragmentShaderID);

    return true;
}

void RendererUseShader(Devendra_Renderer* renderer)
{
    glUseProgram(renderer->DShader->ShaderProgramID);
    renderer->DShader->active = true;
}

void RendererUnloadShader(Devendra_Renderer* renderer)
{
    glDeleteProgram(renderer->DShader->ShaderProgramID);
    renderer->DShader->active = false;
}

bool32 LoadTexture(Devendra_Texture* texture)
{   
    // TODO: Decouple hard coded texture attributes
    texture->type = GL_UNSIGNED_BYTE;
    texture->wrapS = GL_REPEAT;
    texture->wrapT = GL_REPEAT;
    texture->filterMin = GL_LINEAR;
    texture->filterMax = GL_LINEAR;

    GLuint tId;
    glGenTextures(1, &tId);
    texture->textureID = tId;
    
    glBindTexture(GL_TEXTURE_2D, texture->textureID);
    
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture->wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture->wrapT);
    // set texture filtering parameters   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture->filterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture->filterMax);

    int width, height, nrChannels;
    uint8* pixels = stbi_load(texture->texPath, &width, &height, &nrChannels, 0);
    texture->width = width;
    texture->height = height;
    texture->channels = nrChannels;

    bool32 flag = (pixels != NULL);

    if(flag)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, texture->format, texture->width, texture->height, 0, texture->format, texture->type, pixels);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else
    {
        char errorMessage[256];
        sprintf_s(errorMessage, "Failed to load texture %s. Reason: %s", texture->texPath, stbi_failure_reason());
        OutputDebugStringA(errorMessage);
    }

    stbi_image_free(pixels);
    return !flag;
}

void BindTexture(Devendra_Texture* texture, GLenum textureUnit)
{
    glActiveTexture(textureUnit);
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
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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