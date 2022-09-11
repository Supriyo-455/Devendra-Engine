@echo off
TASKKILL /IM devenv.exe -F
devenv ..\build\Win64_Devendra.exe