@echo off

set CommonCompilerFlags=-nologo -Oi -W4 -FC -Zi -WX -wd4706 -wd4701 -wd4273 -wd4201 -wd4100 -wd4305 -wd4530 -wd4996

@REM rem Building tests
@REM if not exist testbin mkdir testbin
@REM pushd testbin
@REM cl %CommonCompilerFlags% ..\engine\tests\DMathTest.cpp


if not exist raytracer mkdir raytracer
pushd raytracer
cl %CommonCompilerFlags% ..\engine\tests\DRayTest.cpp
popd

pushd raytracer
DRayTest.exe
start test.bmp
popd