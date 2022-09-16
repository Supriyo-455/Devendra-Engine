@echo off

echo Building exeutables for Devendra....
REM To find the executables go to Devendra-Engine > Build

IF not exist build mkdir build
pushd build
cl /Oi /W4 /FC /wd4706 /wd4701 /Zi /WX ..\engine\Win32_Devendra.cpp OpenGL32.lib GLu32.lib Gdi32.lib User32.lib
popd