#include "Devendra_Utils.h"

const char* readFile(const char *filePath)
{
    FILE* shaderFile;
    fopen_s(&shaderFile, filePath, "rb");
    uint32 fileSize = 0;
    char* shaderCode = NULL;

    if(!shaderFile)
    {
        printf("Could not read file! %s. File doesn't exist!", filePath);
        return shaderCode;
    }

    //Getting File Size
    fseek(shaderFile, 0, SEEK_END);
    fileSize = ftell(shaderFile);
    rewind(shaderFile); 

    //Reading From File
    shaderCode = (char*)malloc(sizeof(char) * (fileSize+1));
    fread(shaderCode, sizeof(char), fileSize, shaderFile);
    fclose(shaderFile);
    shaderCode[fileSize] = '\0';
    return shaderCode;
}


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