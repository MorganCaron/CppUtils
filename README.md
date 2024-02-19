# CppUtils

<p align="center"><img src="resources/logo.svg" alt="Logo CppUtils" width="200" height="200"/></p>

## *C++ Utilities Library*

![Github Stars](https://img.shields.io/github/stars/MorganCaron/CppUtils?style=for-the-badge)
![Github Forks](https://img.shields.io/github/forks/MorganCaron/CppUtils?style=for-the-badge)
![Top Language](https://img.shields.io/github/languages/top/MorganCaron/CppUtils?style=for-the-badge)
[![Discord](https://img.shields.io/discord/268838260153909249?label=Chat&logo=Discord&style=for-the-badge)](https://discord.gg/mxZvun4)
[![License](https://img.shields.io/github/license/MorganCaron/CppUtils?style=for-the-badge)](https://github.com/MorganCaron/CppUtils/blob/master/LICENSE)

### Project Health
[![CI Windows](https://img.shields.io/github/workflow/status/MorganCaron/CppUtils/CI%20C++:%20Windows?label=Windows&logo=windows&logoColor=white&style=for-the-badge)](https://github.com/MorganCaron/CppUtils/actions/workflows/ci-cpp-windows.yml)
[![CI Windows MSYS2](https://img.shields.io/github/workflow/status/MorganCaron/CppUtils/CI%20C++:%20Windows%20MSYS2?label=Windows%20MSYS2&logo=windows&logoColor=white&style=for-the-badge)](https://github.com/MorganCaron/CppUtils/actions/workflows/ci-cpp-windows-msys2.yml)
[![CI Linux](https://img.shields.io/github/workflow/status/MorganCaron/CppUtils/CI%20C++:%20Linux?label=Linux&logo=linux&logoColor=white&style=for-the-badge)](https://github.com/MorganCaron/CppUtils/actions/workflows/ci-cpp-linux.yml)
[![CI MacOS](https://img.shields.io/github/workflow/status/MorganCaron/CppUtils/CI%20C++:%20MacOS?label=MacOS&logo=macos&logoColor=white&style=for-the-badge)](https://github.com/MorganCaron/CppUtils/actions/workflows/ci-cpp-macos.yml)

---

### In this library you can find:

- Additional containers ([trees](modules/Container/Tree.mpp), [meshes](modules/Container/MeshNetwork.mpp), [stacks](modules/Container/Stack.mpp))
- A [file modification listener](modules/FileSystem/FileWatcher.mpp)
- Tools for creating parsers and compilers (Work in progress <img src="resources/loading.gif" width="12" height="12"/> )
- Parsers and compilers for popular languages (CSS, INI, HTML, JSON, Markdown, XML) (Work in progress <img src="resources/loading.gif" width="12" height="12"/> )
- A logger [logger](modules/Log/Logger.mpp) with a [file rotation system](modules/Log/LogRotate.mpp)
- [Benchmark tools](modules/Log/ChronoLogger.mpp)
- [Hashing functions](modules/String/Hash.mpp)
- [Color terminal writing](modules/Terminal/TextModifier.mpp)
- [Tools for writing unit tests](modules/UnitTest/UnitTest.mpp)
- [STL polyfills](include/Stl)
- Many other functions

The [unit tests](tests) serve as examples of how to use these functions.



## Getting Started

A C++23 compliant compiler with std module support and XMake is needed to build CppUtils

This library is used in my C++ projects, but you can also use it in your projects.
Just follow the installation steps and consult the documentation for each feature you need.

[![Install](https://img.shields.io/badge/-Install-blue?style=for-the-badge)](INSTALL.md)
[![Contribute](https://img.shields.io/badge/-Contribute-blue?style=for-the-badge)](CONTRIBUTING.md)

### Build command
```console
xmake f --toolchain=llvm --runtimes="c++_shared" [--enable_tests=y] --yes
xmake [b|build] [-vD]
```

### Run tests
```console
xmake [r|run]
```

---
