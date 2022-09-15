#include "include/Win32_GL.h"

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

int32 DrawGLScene(uint32 VAO, uint32 indiciesCount) // Here's Where We Do All The Drawing
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);         // Clear The Screen And The Depth Buffer
    glLoadIdentity();                                           // Reset The Current Modelview Matrix
    
    // Linking Vertex Attributes
    glBindVertexArray(VAO);
    
    // Drawing
    glDrawElements(GL_TRIANGLES, indiciesCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    return TRUE;                                                // Everything Went OK
}