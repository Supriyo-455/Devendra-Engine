#pragma once

#include "..\common\defines.h"

#ifdef __DEVENDRA_BENCHMARK__
    #define DEVENDRA_BENCHMARK   __declspec(dllexport)
#else
   #define DEVENDRA_BENCHMARK    __declspec(dllimport)
#endif


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

DEVENDRA_BENCHMARK void Devendra_Benchmark_Start(Devendra_Benchmark* benchmark);

DEVENDRA_BENCHMARK void Devendra_Benchmark_End(Devendra_Benchmark* benchmark);