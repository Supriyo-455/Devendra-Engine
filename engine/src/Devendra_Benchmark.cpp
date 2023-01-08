#include "../include/Devendra_Benchmark.h"

#ifdef _WIN32
#include <profileapi.h>
#endif
#ifdef __unix__
#include <sys/time.h>
#endif


void Devendra_Benchmark_Start(Devendra_Benchmark* benchmark)
{
    LARGE_INTEGER frequency, counter; 
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&counter);

    benchmark->lastCycle = __rdtsc();
    benchmark->lastCounter = counter.QuadPart;
    benchmark->frequency = frequency.QuadPart;
    benchmark->cyclesElapsed = 0;
    benchmark->counterElapsed = 0;
    benchmark->msPerFrame = 0;
    benchmark->fps = 0;
}

void Devendra_Benchmark_End(Devendra_Benchmark* benchmark)
{
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);

    benchmark->cyclesElapsed = __rdtsc() - benchmark->lastCycle;
    benchmark->counterElapsed = counter.QuadPart - benchmark->lastCounter;
    benchmark->msPerFrame = ((real32)benchmark->counterElapsed * 1000.0f) / (real32)benchmark->frequency;
    benchmark->fps = (real32)benchmark->frequency / (real32)benchmark->counterElapsed;
}