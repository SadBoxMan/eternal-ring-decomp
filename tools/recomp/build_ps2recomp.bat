@echo off
rem ER-008: native MSVC + Ninja build of PS2Recomp (Studio off), 4 jobs
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" >nul || exit /b 1
set CM=C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake
set PATH=%CM%\CMake\bin;%CM%\Ninja;%PATH%
cd /d C:\rs\recomp\PS2Recomp
cmake -S . -B out\rel -G Ninja -DCMAKE_BUILD_TYPE=Release -DPS2X_BUILD_STUDIO=OFF || exit /b 2
cmake --build out\rel -j 4 || exit /b 3
echo BUILD_OK
