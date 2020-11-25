[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)

# Graphics Programming

Labs and assignments completed in association with Graphics Programming / PROG2002 at NTNU.

## Contents

#### Labs - Smaller samples of specific features and concepts
  1. Triangle - Draws a triangle to the screen
  2. Square - Draws a pulsating square to the screen
  3. MVP - Draws a movable square to the screen
  4. Texture - Draws a textured square to the screen
  5. Model - Draws a spinning 3D model
  6. (Unimplemented) Shadows - Draws a spinning 3D model that casts a shadow.

#### Assignments - More complete game-like demos
  1. Pacman - A clone of the pacman
  2. Pacman in 3D - The game rules of pacman, but in 3D

#### Glove - Object Oriented OpenGL Wrapper

Glove is a small OpenGL wrapping library that implements all the common functionality of the labs and assignments, and exposes it in a slightly safer, object oriented API.

## How to obtain

Cloning:
```bash
git clone https://github.com/TurboCartPig/gfxprog.git --recurse-submodules
```
> Submodules should be checked out at recorded SHA

## How to build

Configure:
```bash
cmake -G Ninja -S . -B build/
```

Build:
```bash
cmake --build build/
```

Tests:
```bash
cmake --build build/ --target tests
```

Docs:
```bash
cmake --build build/ --target docs
```

## Dependencies

The only hard requirement when it comes to libraries is OpenGL. The rest will be downloaded automatically through vcpkg, if not found locally.

> Currently assimp is broken in vcpkg on linux and you will have to install it using the native package manger.

### Install dependencies using native package manager

Fedora:
```bash
sudo dnf in {assimp,glfw,glew,glm,catch,mesa-libGL}-devel
```
> stbi-devel in fedora repos doesn't work, so it will be downloaded automatically.

Ubuntu:
```bash
sudo apt install lib{assimp,gl,glfw3,glew,glm}-dev
```
> Ubuntu instructions are yet to be tested
> catch2 is not available in ubuntu it seems, the projects will build without it.

Windows:
- Be sure to have the C++ core features package in visual studio installed
- Use vcpkg for dependencies
> Windows instructions are yet to be tested

### Build using vcpkg manually

[Vcpkg](https://github.com/Microsoft/vcpkg) is a ports like package manager for libraries that is the closest to a sane way of obtaining dependencies on Windows.

This project uses a vcpkg manifest to specify it's dependencies, Meaning that the dependencies will be automatically downloaded and built on cmake configuration. This is a rather new feature of vcpkg so make sure you use a new build.

Install vcpkg:
```bash
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat -disableMetrics
.\vcpkg integrate install
```

The last command above gives you a cmake variable. Pass it to cmake as such:
```bash
cmake -G Ninja -S . -B build -DCMAKE_TOOLCHAIN_FILE="C:\path\to\vcpkg.cmake"
```
