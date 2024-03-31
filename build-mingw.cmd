@echo off
setlocal

rem Detect a CMake
where /q cmake
if not %errorlevel% equ 0 for /f "tokens=2*" %%i in ('reg query HKLM\SOFTWARE\Kitware\CMake /v InstallDir ^| findstr REG_SZ') do if exist "%%~j" set "PATH=%PATH%;%%~j\bin"
where /q cmake
if not %errorlevel% equ 0 echo CMake not found! && exit /b %errorlevel%

rem Detect GCC
set "PATH=%PATH%;C:\Qt\Tools\llvm-mingw1706_64\bin"
where /q g++
if not %errorlevel% equ 0 echo GCC not found! && exit /b %errorlevel%

rd /s /q build 2>nul
md build
cd build

rem Generate a project
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
if not %errorlevel% equ 0 exit /b %errorlevel%

rem Build a project
cmake --build . --config Release --target package
if not %errorlevel% equ 0 exit /b %errorlevel%