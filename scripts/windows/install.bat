@echo off

set tempPath=".\tmp"

if not exist "%tempPath%\tmp"      mkdir "%tempPath%\tmp"

cd "%tempPath%"
set extra_path=\vcpkg\scripts\buildsystems\vcpkg.cmake
set CMAKE_TOOLCHAIN_FILE=%cd%%extra_path%
set cmake_dir=C:\\\"Program Files\"\CMake\bin

powershell -Command "git clone https://github.com/microsoft/vcpkg"
powershell -Command ".\vcpkg\bootstrap-vcpkg.bat"
powershell -Command ".\vcpkg\vcpkg install fmt:x64-windows"
powershell -Command ".\vcpkg\vcpkg install boost:x64-windows"

cd ..\..\..

mkdir build

cd build
powershell -Command "%cmake_dir%\cmake.exe -DCMAKE_TOOLCHAIN_FILE=%CMAKE_TOOLCHAIN_FILE% -DUSE_DYNAMIC_LIBRARY=OFF -DCMAKE_BUILD_TYPE=Release -DUSE_SANITIZERS=OFF -G 'Visual Studio 17 2022' -A x64 -S .."
cd ..

echo "SUCCESS! Installation is completed"

echo %CMAKE_TOOLCHAIN_FILE%

