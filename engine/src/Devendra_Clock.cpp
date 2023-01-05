#include "./include/Devendra_Clock.h"

void Devendra_Clock_Init(Devendra_Clock* clock)
{
    clock->frequency = __rdtsc();
    clock->start = 0;
    clock->end = 0;
    clock->delta = 0;
    clock->deltaSeconds = 0;
    clock->elapsed = 0;
}

void Devendra_Clock_Start(Devendra_Clock* clock)
{
    clock->start = __rdtsc();
}

void Devendra_Clock_Stop(Devendra_Clock* clock)
{
    clock->end = __rdtsc();
    clock->delta = clock->end - clock->start;
    clock->deltaSeconds = (real32)clock->delta / (real32)clock->frequency;
    clock->frequency = __rdtsc();
}

void Devendra_Clock_Update(Devendra_Clock* clock)
{
    clock->elapsed += clock->delta;
}