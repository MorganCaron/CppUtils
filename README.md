# CppUtils

<p align="center"><img src="resources/logo.svg" alt="Logo CppUtils" width="200" height="200"/></p>

## *C++ Utilities Library*

![Github Stars](https://img.shields.io/github/stars/MorganCaron/CppUtils?style=for-the-badge)
![Github Forks](https://img.shields.io/github/forks/MorganCaron/CppUtils?style=for-the-badge)
![Top Language](https://img.shields.io/github/languages/top/MorganCaron/CppUtils?style=for-the-badge)
[![Discord](https://img.shields.io/discord/268838260153909249?label=Chat&logo=Discord&style=for-the-badge)](https://discord.gg/mxZvun4)
[![License](https://img.shields.io/github/license/MorganCaron/CppUtils?style=for-the-badge)](https://github.com/MorganCaron/CppUtils/blob/master/LICENSE)
![GitHub Sponsors](https://img.shields.io/github/sponsors/MorganCaron?style=for-the-badge)

### Project Health
![CI Windows](https://img.shields.io/github/actions/workflow/status/MorganCaron/CppUtils/ci-cpp-windows.yml?style=for-the-badge&logo=windows&logoColor=white&label=Windows)
![CI Linux](https://img.shields.io/github/actions/workflow/status/MorganCaron/CppUtils/ci-cpp-linux.yml?style=for-the-badge&logo=linux&logoColor=white&label=Linux)
![CI MacOS](https://img.shields.io/github/actions/workflow/status/MorganCaron/CppUtils/ci-cpp-macos.yml?style=for-the-badge&logo=macos&logoColor=white&label=MacOS)

![Codacy grade](https://img.shields.io/codacy/grade/49d265d3b8934ec398322a0a82c71184?style=for-the-badge&logo=codacy)

---

### Project Activity

![Alt](https://repobeats.axiom.co/api/embed/5ee3902d41c9a270bed3f8aa8dba9dd6298fd5ef.svg "Repobeats analytics image")

## Library Features

### 📦 Containers
- [`BidirectionalMap`](modules/Container/BidirectionalMap.mpp) - Two-way key-value mapping
- [`BTree`](modules/Container/BTree.mpp) - Balanced tree for large sorted datasets with efficient range queries and low memory access
- [`DependencyGraph`](modules/Container/DependencyGraph.mpp) - Dependency graph for managing dependencies between objects
- [`MeshNetwork`](modules/Container/MeshNetwork.mpp) - Graph of shared objects without hierarchy
- [`Size`](modules/Container/Size.mpp) - Semantic wrapper around `std::size_t` for representing 2D (`Size2`: width/height) and 3D (`Size3`: width/height/depth) sizes
- [`Stack`](modules/Container/Stack.mpp) - Type-specific stack without predefined type list (unlike [`TypedStack`](modules/Container/TypedStack.mpp))
- [`Tree`](modules/Container/Tree.mpp) - Hierarchical node structure with parent-child links
- [`TypedStack`](modules/Container/TypedStack.mpp) - Multi-type stack with exact object size layout, suitable for argument passing and VM stacks
- [`Vec2`](modules/Container/Vec2.mpp) / [`Vec3`](modules/Container/Vec3.mpp) - 2D/3D math vectors with operators and common functions

### 🚀 Execution
- [`Event`](modules/Execution/Event.mpp) - An event for thread synchronization
- [`EventDispatcher`](modules/Execution/EventDispatcher.mpp) - Event system to subscribe functions and trigger actions by event name
- [`ScopeGuard`](modules/Execution/ScopeGuard.mpp) - RAII utility to execute a function when leaving a scope, ensuring resource cleanup

### 📁 File System
- [`Watcher`](modules/FileSystem/Watcher.mpp) - File modification watcher

### 🧠 Functional
- [`LambdaCalculus`](modules/Functional/LambdaCalculus.mpp) - Compile-time utilities for lambda calculus manipulation

### 🔣 Languages (Parsers, Compilers, VM)
- Tools to create parsers and compilers (CSS, INI, HTML, JSON, Markdown, XML) (work in progress <img src="resources/loading.gif" width="12" height="12"/>)
- [`CLikeCompiler`](modules/Language/CLikeCompiler.mpp) - Compiler for C-inspired language
- [`GrammarParser`](modules/Language/GrammarParser.mpp) - A parser for defining and interpreting custom grammars, used for building language parsers
- [`MetaEvaluator`](modules/Language/MetaEvaluator.mpp) - Homoiconic meta-circular evaluator with extensible reflexivity
- [`ASTParser`](modules/Language/ASTParser.mpp) - AST parser (work in progress <img src="resources/loading.gif" width="12" height="12"/>)
- [`VirtualMachine`](modules/Language/VirtualMachine.mpp) - Generic virtual machine

### 📝 Logging & Benchmarking
- [`Logger`](modules/Log/Logger.mpp) - Formattable logger with customizable log types and appearances
- [`LogRotate`](modules/Log/LogRotate.mpp) - Log file rotation based on maximum file size
- [`ChronoLogger`](modules/Log/ChronoLogger.mpp) - RAII timer that logs elapsed time at scope exit

### 🧮 Math
- [`Random`](modules/Math/Random.mpp) - Pseudorandom number generation
- [`Utility`](modules/Math/Utility.mpp) - Floating-point comparison with epsilon tolerance

### 🔠 String
- [`Hash`](modules/String/Hash.mpp) - Hashing utilities for strings

### 🌐 Networking
- [`Client`](modules/Network/Client.mpp) - TCP client with synchronous and asynchronous modes
- [`Server`](modules/Network/Server.mpp) - TCP server with multi-client support

### 💻 Terminal
- [`Canvas`](modules/Terminal/Canvas.mpp) - Terminal-based 2D drawing surface
- [`Cursor`](modules/Terminal/Cursor.mpp) - Terminal cursor manipulation
- [`ProgressBar`](modules/Terminal/ProgressBar.mpp) - Dynamically updating terminal progress bar
- [`RawTerminal`](modules/Terminal/RawTerminal.mpp) - Raw input handling (no buffering or echo)
- [`Size`](modules/Terminal/Size.mpp) - Terminal size utilities
- [`TextColor`](modules/Terminal/TextColor.mpp) - Terminal text color utilities
- [`TextModifier`](modules/Terminal/TextModifier.mpp) - Utilities to style and color terminal text
- [`Title`](modules/Terminal/Title.mpp) - Terminal title utilities

### 🚦 Multithreading & Synchronization
- [`Scheduler`](modules/Thread/Scheduler.mpp) - Simple scheduler to run delayed functions on separate thread
- [`ThreadLoop`](modules/Thread/ThreadLoop.mpp) - Thread loop with exception handling
- [`ThreadPool`](modules/Thread/ThreadPool.mpp) - Fixed-size thread pool for parallel task execution
- [`TryAsync`](modules/Thread/TryAsync.mpp) - Launches a function asynchronously, forwards exception to caller
- [`UniqueLocker`](modules/Thread/UniqueLocker.mpp) - RAII wrapper holding a value with exclusive access
- [`SharedLocker`](modules/Thread/SharedLocker.mpp) - RAII wrapper holding a value with shared/exclusive access
- [`Accessor`](modules/Thread/UniqueLocker.mpp) - RAII accessor for reading/writing an `UniqueLocker` or exclusive access to a `SharedLocker`
- [`ReadOnlyAccessor`](modules/Thread/SharedLocker.mpp) - RAII accessor for shared (non-exclusive) reading of a `SharedLocker`, allowing parallel access
- [`MultipleAccessor`](modules/Thread/UniqueLocker.mpp) - RAII accessor for multiple lockers, safely acquiring them to avoid deadlocks and data races


### 🏷️ Type
- [`Concept`](modules/Type/Concept.mpp) - Extensions to `<type_traits>` and `<concepts>` providing additional compile-time checks and utilities
- [`Enum`](modules/Type/Enum.mpp) - Generic enum-to-string conversion
- [`Mapping`](modules/Type/ObjectMapping.mpp) - Static reflection on members (pre-C++26)
- [`Tuple`](modules/Type/Tuple.mpp) - Visitor for `std::tuple`
- [`VariadicTemplate`](modules/Type/VariadicTemplate.mpp) - Metaprogramming on variadic parameters
- [`Variant`](modules/Type/Variant.mpp) - Generic print and comparison operators for `std::variant`

### 🧪 Unit Testing
- [`DummyObject`](modules/UnitTest/DummyObject.mpp) - Dummy object that prints construction, destruction, copy and move operations, while counting copies and moves for test verification
- [`UnitTest`](modules/UnitTest/UnitTest.mpp) - Minimal test framework with assertions and filters

### 🧩 Miscellaneous
- Many other functions

---

## Getting Started

This library is used in my C++ projects, but you can also use it in your projects. Just follow the installation steps and consult the documentation for each feature you need.

### Prerequisites

- A C++26 compliant compiler with std module support
- XMake

### Installation

In your XMake projects:
```lua
add_repositories("xmake-repo git@github.com:MorganCaron/xmake-repo.git")
add_requires("CppUtils")

target("YourProject", function()
	add_packages("CppUtils", {public = true})
end)
```

### Configure the build via the xmake menu
```console
xmake f [--toolchain=llvm] --runtimes="c++_shared" [--sdk=/opt/llvm-git] --menu
```

### Configure the build manually
```console
xmake f [--toolchain=llvm] --runtimes="c++_shared" [--sdk=/opt/llvm-git] [-y|--yes] [--enable_tests=y] [--sanitize_memory=y] [--sanitize_thread=y]
```

### Build command
```console
xmake [b|build] [-vD]
```

### Build & Run tests
```console
xmake [r|run] [-d|--debug]
```

### Watch tests
```console
xmake watch -r [-d|--debug]
```

### Contribute

[![Contribute](https://img.shields.io/badge/-Contribute-blue?style=for-the-badge)](CONTRIBUTING.md)

---
