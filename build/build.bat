@echo off
cl -Oi -W4 -FC /wd4706 /wd4701 ..\engine\Win64_Devendra.cpp /Zi /WX OpenGL32.lib GLu32.lib Gdi32.lib User32.lib