@echo off
rem ʹ��32λ��64λ����
rem set Path=%Path:mingw64\bin=mingw32\bin%
rem set Path=%Path:mingw730_64\bin=mingw730_32\bin%
cd .\Build
cmake -G "MinGW Makefiles" .. > TriffleInfo
mingw32-make
pause