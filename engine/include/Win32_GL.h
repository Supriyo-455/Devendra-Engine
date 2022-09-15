#ifndef _WIN32_GL_H
#define _WIN32_GL_H

void ReSizeGLScene(uint32 width, uint32 height);
bool32 InitGL(void);
int32 DrawGLScene(uint32 VAO, uint32 indiciesCount);

#endif