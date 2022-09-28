#include <windows.h>                              // Header File For Windows
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#include "include/defines.h"
#include "include/Devendra_Win32_GL_EXT.h"
#include "libs/Win32_keycodes.h"
#include "include/Devendra_Utils.h"
#include "include/Devendra_Win32_GL.h"

#define STB_IMAGE_IMPLEMENTATION
#include "libs/stb_image.h"

Devendra_Window DWindow = {};

bool32    keys[256];                                  // Array Used For The Keyboard Routine

// TODO(supriyo): Create seperate API for user input handle
LRESULT CALLBACK WndProc(   
                HWND    wHandle,                   // Handle For This Window
                uint32    uMsg,                   // Message For This Window
                WPARAM  wParam,                 // Additional Message Information
                LPARAM  lParam)                 // Additional Message Information
{
    switch (uMsg)                               // Check For Windows Messages
    {
        case WM_ACTIVATE:                       // Watch For Window Activate Message
        {
            if (!HIWORD(wParam))                    // Check Minimization State
            {
                DWindow.active=TRUE;                    // Program Is Active
            }
            else
            {
                DWindow.active=FALSE;                   // Program Is No Longer Active
            }
        
            return 0;                       // Return To The Message Loop
        }
        /* 
            NOTE(supriyo): If the message is WM_SYSCOMMAND (system command) we'll compare wParam 
            against the case statements.If wParam is SC_SCREENSAVE or SC_MONITORPOWER either 
            a screensaver is trying to start or the monitor is trying to enter power saving mode. 
            By returning 0 we prevent both those things from happening.
        */
        case WM_SYSCOMMAND:                     // Intercept System Commands
        {
            switch (wParam)                     // Check System Calls
            {
                case SC_SCREENSAVE:             // Screensaver Trying To Start?
                case SC_MONITORPOWER:               // Monitor Trying To Enter Powersave?
                return 0;                   // Prevent From Happening
            }
            break;                          // Exit
        }
        case WM_LBUTTONDOWN:
        {
            DWindow.wireframe = !DWindow.wireframe;
            return 0;
        }
        case WM_CLOSE:                          // Did We Receive A Close Message?
        {
            PostQuitMessage(0);                 // Send A Quit Message
            return 0;                       // Jump Back
        }
        case WM_KEYDOWN:                        // Is A Key Being Held Down?
        {
            keys[wParam] = TRUE;                    // If So, Mark It As TRUE
            return 0;                       // Jump Back
        }
        case WM_KEYUP:                          // Has A Key Been Released?
        {
            keys[wParam] = FALSE;                   // If So, Mark It As FALSE
            return 0;                       // Jump Back
        }
        case WM_SIZE:                           // Resize The OpenGL Window
        {
            ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));       // LoWord=Width, HiWord=Height
            return 0;                       // Jump Back
        }
    }

     // Pass All Unhandled Messages To DefWindowProc
    return DefWindowProc(wHandle,uMsg,wParam,lParam);
}

float counter;
int WINAPI WinMain(HINSTANCE   Instance,              // Instance
                    HINSTANCE   hPrevInstance,              // Previous Instance
                    LPSTR       lpCmdLine,              // Command Line Parameters
                    int         nCmdShow)               // Window Show State
{
    /// @brief  TODO(supriyo): Use this in future?
    /// @param Instance 
    /// @param hPrevInstance 
    /// @param lpCmdLine 
    /// @param nCmdShow 
    /// @return 
    Instance;
    hPrevInstance;
    lpCmdLine;
    nCmdShow;

    MSG msg;                                // Windows Message Structure
    bool32 done=FALSE;                         // Bool Variable To Exit Loop

    DWindow.title = "Devendra Engine";
    DWindow.width = 640;
    DWindow.height = 480;
    DWindow.bits = 16;
    
    // Ask The User Which Screen Mode They Prefer
    if 
    (
        MessageBox
            (
                NULL,
                "Would You Like To Run In Fullscreen Mode?", 
                "Start FullScrseen?",
                MB_YESNO|MB_ICONQUESTION
            )   ==  IDNO
    )
    {
        DWindow.fullscreen = false;                       // Windowed Mode
    }

    // Create Our OpenGL Window
    if (!CreateGLWindow(&DWindow, (WNDPROC) WndProc))
    {
        return 0;                           // Quit If Window Was Not Created
    }

    // Init the opengl functions
    InitGLFunctions(DWindow.hDC, DWindow.hRC);

    // Supported OpenGL extension
    PrintSupportedOpenGLExtensions();

    // TODO(supriyo): Pull out the vertex buffer object related code from here!!
    //  Vertex data
    real32 vertices[] = {
        // positions         // colors              // Tex coords
         0.5f,  -0.5f,  0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,  // lower-right corner
        -0.5f,  -0.5f,  0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,  // lower-left corner  
         0.5f,   0.5f,  0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,   // top-left corner
        -0.5f,   0.5f,  0.0f,   0.0f, 1.0f, 1.0f,   0.0f, 1.0f    // top-right corner
    }; 
    uint32 indices[] = {  // note that we start from 0!
        0, 1, 2,
        2, 3, 1
    };
   
    uint32 texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int32 width, height, nrChannels;
    uint8 *data = stbi_load("E:\\personal project\\Devendra-Engine\\engine\\misc\\textures\\wall.jpg", &width, &height, &nrChannels, 0);
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        MessageBox
            (
                NULL,
                "Failed to load the texture format!", 
                "Error occured!",
                MB_OK|MB_ICONERROR
            );
    }
    stbi_image_free(data);

    // real32 color = {1.0f, 0.0f, 0.0f, 1.0f};
    // glTexParameterfv();

    uint32 verticesCount = (uint32)(sizeof(vertices)/sizeof(vertices[0]));
    uint32 indiciesCount = (uint32)(sizeof(indices)/sizeof(indices[0]));  
    
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(real32), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(real32), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    // Texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(real32), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);  

    // Element Buffer Object
    uint32 EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);   

    glVertexAttribPointer(0, verticesCount, GL_FLOAT, GL_FALSE, verticesCount * sizeof(real32), (void*)0);
    glEnableVertexAttribArray(0);

    Devendra_Shader simple_shader;
    simple_shader = {};
    CompileFragmentShader(&simple_shader, "E:\\personal project\\Devendra-Engine\\engine\\misc\\shader\\defaultfragment.glsl");
    CompileVertexShader(&simple_shader, "E:\\personal project\\Devendra-Engine\\engine\\misc\\shader\\defaultvertex.glsl");
    CompileShaderProgram(&simple_shader);
    useShader(&simple_shader);

    // Vsync
    // 0 - off, 1 - on, -1 - adaptive vsync
    wglSwapIntervalEXT(-1);

    LARGE_INTEGER LastCounter;
	QueryPerformanceCounter(&LastCounter);
	uint64 LastCycleCount = __rdtsc();
    while(!done)                                // Loop That Runs Until done=TRUE
    {   
        LARGE_INTEGER PerformanceCountFrequencyResult;
	    QueryPerformanceFrequency(&PerformanceCountFrequencyResult);
	    int64 PerformanceCountFrequency = PerformanceCountFrequencyResult.QuadPart;

        while (PeekMessage(&msg,NULL,0,0,PM_REMOVE))           // Is There A Message Waiting?
        {
            if (msg.message==WM_QUIT)               // Have We Received A Quit Message?
            {
                done=TRUE;                    // If So done=TRUE
            }
            TranslateMessage(&msg);             // Translate The Message
            DispatchMessage(&msg);
        }
        // Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
        if (keys[VK_ESCAPE])                
        {
            done=TRUE;              
        }

        if(keys[VK_KEY_W])
        {
            DWindow.wireframe = !DWindow.wireframe;
        }

        if (keys[VK_F1])                    
        {
            keys[VK_F1]=FALSE;              
            KillGLWindow(&DWindow);                 
            DWindow.fullscreen=!DWindow.fullscreen;             
            // Recreate Our OpenGL Window
            if (!CreateGLWindow(&DWindow, (WNDPROC) WndProc))
            {
                return 0;              
            }
        }

        //////////// Drawing to the Screen /////////////////
        if(DWindow.wireframe)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        useShader(&simple_shader);
        setUniform1f(&simple_shader, "time", counter);
        glBindTexture(GL_TEXTURE_2D, texture);
        DrawGLScene(VAO, indiciesCount);              
        SwapBuffers(DWindow.hDC);
        ////////////////////////////////////////////////////
        
        uint64 EndCycleCount = __rdtsc();
        LARGE_INTEGER EndCounter;
		QueryPerformanceCounter(&EndCounter);

		// TODO: Display the value here
		uint64 CycleElapsed = EndCycleCount - LastCycleCount;
		int64 CounterElapsed = EndCounter.QuadPart - LastCounter.QuadPart;
		real32 MSPerFrame = ((1000.0f * (real32)CounterElapsed) / (real32)PerformanceCountFrequency);
		real32 FPS = (real32)PerformanceCountFrequency / (real32) CounterElapsed;
		real32 MegaCycles = (real32)(CycleElapsed / (1000.0f * 1000.0f));
        char* vsync = "OFF";
        if(wglGetSwapIntervalEXT())
        {
            vsync = "ON";
        }

		char Buffer[250];
		// TODO: Optimize this wsprintf or replace it with something else
		sprintf_s(Buffer, 250, "Miliseconds/Frame : %fms & FPS: %f Mega CyclesElapsed: %fmc & Vsync=%s\n", MSPerFrame, FPS, MegaCycles, vsync);
		OutputDebugStringA(Buffer);

		LastCounter = EndCounter;
		LastCycleCount = EndCycleCount;

        counter += 0.05f;
    }
    // Shutdown
    KillGLWindow(&DWindow);                                 
    return (int32)(msg.wParam);                            
}