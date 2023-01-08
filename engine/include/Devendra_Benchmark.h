#pragma once

#include "defines.h"

typedef struct Devendra_Benchmark
{
    uint64 lastCycle;
    uint64 lastCounter;
    uint64 frequency;
    uint64 cyclesElapsed;
    uint64 counterElapsed;
    real32 msPerFrame;
    real32 fps;
}Devendra_Benchmark;

void Devendra_Benchmark_Start(Devendra_Benchmark* benchmark);

void Devendra_Benchmark_End(Devendra_Benchmark* benchmark);