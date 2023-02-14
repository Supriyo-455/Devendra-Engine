#include "Devendra_Win32_GL.h"

// TODO: Probably use different translational unit for this 

bool32 CreateGLWindow(Devendra_Window* DWindow)
{
    uint32      PixelFormat;                        // Holds The Results After Searching For A Match
    WNDCLASS    wc;                                 // Windows Class Structure

    DWORD       dwExStyle;                      // Window Extended Style
    DWORD       dwStyle;                        // Window Style

    RECT WindowRect;                            // Grabs Rectangle Upper Left / Lower Right Values
    WindowRect.left = (long)0;                        // Set Left Value To 0
    WindowRect.right = (long)DWindow->width;                       // Set Right Value To Requested Width
    WindowRect.top = (long)0;                         // Set Top Value To 0
    WindowRect.bottom = (long)DWindow->height;                     // Set Bottom Value To Requested Height

    DWindow->hInstance = GetModuleHandle(NULL);            // Grab An Instance For Our Window
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;       // Redraw On Move, And Own DC For Window
    wc.lpfnWndProc = DWindow->WndProc;                // WndProc Handles Messages
    wc.cbClsExtra = 0;                        // No Extra Window Data
    wc.cbWndExtra = 0;                        // No Extra Window Data
    wc.hInstance = DWindow->hInstance;                    // Set The Instance
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);          // Load The Default Icon
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);          // Load The Arrow Pointer
    wc.hbrBackground = NULL;                     // No Background Required For GL
    wc.lpszMenuName = NULL;                     // We Don't Want A Menu
    wc.lpszClassName = "Devendra Engine";      // Set The Class Name

    if (!RegisterClass(&wc))                        // Attempt To Register The Window Class
    {
        MessageBox(
            NULL,
            "Failed To Register The Window Class.",
            "ERROR",
            MB_OK | MB_ICONEXCLAMATION
        );
        return FALSE;                           // Exit And Return FALSE
    }

    if (DWindow->fullscreen)                             // Attempt Fullscreen Mode?
    {
        DEVMODE dmScreenSettings;                                   // Device Mode
        memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));       // Makes Sure Memory's Cleared
        dmScreenSettings.dmSize = sizeof(dmScreenSettings);       // Size Of The Devmode Structure
        dmScreenSettings.dmPelsWidth = DWindow->width;            // Selected Screen Width
        dmScreenSettings.dmPelsHeight = DWindow->height;           // Selected Screen Height
        dmScreenSettings.dmBitsPerPel = DWindow->bits;             // Selected Bits Per Pixel
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        // Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
        if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
        {
            // If The Mode Fails, Offer Two Options.  Quit Or Run In A Window.
            if (MessageBox
            (
                NULL,
                "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?",
                "NeHe GL",
                MB_YESNO | MB_ICONEXCLAMATION
            ) == IDYES
                )

            {
                DWindow->fullscreen = FALSE;               // Select Windowed Mode (Fullscreen=FALSE)
            }
            else
            {
                // Pop Up A Message Box Letting User Know The Program Is Closing.
                MessageBox(
                    NULL,
                    "Program Will Now Close.",
                    "ERROR",
                    MB_OK | MB_ICONSTOP
                );
                return FALSE;                   // Exit And Return FALSE
            }
        }
    }

    if (DWindow->fullscreen)                             // Are We Still In Fullscreen Mode?
    {
        dwExStyle = WS_EX_APPWINDOW;                  // Window Extended Style
        dwStyle = WS_POPUP;                       // Windows Style
        ShowCursor(FALSE);                      // Hide Mouse Pointer
    }
    else
    {
        dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;           // Window Extended Style
        dwStyle = WS_OVERLAPPEDWINDOW;                    // Windows Style
    }

    AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);     // Adjust Window To True Requested Size

    if (!(DWindow->hWnd = CreateWindowEx(
        dwExStyle,                       // Extended Style For The Window
        "Devendra Engine",               // Class Name
        DWindow->title,                  // Window Title
        WS_CLIPSIBLINGS |               // Required Window Style
        WS_CLIPCHILDREN |               // Required Window Style
        dwStyle,                        // Selected Window Style
        0, 0,                           // Window Position
        WindowRect.right - WindowRect.left,   // Calculate Adjusted Window Width
        WindowRect.bottom - WindowRect.top,   // Calculate Adjusted Window Height
        NULL,                   // No Parent Window
        NULL,                   // No Menu
        DWindow->hInstance,              // Instance
        NULL)))                 // Don't Pass Anything To WM_CREATE
    {
        KillGLWindow(DWindow);                         // Reset The Display
        MessageBox(
            NULL,
            "Window Creation Error.",
            "ERROR",
            MB_OK | MB_ICONEXCLAMATION
        );
        return FALSE;                           // Return FALSE
    }

    static  PIXELFORMATDESCRIPTOR pfd =                  // pfd Tells Windows How We Want Things To Be
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
    if (!(DWindow->hDC = GetDC(DWindow->hWnd)))                         // Did We Get A Device Context?
    {
        KillGLWindow(DWindow);                         // Reset The Display
        MessageBox(
            NULL,
            "Can't Create A GL Device Context.",
            "ERROR",
            MB_OK | MB_ICONEXCLAMATION
        );
        return FALSE;                           // Return FALSE
    }

    if (!(PixelFormat = ChoosePixelFormat(DWindow->hDC, &pfd)))             // Did Windows Find A Matching Pixel Format?
    {
        KillGLWindow(DWindow);                         // Reset The Display
        MessageBox(
            NULL,
            "Can't Find A Suitable PixelFormat.",
            "ERROR",
            MB_OK | MB_ICONEXCLAMATION
        );
        return FALSE;                           // Return FALSE
    }

    if (!SetPixelFormat(DWindow->hDC, PixelFormat, &pfd))               // Are We Able To Set The Pixel Format?
    {
        KillGLWindow(DWindow);                         // Reset The Display
        MessageBox(
            NULL,
            "Can't Set The PixelFormat.",
            "ERROR",
            MB_OK | MB_ICONEXCLAMATION
        );
        return FALSE;                           // Return FALSE
    }

    if (!(DWindow->hRC = wglCreateContext(DWindow->hDC)))                   // Are We Able To Get A Rendering Context?
    {
        KillGLWindow(DWindow);                         // Reset The Display
        MessageBox(
            NULL,
            "Can't Create A GL Rendering Context.",
            "ERROR",
            MB_OK | MB_ICONEXCLAMATION
        );
        return FALSE;                           // Return FALSE
    }

    if (!wglMakeCurrent(DWindow->hDC, DWindow->hRC))                        // Try To Activate The Rendering Context
    {
        KillGLWindow(DWindow);                         // Reset The Display
        MessageBox(
            NULL,
            "Can't Activate The GL Rendering Context.",
            "ERROR",
            MB_OK | MB_ICONEXCLAMATION
        );
        return FALSE;                           // Return FALSE
    }

    ShowWindow(DWindow->hWnd, SW_SHOW);                       // Show The Window
    SetForegroundWindow(DWindow->hWnd);                      // Slightly Higher Priority
    SetFocus(DWindow->hWnd);                             // Sets Keyboard Focus To The Window
    ReSizeGLScene(DWindow->width, DWindow->height);                       // Set Up Our Perspective GL Screen
    InitGLFunctions(DWindow->hDC, DWindow->hRC);

    if (!InitGL())                              // Initialize Our Newly Created GL Window
    {
        KillGLWindow(DWindow);                         // Reset The Display
        MessageBox(
            NULL,
            "Initialization Failed.",
            "ERROR",
            MB_OK | MB_ICONEXCLAMATION
        );
        return FALSE;                           // Return FALSE
    }

    return TRUE;                                // Success
}

GLvoid KillGLWindow(Devendra_Window* DWindow)                         // Properly Kill The Window
{
    /*
        NOTE(supriyo): We should destroy the Window before disabling fullscreen mode,
        but on some video cards if we destroy the Window BEFORE we disable fullscreen mode,
        the desktop will become corrupt.
        So we'll disable fullscreen mode first.
        This will prevent the desktop from becoming corrupt,
        and works well on both Nvidia and 3dfx video cards!
    */
    if (DWindow->fullscreen)                             // Are We In Fullscreen Mode?
    {
        ChangeDisplaySettings(NULL, 0);                  // If So Switch Back To The Desktop
        ShowCursor(TRUE);                               // Show Mouse Pointer
    }

    if (DWindow->hRC)                                // Do We Have A Rendering Context?
    {
        if (!wglMakeCurrent(NULL, NULL))                 // Are We Able To Release The DC And RC Contexts?
        {
            MessageBox(
                NULL,
                "Release Of DC And RC Failed.",
                "SHUTDOWN ERROR",
                MB_OK | MB_ICONINFORMATION
            );
        }
        if (!wglDeleteContext(DWindow->hRC))                 // Are We Able To Delete The RC?
        {
            MessageBox(
                NULL,
                "Release Rendering Context Failed.",
                "SHUTDOWN ERROR",
                MB_OK | MB_ICONINFORMATION
            );
        }
        DWindow->hRC = NULL;                           // Set RC To NULL
    }
    if (DWindow->hDC && !ReleaseDC(DWindow->hWnd, DWindow->hDC))                    // Are We Able To Release The DC
    {
        MessageBox(
            NULL,
            "Release Device Context Failed.",
            "SHUTDOWN ERROR",
            MB_OK | MB_ICONINFORMATION
        );
        DWindow->hDC = NULL;                           // Set DC To NULL
    }
    if (DWindow->hWnd && !DestroyWindow(DWindow->hWnd))                   // Are We Able To Destroy The Window?
    {
        MessageBox(
            NULL,
            "Could Not Release hWnd.",
            "SHUTDOWN ERROR",
            MB_OK | MB_ICONINFORMATION
        );
        DWindow->hWnd = NULL;                          // Set hWnd To NULL
    }
    if (!UnregisterClass("Devendra Engine", DWindow->hInstance))               // Are We Able To Unregister Class
    {
        MessageBox(
            NULL,
            "Could Not Unregister Class.",
            "SHUTDOWN ERROR",
            MB_OK | MB_ICONINFORMATION
        );
        DWindow->hInstance = NULL;                         // Set hInstance To NULL
    }
}

void ReSizeGLScene(int width, int height)                 // Resize And Initialize The GL Window
{
    if (height == 0)                              // Prevent A Divide By Zero By
    {
        height = 1;                               // Making Height Equal One
    }

    glViewport(0, 0, width, height);                    // Reset The Current Viewport
    glMatrixMode(GL_PROJECTION);                        // Select The Projection Matrix
    glLoadIdentity();                                   // Reset The Projection Matrix

    // Calculate The Aspect Ratio Of The Window with 45deg viewing angle
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

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

void CompileFragmentShader(Devendra_Shader* shader, const char* fragmentShaderPath)
{
    const char* fragmentShaderSource = readFile(fragmentShaderPath);
    // Compiling fragment shader
    if (fragmentShaderSource == NULL)        // fragment Shader loading from file failed!
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
    if (!success)
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
    if (vertexShaderSource == NULL)      // Vertex Shader loading from file failed!
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
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        char Buffer[512];
        sprintf_s(Buffer, 512, "\nERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s", infoLog);
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
    if (!success) {
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
    shader->active = true;
}

void setUniform1i(Devendra_Shader* shader, const char* name, int32 value)
{
    glUniform1i(glGetUniformLocation(shader->ShaderProgramID, name), value);
}
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