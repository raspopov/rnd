@echo off
setlocal

rem Detect a CMake
where /q cmake
if not %errorlevel% equ 0 for /f "tokens=2*" %%i in ('reg query HKLM\SOFTWARE\Kitware\CMake /v InstallDir ^| findstr REG_SZ') do if exist "%%~j" set "PATH=%PATH%;%%~j\bin"
where /q cmake
if not %errorlevel% equ 0 echo CMake not found! && exit /b %errorlevel%

rem Detect MSVC
call "%ProgramFiles%\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
where /q msbuild
if not %errorlevel% equ 0 echo MSVC not found! && exit /b %errorlevel%

rd /s /q build 2>nul
md build
cd build

rem Generate a project
cmake .. -G "Visual Studio 17 2022" -A x64
if not %errorlevel% equ 0 exit /b %errorlevel%

rem Build a project
cmake --build . --config Release
if not %errorlevel% equ 0 exit /b %errorlevel%
