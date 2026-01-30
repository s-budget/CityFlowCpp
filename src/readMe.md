# CityFlow Library - Installation Guide

CityFlow is a C++ traffic simulation library that can be installed and used accross multiple projects.

## Table of Contents
- [Prerequisites](#prerequisites)
- [Quick Installation](#quick-installation)
    - [Windows](#windows)
    - [Linux](#linux)
- [Custom Installation Location](#custom-installation-location)
- [Manual Installation](#manual-installation)
- [Debug vs Release Builds](#Debug-vs-Release-Builds)
- [CMake Generator Options (Windows)](#cmake-generator-options-windows)
- [Using CityFlow in Your Projects](#using-cityflow-in-your-projects)
- [Uninstallation](#uninstallation)
- [Files used in simulations](#files-used-in-simulations)

---

## Prerequisites

### All Platforms
- **CMake** 3.10 or higher ([Download CMake](https://cmake.org/download/))
- **C++ Compiler** with C++11 suport

### Windows
- **Visual Studio 2017** or newer (Community Edition is fine)
    - OR **MinGW-w64** for MinGW builds
- **Git Bash** (optional, for runing `.sh` scripts on Windows)

### Linux
- **GCC** 7+ or **Clang** 5+
- **Make** or **Ninja** build sytem

Install on Ubuntu/Debian:
```bash
sudo apt update
sudo apt install build-essential cmake
```

Install on Fedora/RHEL:
```bash
sudo dnf install gcc-c++ cmake make
```

---

## Quick Installation

### Windows

1. Open **Command Prompt** or **PowerShell**
2. Navigate to the CityFlow source directory:
```cmd
   cd C:\Users\userName\downloads\CityFlowCpp\src
```
3. Run the instalation script:
```cmd
   install.bat
```
4. The library will be installed to: `C:\Users\userName\local\cityflowcpp`

### Linux

1. Open **Terminal**
2. Navigate to the CityFlow source directoy:
```bash
   cd /path/to/CityFlowCpp/src
```
3. Make the script executible (first time only):
```bash
   chmod +x install.sh
```
4. Run the installation script:
```bash
   ./install.sh
```
5. The library will be installed to: `~/local/cityflowcpp`

---

## Custom Installation Location

You can specify a custom installation path as an arguement to the script.

### Windows
```cmd
install.bat C:\my\custom\path
```

### Linux
```bash
./install.sh /my/custom/path
```
## Debug vs Release Builds

### Why Your Debugger Might Not Work

If your IDE debugger won't stop at breakpoints or you're getting linker errors about `RuntimeLibrary` or `_ITERATOR_DEBUG_LEVEL` mismatches, it's because **Debug and Release builds are incompatible**. Your project's build type must match CityFlow's build type.

### Installing Different Build Types

The installation scripts accept two optional arguments:

**Linux/macOS:**
```bash
./install.sh [install_path] [build_type]

# Examples:
./install.sh                                    # Default: ~/local/cityflowcpp (Release)
./install.sh ~/local/cityflowcpp-debug Debug    # Debug build
./install.sh ~/custom/path RelWithDebInfo       # RelWithDebInfo build
```

**Windows:**
```cmd
install.bat [install_path] [build_type]

REM Examples:
install.bat                                                # Default (Release)
install.bat C:\Users\YourName\local\cityflowcpp-debug Debug
```

**Valid build types:** `Release`, `Debug`...

### Using Debug and Release Side-by-Side

Install both versions to different directories and switch in your CMakeLists.txt:

```bash
# Install both versions
./install.sh ~/local/cityflowcpp-release Release
./install.sh ~/local/cityflowcpp-debug Debug
```

* make sure to rebuild your project with updated path in CMakeLists.txt
---

## System-Wide Installation

#### Windows (Admin Requried)
```cmd
install.bat C:\Program Files\cityflowcpp
```

#### Linux (Sudo Required)
```bash
./install.sh /usr/local
```

---

## Manual Installation

If you prefer to install manualy or customize the build process:

### Step 1: Create Build Directory
```bash
cd CityFlowCpp/src
mkdir build
cd build
```

### Step 2: Configure CMake

**Windows (Visual Studio):**
```cmd
cmake -DCMAKE_INSTALL_PREFIX=C:\path\to\install -G "Visual Studio 17 2022" ..
```

**Linux:**
```bash
cmake -DCMAKE_INSTALL_PREFIX=/path/to/install -DCMAKE_BUILD_TYPE=Release ..
```

### Step 3: Build

**Windows:**
```cmd
cmake --build . --config Release
```

**Linux:**
```bash
make -j$(nproc)
```

### Step 4: Install

**Windows:**
```cmd
cmake --install . --config Release
```

**Linux:**
```bash
make install
# Or with sudo for system-wide:
sudo make install
```

---

## CMake Generator Options (Windows)

The default Windows script uses Visual Studio 2022. You can change the generator by editing `install.bat` or specifing it manually.

### Available Generators

**Visual Studio:**
- `"Visual Studio 17 2022"` - Visual Studio 2022 (default in script)
- `"Visual Studio 16 2019"` - Visual Studio 2019
- `"Visual Studio 15 2017"` - Visual Studio 2017

**MinGW:**
- `"MinGW Makefiles"` - For MinGW-w64 compiler

**Ninja:**
- `"Ninja"` - Fast paralel build system

### How to Change Generator

#### Option 1: Edit install.bat

Open `src/install.bat` and find this line:
```batch
cmake -DCMAKE_INSTALL_PREFIX="%INSTALL_PATH%" -G "Visual Studio 17 2022" ..
```

Change to your prefered generator:
```batch
cmake -DCMAKE_INSTALL_PREFIX="%INSTALL_PATH%" -G "Visual Studio 16 2019" ..
```

Or for MinGW:
```batch
cmake -DCMAKE_INSTALL_PREFIX="%INSTALL_PATH%" -G "MinGW Makefiles" ..
```

#### Option 2: Manual Build

See [Manual Installation](#manual-installation) above and specify your generator in Step 2.

### Check Available Generators

To see all available generators on your system:
```cmd
cmake --help
```

Scroll to the bottom to see the "Generators" section.

---

## Using CityFlow in Your Projects

After installation, you can use CityFlow in any C++ projet.

### Project Structure
```
my-project/
├── CMakeLists.txt
└── main.cpp
```

### CMakeLists.txt
```cmake
cmake_minimum_required(VERSION 3.10)
project(MyTrafficSimulation)

set(CMAKE_CXX_STANDARD 11)
# Change to Debug if needed along with path below
set(CMAKE_BUILD_TYPE Release)
# Point to CityFlow installation
set(CMAKE_PREFIX_PATH "C:/Users/userName/local/cityflowcpp")  # Windows
# set(CMAKE_PREFIX_PATH "$ENV{HOME}/local/cityflowcpp")        # Linux

# Find CityFlow package
find_package(CityFlowCpp REQUIRED)

# Your executable
add_executable(my_project main.cpp)

# Link CityFlow library
target_link_libraries(my_project PRIVATE CityFlow::cityflow_lib)
```

### main.cpp
```cpp
#include <engine/engine.h>
#include <engine/archive.h>
#include <iostream>

int main() {
    // Create simulation engine
    CityFlow::Engine engine("config.json", 4);
    
    // Run simulation for 1000 steps
    for (int step = 0; step < 1000; step++) {
        engine.nextStep();
        
        if (step % 100 == 0) {
            int vehicleCount = engine.getVehicleCount();
            double avgTime = engine.getAverageTravelTime();
            
            std::cout << "Step " << step 
                      << ": " << vehicleCount << " vehicles, "
                      << "Avg travel time: " << avgTime << "s" 
                      << std::endl;
        }
    }
    
    // Save simulation state
    CityFlow::Archive archive(engine);
    archive.dump("simulation_state.archive");
    
    return 0;
}
```

## Uninstallation

### Windows
Simply delete the instalation directory:
```cmd
rmdir /s C:\Users\userName\local\cityflowcpp
```

### Linux
Delete the installation directory:
```bash
rm -rf ~/local/cityflowcpp
# Or for system-wide:
sudo rm -rf /usr/local/lib/libcityflowcpp*
sudo rm -rf /usr/local/include/cityflow
sudo rm -rf /usr/local/lib/cmake/CityFlowCpp
```

---
## Files used in simulations


```
when using files make sure Working directory is set corectly
  This mainlly affects:
   creation of Engine where you need to pass config.json
   creation of roadnet and flow inside config.json (see tag 'dir')
```
---
**Last Updated:** January 2026