@echo off

echo Building exeutables for Devendra....
REM To find the executables go to Devendra-Engine > Build

set CommonCompilerFlags=-nologo -Oi -W4 -FC -Zi -WX -wd4706 -wd4701 -wd4273 -wd4201 -wd4100 -wd4305
set CommonLinkerFlags=OpenGL32.lib GLu32.lib Gdi32.lib User32.lib

IF not exist build mkdir build

pushd build
cl %CommonCompilerFlags% ..\engine\src\Devendra_Utils.cpp /LD
cl %CommonCompilerFlags% ..\engine\src\Devendra_Win32_GL_EXT.cpp /LD /link %CommonLinkerFlags%
cl %CommonCompilerFlags% ..\engine\src\Devendra_Win32_GL.cpp /LD /link %CommonLinkerFlags% Devendra_Utils.lib Devendra_Win32_GL_EXT.lib
cl %CommonCompilerFlags% ..\engine\Win32_Devendra.cpp /link %CommonLinkerFlags% Devendra_Utils.lib Devendra_Win32_GL_EXT.lib Devendra_Win32_GL.lib
popd