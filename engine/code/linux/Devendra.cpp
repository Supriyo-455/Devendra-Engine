#include <DevendraConfig.h>

#include <GLFW/glfw3.h>
#include <iostream>

#include "Devendra_Math.h"
#include "logger/DLogger.h"

int main(int argc, char *argv[])
{
    Devendra::log(Devendra::INFO, "Hello from Devendra!");
    return 0;
}