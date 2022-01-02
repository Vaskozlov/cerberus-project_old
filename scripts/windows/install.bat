@echo off

set tempPath=".\tmp"

if not exist "%tempPath%\tmp"      mkdir "%tempPath%\tmp"

cd "%tempPath%"
set extra_path=\vcpkg\scripts\buildsystems\vcpkg.cmake
set CMAKE_TOOLCHAIN_FILE=%cd%%extra_path%
set cmake_dir=C:\\\"Program Files\"\CMake\bin


cd ..\..\..

mkdir build

cd build
powershell -Command "%cmake_dir%\cmake.exe -DCMAKE_BUILD_TYPE=Release -G 'Visual Studio 16 2019' -A x64 -S .. -DCMAKE_TOOLCHAIN_FILE=%CMAKE_TOOLCHAIN_FILE%"
cd ..

echo "SUCCESS! Installation is completed"

echo %CMAKE_TOOLCHAIN_FILE%

