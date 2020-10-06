[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)

# Graphics Programming

Labs and assignments completed in association with Graphics Programming / PROG2002 at NTNU.

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

The only hard requirement when it comes to libraries is OpenGL. The rest will be downloaded automatically, if not found.

### Install dependencies using native package manager

Fedora:
```bash
sudo dnf in glfw-devel glew-devel glm-devel catch-devel mesa-{libGL,libEGL}-devel
```
> ImGui is not currently available in mirrors on fedora and stbi-devel doesn't work, so they will be downloaded automatically

Ubuntu:
```bash
sudo apt install libgl-dev libglfw3-dev libglew-dev libglm-dev catch2
```
> Ubuntu instructions are yet to be tested

Windows:
- Be sure to have the C++ core features package in visual studio installed
> Windows instructions are yet to be tested


### Using vcpkg

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
