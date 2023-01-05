#pragma once

#include "defines.h"

typedef struct Devendra_Clock
{
    uint64 frequency;
    uint64 start;
    uint64 end;
    uint64 elapsed;
    uint64 delta;
    real32 deltaSeconds;
    uint64 cpuCyclesPerSecond;
}Devendra_Clock;


void Devendra_Clock_Init(Devendra_Clock* clock);

void Devendra_Clock_Start(Devendra_Clock* clock);

void Devendra_Clock_Stop(Devendra_Clock* clock);

void Devendra_Clock_Update(Devendra_Clock* clock);