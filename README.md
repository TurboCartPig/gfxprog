[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)

# Graphics Programming

A series of labs completed in association with the course PROG2002 at NTNU.

### How to build

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

To build the user must have the following libraries available for cmake to discover: GLFW3, GLEW, GLM, Catch2, ImGui, stb and native OpenGL headers.

### Install dependencies using native package manager

Fedora:
```bash
sudo dnf in glfw-devel glew-devel glm-devel catch-devel mesa-{libGL,libEGL}-devel
```

> ImGui and stb are not currently available in mirrors on fedora, use vcpkg to obtain them.

### Using vcpkg

[Vcpkg](https://github.com/Microsoft/vcpkg) is a ports like package manager for libraries that is the closest to a sane way of obtaining dependencies on Windows.

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

Install dependencies:
```bash
.\vcpkg install glfw3 glm glew catch imgui 'imgui[glfw-binding]' stb opengl
```

