#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include "../../libs/stb_image.h"
#include "../common/defines.h"
#include "../common/Devendra_Utils.h"

#ifdef _WIN32
#include "../win32/Devendra_Win32_GL.h"
#include "../win32/Devendra_Win32_GL_EXT.h"
#endif

// TODO: Abstract out the ways to work with different graphics api

#define VSYNC 1
#define ADAPTIVE_VSYNC -1
#define No_VSYNC 0

#ifdef __DEVENDRA_RENDERER__
    #define DEVENDRA_RENDERER   __declspec(dllexport)
#else
   #define DEVENDRA_RENDERER    __declspec(dllimport)
#endif


typedef struct Devendra_Renderer
{
    Devendra_Window* DWindow;
    Devendra_Shader* DShader;
    uint32 VAO;
    uint32 VBO;
    uint32 EBO;
    uint32 vertexCount;
    uint32 indiciesCount;
    uint32 stride;
}Devendra_Renderer;

typedef struct Devendra_Texture
{
    uint32 textureID;
    int width;
    int height;
    int channels;
    uint32 format;
    uint32 type;
    uint32 wrapS;
    uint32 wrapT;
    uint32 filterMin;
    uint32 filterMax;
    bool32 active;
    const char* texPath;
}Devendra_Texture;

// RendererInit creates the window and initializes the renderer
DEVENDRA_RENDERER void RendererInit(Devendra_Renderer* renderer, Devendra_Window* window);

// Bind the buffers to the renderer
DEVENDRA_RENDERER void RendererBindBuffers(Devendra_Renderer* renderer, real32 vertices[], uint32 vertexCount, uint32 indices[], uint32 indiciesCount, uint32 stride);

// Draws on the screen
DEVENDRA_RENDERER void RendererDraw(Devendra_Renderer* renderer);

// Loads the shader into the renderer
DEVENDRA_RENDERER bool32 RendererLoadShader(Devendra_Renderer* renderer, Devendra_Shader* shader);

// Use shader
DEVENDRA_RENDERER void RendererUseShader(Devendra_Renderer* renderer);

// Unload the shader from the renderer
DEVENDRA_RENDERER void RendererUnloadShader(Devendra_Renderer* renderer);

// Dispose the shader
DEVENDRA_RENDERER void ShaderDispose(Devendra_Shader* shader);

// Loads the texture into the renderer
DEVENDRA_RENDERER bool32 LoadTexture(Devendra_Texture* texture);

// Bind texture to the shader
DEVENDRA_RENDERER void BindTexture(Devendra_Texture* texture, GLenum textureUnit);

// Unbinds VAO, VBO, EBO
DEVENDRA_RENDERER void UnbindBuffers(Devendra_Renderer* renderer);

// Set VSync
DEVENDRA_RENDERER void SetVSync(int32 vsync);

// Renderer Dispose
DEVENDRA_RENDERER void RendererDispose(Devendra_Renderer* renderer);

// Get Uniform location in shader
DEVENDRA_RENDERER uint32 ShaderGetUniformLocation(Devendra_Shader* shader, const char* name);

DEVENDRA_RENDERER void ShaderSetInt(Devendra_Shader* shader, const char* name, int value);

DEVENDRA_RENDERER void ShaderSetFloat(Devendra_Shader* shader, const char* name, float value);