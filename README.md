[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)

# CMake Template Project

A relativly simple template for a cmake project setup with a separation between binaries and libraries, in addition to testing via [Catch2](https://github.com/catchorg/Catch2) and documentation via [Doxygen](https://doxygen.nl).

### How to build

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

### Further Reading

- [Modern CMake Tutorial](https://cliutils.gitlab.io/modern-cmake)
- [Catch2 Tutorial](https://github.com/catchorg/Catch2/blob/master/docs/tutorial.md#top)

