#include "../include/Devendra_Utils.h"

#include <stdio.h>
#include <malloc.h>
#include "../include/defines.h"

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