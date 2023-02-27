#pragma once

/*
    TODO(supriyo):
        1) Dead Zones for XInput controllers,
        2) Event Detection,
        3) Gesture Detection,
        4) Management of multiple HIDs, for multiple Players
        5) Multiple User Input devices support,
        6) Controller Input remapping
        7) Context-sensitive inputs
        8) Ability to temporarily disable certain inputs
*/

#include "defines.h"

struct MOUSE
{
    int64 dx, dy;
    int32 mouseData;
    int32 dwFlags;
};

struct dInput
{
    uint8 type;
    union
    {
        MOUSE    m;
        //KEYBOARD k;
        //HARDWARE h;
    };
};