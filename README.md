# CppUtils

<p align="center"><img src="resources/logo.svg" alt="Logo CppUtils" width="200" height="200"/></p>

## *C++ Utilities Library*

![Github Stars](https://img.shields.io/github/stars/MorganCaron/CppUtils?style=for-the-badge)
![Github Forks](https://img.shields.io/github/forks/MorganCaron/CppUtils?style=for-the-badge)
![Top Language](https://img.shields.io/github/languages/top/MorganCaron/CppUtils?style=for-the-badge)
[![Discord](https://img.shields.io/discord/268838260153909249?label=Chat&logo=Discord&style=for-the-badge)](https://discord.gg/mxZvun4)
[![License](https://img.shields.io/github/license/MorganCaron/CppUtils?style=for-the-badge)](https://github.com/MorganCaron/CppUtils/blob/master/LICENSE)

### Project Health
![CI Windows](https://img.shields.io/github/actions/workflow/status/MorganCaron/CppUtils/ci-cpp-windows.yml?style=for-the-badge&logo=windows&logoColor=white&label=Windows)
![CI Windows MSYS2](https://img.shields.io/github/actions/workflow/status/MorganCaron/CppUtils/ci-cpp-windows-msys2.yml?style=for-the-badge&logo=windows&logoColor=white&label=Windows%20MSYS2)
![CI Linux](https://img.shields.io/github/actions/workflow/status/MorganCaron/CppUtils/ci-cpp-linux.yml?style=for-the-badge&logo=linux&logoColor=white&label=Linux)
![CI MacOS](https://img.shields.io/github/actions/workflow/status/MorganCaron/CppUtils/ci-cpp-macos.yml?style=for-the-badge&logo=macos&logoColor=white&label=MacOS)

---

### In this library you can find:

- Additional containers ([bidirectional map](modules/Container/BidirectionalMap.mpp), [b-tree](modules/Container/BTree.mpp), [meshes](modules/Container/MeshNetwork.mpp), [stacks](modules/Container/TypedStack.mpp), [trees](modules/Container/Tree.mpp))
- A [file modification listener](modules/FileSystem/Watcher.mpp)
- Tools for creating parsers and compilers (Work in progress <img src="resources/loading.gif" width="12" height="12"/> )
- Parsers and compilers for popular languages (CSS, INI, HTML, JSON, Markdown, XML) (Work in progress <img src="resources/loading.gif" width="12" height="12"/> )
- A logger [logger](modules/Log/Logger.mpp) with a [file rotation system](modules/Log/LogRotate.mpp)
- [Benchmark tools](modules/Log/ChronoLogger.mpp)
- [Hashing functions](modules/String/Hash.mpp)
- Networking functions ([Server](modules/Network/Server.mpp)/[Client](modules/Network/Client.mpp))
- [Color terminal writing](modules/Terminal/TextModifier.mpp)
- Utilities for securing multithreaded accesses ([UniqueLocker](modules/Thread/UniqueLocker.mpp), [SharedLocker](modules/Thread/SharedLocker.mpp))
- A [ThreadPool](modules/Thread/ThreadPool.mpp)
- [Tools for writing unit tests](modules/UnitTest/UnitTest.mpp)
- [STL polyfills](include/Stl)
- Many other functions

The [unit tests](tests) serve as examples of how to use these functions.



## Getting Started

A C++23 compliant compiler with std module support and XMake is needed to build CppUtils

This library is used in my C++ projects, but you can also use it in your projects.
Just follow the installation steps and consult the documentation for each feature you need.

### Installation

In your XMake projects:
```lua
add_repositories("xmake-repo git@github.com:MorganCaron/xmake-repo.git")
add_requires("CppUtils")

target("YourProject", function()
	add_packages("CppUtils", { public = true })
end)
```

### Contribute

### Dependencies

- wayland
- wayland-protocol

### Build command

```console
xmake f [--toolchain=llvm] --runtimes="c++_shared" [--sdk=/opt/llvm-git] [--enable_tests=y] --yes
xmake [b|build] [-vD]
```

### Run tests

```console
xmake [r|run]
```

### Watch tests
```console
xmake watch -r
```

[![Contribute](https://img.shields.io/badge/-Contribute-blue?style=for-the-badge)](CONTRIBUTING.md)

---
