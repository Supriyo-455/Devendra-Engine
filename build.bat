@echo off

rem Ctime utility for benchmarking

pushd tools
cl -nologo -Oi ctime.c /link winmm.lib /out:ctime.exe 
popd

tools\ctime.exe -begin ctime.ctm

echo Building exeutables for Devendra....
@echo:
REM To find the executables go to Devendra-Engine > Build

set CommonCompilerFlags=-nologo -Oi -W4 -FC -Zi -WX -wd4706 -wd4701 -wd4273 -wd4201 -wd4100 -wd4305
set CommonLinkerFlags=OpenGL32.lib GLu32.lib Gdi32.lib User32.lib

IF not exist build mkdir build

pushd build

set src=..\engine\code

rem compiling commons
set file=%src%\Devendra_Utils.cpp
cl %CommonCompilerFlags% %file% /LD

rem building Devendra executable
set file=%src%\Win32_Devendra.cpp
cl %CommonCompilerFlags% %file% /link %CommonLinkerFlags% Devendra_Utils.lib

@echo:
@echo Executables are built in build folder.
@echo To run the executable type "run" without quotes.

popd

tools\ctime.exe -end ctime.ctm