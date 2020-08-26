[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)

# Graphics Programming

A series of labs completed in association with the course PROG2002 at NTNU.

### How to build

To build the user must have the following libraries available for cmake to discover: GLFW3, GLEW, GLM and native OpenGL headers.

Configure:
```bash
cmake -G Ninja -S . -B build
```

Build:
```bash
cmake --build build/
```

Test:
```bash
cmake --build build/ --target test
```

Docs:
```bash
cmake --build build/ --target docs
```

