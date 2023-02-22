@echo off

rem Get start time:
for /F "tokens=1-4 delims=:.," %%a in ("%time%") do (
   set /A "start=(((%%a*60)+1%%b %% 100)*60+1%%c %% 100)*100+1%%d %% 100"
)

echo Building exeutables for Devendra....
@echo:
REM To find the executables go to Devendra-Engine > Build

set CommonCompilerFlags=-nologo -Oi -W4 -FC -Zi -WX -wd4706 -wd4701 -wd4273 -wd4201 -wd4100 -wd4305 -wd4530
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

rem Get end time:
for /F "tokens=1-4 delims=:.," %%a in ("%time%") do (
   set /A "end=(((%%a*60)+1%%b %% 100)*60+1%%c %% 100)*100+1%%d %% 100"
)

rem Get elapsed time:
set /A elapsed=end-start


rem Show elapsed time:
set /A hh=elapsed/(60*60*100), rest=elapsed%%(60*60*100), mm=rest/(60*100), rest%%=60*100, ss=rest/100, cc=rest%%100
if %mm% lss 10 set mm=0%mm%
if %ss% lss 10 set ss=0%ss%
if %cc% lss 10 set cc=0%cc%
echo Build time %hh%:%mm%:%ss%:%cc%