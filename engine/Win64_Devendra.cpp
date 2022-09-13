#include <windows.h>                              // Header File For Windows
#include <gl\gl.h>                                // Header File For The OpenGL32 Library
#include <gl\glu.h>                               // Header File For The GLu32 Library
#include "libs\glext.h"                             // OpenGL 1.2 and above compatibility profile and extension interfaces
#include "libs\wglext.h"                            // WGL extension interfaces.
#include <stdio.h>

#include "include/Win32_GL.h"
#include "src/Win32_GL.cpp"

HGLRC           hRC=NULL;                           // Permanent Rendering Context
HDC             hDC=NULL;                           // Private GDI Device Context
HWND            hWnd=NULL;                          // Holds Our Window Handle
HINSTANCE       hInstance;                          // Holds The Instance Of The Application

bool    keys[256];                                  // Array Used For The Keyboard Routine
bool    active=TRUE;                                // Window Active Flag Set To TRUE By Default
bool    fullscreen=TRUE;                            // Fullscreen Flag Set To Fullscreen Mode By Default
	
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);               // Declaration For WndProc

GLvoid KillGLWindow(GLvoid)                         // Properly Kill The Window
{
    /*
        NOTE(supriyo): We should destroy the Window before disabling fullscreen mode, 
        but on some video cards if we destroy the Window BEFORE we disable fullscreen mode, 
        the desktop will become corrupt. 
        So we'll disable fullscreen mode first. 
        This will prevent the desktop from becoming corrupt, 
        and works well on both Nvidia and 3dfx video cards! 
    */
    if (fullscreen)                             // Are We In Fullscreen Mode?
    {
        ChangeDisplaySettings(NULL,0);                  // If So Switch Back To The Desktop
        ShowCursor(TRUE);                               // Show Mouse Pointer
    }
    
    if (hRC)                                // Do We Have A Rendering Context?
    {
        if (!wglMakeCurrent(NULL,NULL))                 // Are We Able To Release The DC And RC Contexts?
        {
            MessageBox(
                NULL,
                "Release Of DC And RC Failed.",
                "SHUTDOWN ERROR",
                MB_OK | MB_ICONINFORMATION
            );
        }
        if (!wglDeleteContext(hRC))                 // Are We Able To Delete The RC?
        {
              MessageBox(
                NULL,
                "Release Rendering Context Failed.",
                "SHUTDOWN ERROR",
                MB_OK | MB_ICONINFORMATION
            );
        }
        hRC=NULL;                           // Set RC To NULL
    }
    if (hDC && !ReleaseDC(hWnd,hDC))                    // Are We Able To Release The DC
    {
        MessageBox(
            NULL,
            "Release Device Context Failed.",
            "SHUTDOWN ERROR",
            MB_OK | MB_ICONINFORMATION
        );
        hDC=NULL;                           // Set DC To NULL
    }
    if (hWnd && !DestroyWindow(hWnd))                   // Are We Able To Destroy The Window?
    {
        MessageBox(
            NULL,
            "Could Not Release hWnd.",
            "SHUTDOWN ERROR",
            MB_OK | MB_ICONINFORMATION
        );
        hWnd=NULL;                          // Set hWnd To NULL
    }
    if (!UnregisterClass("OpenGL",hInstance))               // Are We Able To Unregister Class
    {
        MessageBox(
            NULL,
            "Could Not Unregister Class.",
            "SHUTDOWN ERROR",
            MB_OK | MB_ICONINFORMATION
        );
        hInstance=NULL;                         // Set hInstance To NULL
    }
}

BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
    GLuint      PixelFormat;                        // Holds The Results After Searching For A Match
    WNDCLASS    wc;                         // Windows Class Structure
    
    DWORD       dwExStyle;                      // Window Extended Style
    DWORD       dwStyle;                        // Window Style
    
    RECT WindowRect;                            // Grabs Rectangle Upper Left / Lower Right Values
    WindowRect.left=(long)0;                        // Set Left Value To 0
    WindowRect.right=(long)width;                       // Set Right Value To Requested Width
    WindowRect.top=(long)0;                         // Set Top Value To 0
    WindowRect.bottom=(long)height;                     // Set Bottom Value To Requested Height

    fullscreen=fullscreenflag;                      // Set The Global Fullscreen Flag

    hInstance       = GetModuleHandle(NULL);            // Grab An Instance For Our Window
    wc.style        = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;       // Redraw On Move, And Own DC For Window
    wc.lpfnWndProc      = (WNDPROC) WndProc;                // WndProc Handles Messages
    wc.cbClsExtra       = 0;                        // No Extra Window Data
    wc.cbWndExtra       = 0;                        // No Extra Window Data
    wc.hInstance        = hInstance;                    // Set The Instance
    wc.hIcon        = LoadIcon(NULL, IDI_WINLOGO);          // Load The Default Icon
    wc.hCursor      = LoadCursor(NULL, IDC_ARROW);          // Load The Arrow Pointer
    wc.hbrBackground    = NULL;                     // No Background Required For GL
    wc.lpszMenuName     = NULL;                     // We Don't Want A Menu
    wc.lpszClassName    = "Devendra Engine";      // Set The Class Name

    if (!RegisterClass(&wc))                        // Attempt To Register The Window Class
    {
        MessageBox(
            NULL,
            "Failed To Register The Window Class.",
            "ERROR",
            MB_OK|MB_ICONEXCLAMATION
        );
        return FALSE;                           // Exit And Return FALSE
    }

    if (fullscreen)                             // Attempt Fullscreen Mode?
    {
        DEVMODE dmScreenSettings;                   // Device Mode
        memset(&dmScreenSettings,0,sizeof(dmScreenSettings));       // Makes Sure Memory's Cleared
        dmScreenSettings.dmSize=sizeof(dmScreenSettings);       // Size Of The Devmode Structure
        dmScreenSettings.dmPelsWidth    = width;            // Selected Screen Width
        dmScreenSettings.dmPelsHeight   = height;           // Selected Screen Height
        dmScreenSettings.dmBitsPerPel   = bits;             // Selected Bits Per Pixel
        dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

        // Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
        if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
        {
            // If The Mode Fails, Offer Two Options.  Quit Or Run In A Window.
            if (MessageBox
                    (
                        NULL,
                        "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?",
                        "NeHe GL",
                        MB_YESNO|MB_ICONEXCLAMATION
                    )==IDYES
                )

            {
                fullscreen=FALSE;               // Select Windowed Mode (Fullscreen=FALSE)
            }
            else
            {
                // Pop Up A Message Box Letting User Know The Program Is Closing.
                MessageBox(
                    NULL,
                    "Program Will Now Close.",
                    "ERROR",
                    MB_OK|MB_ICONSTOP
                );
                return FALSE;                   // Exit And Return FALSE
            }
        }
    }

    if (fullscreen)                             // Are We Still In Fullscreen Mode?
    {
        dwExStyle=WS_EX_APPWINDOW;                  // Window Extended Style
        dwStyle=WS_POPUP;                       // Windows Style
        ShowCursor(FALSE);                      // Hide Mouse Pointer
    }
    else
    {
        dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;           // Window Extended Style
        dwStyle=WS_OVERLAPPEDWINDOW;                    // Windows Style
    }
    
    AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);     // Adjust Window To True Requested Size

    if (!(hWnd=CreateWindowEx(  dwExStyle,              // Extended Style For The Window
                "Devendra Engine",               // Class Name
                title,                  // Window Title
                WS_CLIPSIBLINGS |           // Required Window Style
                WS_CLIPCHILDREN |           // Required Window Style
                dwStyle,                // Selected Window Style
                0, 0,                   // Window Position
                WindowRect.right-WindowRect.left,   // Calculate Adjusted Window Width
                WindowRect.bottom-WindowRect.top,   // Calculate Adjusted Window Height
                NULL,                   // No Parent Window
                NULL,                   // No Menu
                hInstance,              // Instance
                NULL)))                 // Don't Pass Anything To WM_CREATE
    {
        KillGLWindow();                         // Reset The Display
        MessageBox(
            NULL,
            "Window Creation Error.",
            "ERROR",
            MB_OK|MB_ICONEXCLAMATION
        );
        return FALSE;                           // Return FALSE
    }

    static  PIXELFORMATDESCRIPTOR pfd=                  // pfd Tells Windows How We Want Things To Be
    {
        sizeof(PIXELFORMATDESCRIPTOR),                  // Size Of This Pixel Format Descriptor
        1,                              // Version Number
        PFD_DRAW_TO_WINDOW |                        // Format Must Support Window
        PFD_SUPPORT_OPENGL |                        // Format Must Support OpenGL
        PFD_DOUBLEBUFFER,                       // Must Support Double Buffering
        PFD_TYPE_RGBA,                          // Request An RGBA Format
        32,                               // Select Our Color Depth
        0, 0, 0, 0, 0, 0,                       // Color Bits Ignored
        0,                              // No Alpha Buffer
        0,                              // Shift Bit Ignored
        0,                              // No Accumulation Buffer
        0, 0, 0, 0,                         // Accumulation Bits Ignored
        24,                   // Number of bits for the depthbuffer
	    8,                    // Number of bits for the stencilbuffer
        0,                              // No Auxiliary Buffer
        PFD_MAIN_PLANE,                         // Main Drawing Layer
        0,                              // Reserved
        0, 0, 0                             // Layer Masks Ignored
    };
    
    /*
        NOTE(supriyo): Info about stencil buffers
        https://computergraphics.stackexchange.com/questions/12/what-is-a-stencil-buffer
    */
    if (!(hDC=GetDC(hWnd)))                         // Did We Get A Device Context?
    {
        KillGLWindow();                         // Reset The Display
        MessageBox(
            NULL,
            "Can't Create A GL Device Context.",
            "ERROR",
            MB_OK|MB_ICONEXCLAMATION
        );
        return FALSE;                           // Return FALSE
    }

    if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))             // Did Windows Find A Matching Pixel Format?
    {
        KillGLWindow();                         // Reset The Display
        MessageBox(
            NULL,
            "Can't Find A Suitable PixelFormat.",
            "ERROR",
            MB_OK|MB_ICONEXCLAMATION
        );
        return FALSE;                           // Return FALSE
    }

    if(!SetPixelFormat(hDC,PixelFormat,&pfd))               // Are We Able To Set The Pixel Format?
    {
        KillGLWindow();                         // Reset The Display
        MessageBox(
            NULL,
            "Can't Set The PixelFormat.",
            "ERROR",
            MB_OK|MB_ICONEXCLAMATION
        );
        return FALSE;                           // Return FALSE
    }

    if (!(hRC=wglCreateContext(hDC)))                   // Are We Able To Get A Rendering Context?
    {
        KillGLWindow();                         // Reset The Display
        MessageBox(
            NULL,
            "Can't Create A GL Rendering Context.",
            "ERROR",
            MB_OK|MB_ICONEXCLAMATION
        );
        return FALSE;                           // Return FALSE
    }

    if(!wglMakeCurrent(hDC,hRC))                        // Try To Activate The Rendering Context
    {
        KillGLWindow();                         // Reset The Display
        MessageBox(
            NULL,
            "Can't Activate The GL Rendering Context.",
            "ERROR",
            MB_OK|MB_ICONEXCLAMATION
        );
        return FALSE;                           // Return FALSE
    }

    ShowWindow(hWnd,SW_SHOW);                       // Show The Window
    SetForegroundWindow(hWnd);                      // Slightly Higher Priority
    SetFocus(hWnd);                             // Sets Keyboard Focus To The Window
    ReSizeGLScene(width, height);                       // Set Up Our Perspective GL Screen
    InitGLFunctions(hDC, hRC);

    if (!InitGL())                              // Initialize Our Newly Created GL Window
    {
        KillGLWindow();                         // Reset The Display
        MessageBox(
            NULL,
            "Initialization Failed.",
            "ERROR",
            MB_OK|MB_ICONEXCLAMATION
        );
        return FALSE;                           // Return FALSE
    }
    
    return TRUE;                                // Success
}

LRESULT CALLBACK WndProc(   
                HWND    wHandle,                   // Handle For This Window
                UINT    uMsg,                   // Message For This Window
                WPARAM  wParam,                 // Additional Message Information
                LPARAM  lParam)                 // Additional Message Information
{
    switch (uMsg)                               // Check For Windows Messages
    {
        case WM_ACTIVATE:                       // Watch For Window Activate Message
        {
            if (!HIWORD(wParam))                    // Check Minimization State
            {
                active=TRUE;                    // Program Is Active
            }
            else
            {
                active=FALSE;                   // Program Is No Longer Active
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

int WINAPI WinMain(HINSTANCE   Instance,              // Instance
            HINSTANCE   hPrevInstance,              // Previous Instance
            LPSTR       lpCmdLine,              // Command Line Parameters
            int     nCmdShow)               // Window Show State
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
    BOOL done=FALSE;                         // Bool Variable To Exit Loop
    
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
        fullscreen=FALSE;                       // Windowed Mode
    }

    // Create Our OpenGL Window
    if (!CreateGLWindow("Devendra Engine",640,480,16,fullscreen))
    {
        return 0;                           // Quit If Window Was Not Created
    }

    // Supported OpenGL extension
    PrintSupportedOpenGLExtensions();

    // TODO(supriyo): Pull out the vertex buffer object related code from here!!
    //  Vertex data
    float vertices[] = {                                           
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };

    // Vertex Buffer Object
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // compile shaders and get the shader program
    GLint shaderProgram = CompileShaders();
    glUseProgram(shaderProgram);

    while(!done)                                // Loop That Runs Until done=TRUE
    {
        if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))           // Is There A Message Waiting?
        {
            if (msg.message==WM_QUIT)               // Have We Received A Quit Message?
            {
                done=TRUE;                  // If So done=TRUE
            }
            else                            // If Not, Deal With Window Messages
            {
                TranslateMessage(&msg);             // Translate The Message
                DispatchMessage(&msg);              // Dispatch The Message
            }
        }
        else                                // If There Are No Messages
        {
            // Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
            if (active)                     
            {
                if (keys[VK_ESCAPE])                
                {
                    done=TRUE;              
                }
                else                        
                {
                    DrawGLScene(&VBO);              
                    SwapBuffers(hDC);           
                }
            }
            if (keys[VK_F1])                    
            {
                keys[VK_F1]=FALSE;              
                KillGLWindow();                 
                fullscreen=!fullscreen;             
                // Recreate Our OpenGL Window
                if (!CreateGLWindow("Devendra Engine",640,480,16,fullscreen))
                {
                    return 0;              
                }
            }
        }
    }
    // Shutdown
    KillGLWindow();                                 
    return (int)(msg.wParam);                            
}