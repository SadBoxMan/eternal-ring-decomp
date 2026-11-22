@echo off
rem ER-008: compile the recompiled game C++ into ps2EntryRunner (RelWithDebInfo, no LTCG), 4 jobs
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" >nul || exit /b 1
set CM=C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake
set PATH=%CM%\CMake\bin;%CM%\Ninja;%PATH%
set D=C:/rs/recomp/PS2Recomp/out/rel/_deps
cd /d C:\rs\recomp\game_build
cmake -S . -B out\game -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo -DPS2X_BUILD_STUDIO=OFF -DPS2X_BUILD_TEST=OFF -DPS2X_BUILD_ANALYZER=OFF -DPS2X_BUILD_RECOMP=OFF -DPS2X_ENABLE_AGRESSIVE_LOGS=OFF -DFETCHCONTENT_SOURCE_DIR_RAYLIB=%D%/raylib-src -DFETCHCONTENT_SOURCE_DIR_IMGUI=%D%/imgui-src -DFETCHCONTENT_SOURCE_DIR_RLIMGUI=%D%/rlimgui-src || exit /b 2
cmake --build out\game -j 4 --target ps2EntryRunner -- -k 0 || exit /b 3
echo BUILD_OK
