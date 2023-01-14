#include <time.h>

#include "include/defines.h"
#include "include/Devendra_Win32_GL_EXT.h"
#include "libs/Win32_keycodes.h"
#include "include/Devendra_Utils.h"
#include "include/Devendra_Win32_GL.h"

#define STB_IMAGE_IMPLEMENTATION
#include "libs/stb_image.h"

#include "include/Devendra_Math.h"

#include "include/Devendra_Renderer.h"
#include "src/Devendra_Renderer.cpp"

#include "include/Devendra_Benchmark.h"
#include "src/Devendra_Benchmark.cpp"

Devendra_Window DWindow = {};
Devendra_Benchmark DBenchmark = {};

bool32    keys[256];                                  // Array Used For The Keyboard Routine
typedef struct mouse{
	int16 x, y;
	uint8 buttons;
} mouse;
enum { MOUSE_LEFT = 0b1, MOUSE_MIDDLE = 0b10, MOUSE_RIGHT = 0b100, MOUSE_X1 = 0b1000, MOUSE_X2 = 0b10000 };

// TODO: Create seperate API for user input handle
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
            DWindow.wireframe = ~DWindow.wireframe;
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
            return 0;                               // Jump Back
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

int WINAPI WinMain(HINSTANCE   Instance,              // Instance
                    HINSTANCE   hPrevInstance,              // Previous Instance
                    LPSTR       lpCmdLine,              // Command Line Parameters
                    int         nCmdShow)               // Window Show State
{
    /// @brief  TODO: Use this in future?
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
    float counter = 0.0f;

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

    
    // Supported OpenGL extension
    PrintSupportedOpenGLExtensions();
    InitGLFunctions(DWindow.hDC, DWindow.hRC);

    // TODO: Pull out the vertex buffer object related code from here!!
    //  Vertex data
    real32 vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };
    uint32 indices[] = {  // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    uint32 vertexCount = sizeof(vertices) / sizeof(vertices[0]);
    uint32 indexCount = sizeof(indices) / sizeof(indices[0]);
    uint32 stride = 8 * sizeof(real32);

    Devendra_Renderer renderer = {};
    
    Devendra_Shader simple_shader = {};
    simple_shader.fragmentShaderPath = "E:\\personal project\\Devendra-Engine\\engine\\misc\\shader\\defaultfragment.glsl";
    simple_shader.vertexShaderPath = "E:\\personal project\\Devendra-Engine\\engine\\misc\\shader\\defaultvertex.glsl";

    RendererInit(&renderer, &DWindow);

    // Texture 1
    Devendra_Texture texture1 = {};
    texture1.texPath = "E:\\personal project\\Devendra-Engine\\engine\\misc\\textures\\wall.jpg";
    texture1.format = GL_RGB;
    if(LoadTexture(&texture1))
    {
        MessageBox
            (
                NULL,
                "Failed to load the texture format!", 
                "Error occured!",
                MB_OK|MB_ICONERROR
            );
    }

    // Texture 2
    Devendra_Texture texture2 = {};
    texture2.texPath = "E:\\personal project\\Devendra-Engine\\engine\\misc\\textures\\awesomeface.png";
    texture2.format = GL_RGBA;
    if(LoadTexture(&texture2)) 
    {
       MessageBox
            (
                NULL,
                "Failed to load the texture format!", 
                "Error occured!",
                MB_OK|MB_ICONERROR
            );
    }


    RendererBindBuffers(&renderer, vertices, vertexCount, indices, indexCount, stride);
    if(!RendererLoadShader(&renderer, &simple_shader)) {
        MessageBox
            (
                NULL,
                "Failed to load the shader!", 
                "Error occured!",
                MB_OK|MB_ICONERROR
            );
    }
    RendererUseShader(&renderer);

    // Set the Texture variables locations for fragment shader
    ShaderSetInt(renderer.DShader, "texture1", 0);
    ShaderSetInt(renderer.DShader, "texture2", 1);
    
    mat4x4 trans = {};
    uint32 transformLoc = ShaderGetUniformLocation(renderer.DShader, "transform");

    // Vsync
    // 0 - off, 1 - on, -1 - adaptive vsync
    SetVSync(ADAPTIVE_VSYNC);
    
    while(!done)                                // Loop That Runs Until done=TRUE
    {
        Devendra_Benchmark_Start(&DBenchmark);

        while (PeekMessage(&msg,NULL,0,0,PM_REMOVE))           // Is There A Message Waiting?
        {
            if (msg.message==WM_QUIT)               // Have We Received A Quit Message?
            {
                done=TRUE;                    // If So done=TRUE
            }
            TranslateMessage(&msg);             // Translate The Message
            DispatchMessage(&msg);
        }
        
        //////////////////////////////// Key events ////////////////////////////////////////
        // Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
        if (keys[VK_ESCAPE])                
        {
            done=TRUE;              
        }

        if(keys[VK_KEY_W])
        {
            DWindow.wireframe = ~DWindow.wireframe;
        }

        if (keys[VK_F1])                    
        {
            keys[VK_F1]=FALSE;              
            KillGLWindow(&DWindow);                 
            DWindow.fullscreen = ~DWindow.fullscreen;
            // Recreate Our OpenGL Window
            if (!CreateGLWindow(&DWindow, (WNDPROC) WndProc))
            {
                return 0;              
            }
        }
        ////////////////////////////////////////////////////////////////////////////////////////

        //////////// Drawing to the Screen /////////////////
        ShaderSetFloat(renderer.DShader, "time", absf(sin(counter)));
        
        BindTexture(&texture1, GL_TEXTURE0);
        BindTexture(&texture2, GL_TEXTURE1);


        trans = rotateZ(-4*360.0f*sin(counter));  // Note: For Variable Rotation
        trans = transMat4x4(trans, vec(sin(counter) * 0.5f, 0.0f, 0.0f));   // Note: For translate along X axis back and forth
        trans = transpose(trans);
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &trans.E[0][0]); // NOTE: How to convert mat4x4 to GLFLoat *
        
        
        RendererUseShader(&renderer);
        RendererDraw(&renderer);
        ////////////////////////////////////////////////////

		// TODO: Calculate FPS, MSPerFrame, MegaCycles
        Devendra_Benchmark_End(&DBenchmark);

        char* vsync = "OFF";
        if(wglGetSwapIntervalEXT())
        {
            vsync = "ON";
        }

		// char Buffer[250];
		// // TODO: Optimize this wsprintf or replace it with something else
		// sprintf_s(
        //     Buffer, 
        //     250, 
        //     "Miliseconds/Frame : %fms & FPS: %f Mega CyclesElapsed: %fmc & Vsync=%s\n", 
        //     DBenchmark.msPerFrame, DBenchmark.fps, (real32)(DBenchmark.cyclesElapsed / (1000.0f * 1000.0f)), vsync
        // );
		// OutputDebugStringA(Buffer);

        counter += 0.8f;
    }
    // Shutdown
    KillGLWindow(&DWindow);                               
    return (int32)(msg.wParam);                            
}