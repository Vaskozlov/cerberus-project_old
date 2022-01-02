@echo off

set tempPath=".\tmp"

if not exist "%tempPath%\tmp"      mkdir "%tempPath%\tmp"

cd "%tempPath%"
set extra_path=\vcpkg\scripts\buildsystems\vcpkg.cmake
set CMAKE_TOOLCHAIN_FILE=%cd%%extra_path%
set cmake_dir=C:\"Program Files"\CMake\bin

if not exist ".\cmake"      mkdir ".\cmake"

powershell -Command "(New-Object System.Net.WebClient).DownloadFile('https://github.com/Kitware/CMake/releases/download/v3.22.0-rc3/cmake-3.22.0-rc3-windows-x86_64.zip', '.\cmake\cmake.zip')"
powershell -Command "Expand-Archive -LiteralPath .\cmake\cmake.zip -DestinationPath .\cmake"

powershell -Command "git clone https://github.com/microsoft/vcpkg"
powershell -Command ".\vcpkg\bootstrap-vcpkg.bat"
powershell -Command ".\vcpkg\vcpkg install fmt:x64-windows"

cd ..\..\..\

mkdir build

powershell -Command "%cmake_dir%\cmake.exe\" -DCMAKE_BUILD_TYPE=Release -G 'Visual Studio 16 2019' -A x64 -S .. -DCMAKE_TOOLCHAIN_FILE=%CMAKE_TOOLCHAIN_FILE%"

echo "SUCCESS! Installation is completed"

echo %CMAKE_TOOLCHAIN_FILE%

