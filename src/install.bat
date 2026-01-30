@echo off
setlocal enabledelayedexpansion

echo ========================================
echo CityFlow Library Installation (Windows)
echo ========================================
echo.

REM Default install location and build type
set "DEFAULT_INSTALL_PATH=%USERPROFILE%\local\cityflowcpp"
set "DEFAULT_BUILD_TYPE=Release"

set "INSTALL_PATH=%~1"
set "BUILD_TYPE=%~2"

if "%INSTALL_PATH%"=="" (
    set "INSTALL_PATH=%DEFAULT_INSTALL_PATH%"
)

if "%BUILD_TYPE%"=="" (
    set "BUILD_TYPE=%DEFAULT_BUILD_TYPE%"
)

REM Validate build type
if /i "%BUILD_TYPE%"=="Release" goto :valid_build_type
if /i "%BUILD_TYPE%"=="Debug" goto :valid_build_type
if /i "%BUILD_TYPE%"=="RelWithDebInfo" goto :valid_build_type
if /i "%BUILD_TYPE%"=="MinSizeRel" goto :valid_build_type

echo Error: Invalid build type '%BUILD_TYPE%'
echo Valid options: Release, Debug, RelWithDebInfo, MinSizeRel
pause
exit /b 1

:valid_build_type
echo Install location: !INSTALL_PATH!
echo Build type: !BUILD_TYPE!
echo.
set /p CONFIRM="Continue with installation? (Y/N): "
if /i not "!CONFIRM!"=="Y" (
    echo Installation cancelled.
    exit /b 0
)

echo.
echo Creating build directory...
if exist build (
    echo Cleaning old build directory...
    rmdir /s /q build
)
mkdir build
cd build

echo.
echo Configuring CMake...
cmake -DCMAKE_INSTALL_PREFIX="%INSTALL_PATH%" -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -G "Visual Studio 17 2022" ..
if errorlevel 1 (
    echo ERROR: CMake configuration failed!
    echo.
    echo Make sure you have:
    echo - CMake installed
    echo - Visual Studio 2022 or change the generator above
    pause
    exit /b 1
)

echo.
echo Building CityFlow (%BUILD_TYPE%)...
cmake --build . --config %BUILD_TYPE%
if errorlevel 1 (
    echo ERROR: Build failed!
    pause
    exit /b 1
)

echo.
echo Installing CityFlow...
cmake --install . --config %BUILD_TYPE%
if errorlevel 1 (
    echo ERROR: Installation failed!
    pause
    exit /b 1
)

echo.
echo ========================================
echo Installation completed successfully!
echo ========================================
echo.
echo CityFlow installed to: !INSTALL_PATH!
echo Build type: !BUILD_TYPE!
echo.
echo.
pause