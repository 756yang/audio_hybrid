@echo off
rem 使用32位或64位编译
rem set Path=%Path:mingw64\bin=mingw32\bin%
rem set Path=%Path:mingw730_64\bin=mingw730_32\bin%
cd .\Build
cmake -G "MinGW Makefiles" .. > TriffleInfo
mingw32-make
pause