@echo off

set CommonCompilerFlags=-Oi -W4 -FC -Zi -WX -wd4706 -wd4701 -wd4273 -wd4201 -wd4100 -wd4305 -wd4530

rem Building tests
if not exist testbin mkdir testbin
pushd testbin
cl %CommonCompilerFlags% ..\engine\tests\DMathTest.cpp
popd