#include <time.h>

#include "defines.h"
#include "Devendra_Utils.h"
#include "libs/Win32_Keycodes.h"

#define STB_IMAGE_IMPLEMENTATION
#include "libs/stb_image.h"

#include "Devendra_Win32_GL.h"

#include "Devendra_Math.h"

#include "Devendra_User_Input.h"

struct mouse
{
	int16 x, y;
	uint8 buttons;
};

enum { 
    MOUSE_LEFT = 0b1, 
    MOUSE_MIDDLE = 0b10, 
    MOUSE_RIGHT = 0b100, 
    MOUSE_X1 = 0b1000, 
    MOUSE_X2 = 0b10000 
};