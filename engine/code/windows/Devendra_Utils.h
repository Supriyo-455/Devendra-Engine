#pragma once

#include <stdio.h>
#include <malloc.h>
#include "defines.h"

#ifdef __DEVENDRA_UTILS_EXPORTS__
#define DEVENDRA_UTILS_API __declspec(dllexport)
#else
#define DEVENDRA_UTILS_API __declspec(dllimport)
#endif

DEVENDRA_UTILS_API const char* readFile(const char *filePath);

#ifdef _WIN32
#include <Windows.h>
#endif
#ifdef __unix__
#include <sys/time.h>
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

DEVENDRA_UTILS_API void Devendra_Benchmark_Start(Devendra_Benchmark* benchmark);

DEVENDRA_UTILS_API void Devendra_Benchmark_End(Devendra_Benchmark* benchmark);