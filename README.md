# CppUtils

<p align="center">
  <img src="resources/logo.svg" alt="Logo CppUtils" width="200" height="200"/>
</p>

<h2 align="center"><em>C++ Utilities Library</em></h2>

<p align="center">
  <a href="https://github.com/MorganCaron/CppUtils/releases"><img src="https://img.shields.io/github/v/release/MorganCaron/CppUtils?style=for-the-badge&logo=github" alt="Release"/></a>
  <a href="https://morgancaron.github.io/CppUtils/"><img src="https://img.shields.io/badge/-Documentation-blue?style=for-the-badge" alt="Documentation"/></a>
  <a href="https://discord.gg/mxZvun4"><img src="https://img.shields.io/discord/268838260153909249?label=Chat&logo=Discord&style=for-the-badge" alt="Discord"/></a>
  <a href="https://github.com/MorganCaron/CppUtils/blob/master/LICENSE"><img src="https://img.shields.io/github/license/MorganCaron/CppUtils?style=for-the-badge" alt="License"/></a>
  <br/>
  <img src="https://img.shields.io/github/stars/MorganCaron/CppUtils?style=for-the-badge" alt="Github Stars"/>
  <img src="https://img.shields.io/github/forks/MorganCaron/CppUtils?style=for-the-badge" alt="Github Forks"/>
  <img src="https://img.shields.io/github/languages/top/MorganCaron/CppUtils?style=for-the-badge" alt="Top Language"/>
  <img src="https://img.shields.io/github/sponsors/MorganCaron?style=for-the-badge" alt="GitHub Sponsors"/>
  <a href="CONTRIBUTING.md"><img src="https://img.shields.io/badge/-Contribute-blue?style=for-the-badge" alt="Contribute"/></a>
</p>

<p align="center">
  <a href="https://morgancaron.github.io/CppUtils/"><strong><font size="+2">Explore the Official Documentation</font></strong></a>
</p>
<p align="center">
  <strong><a href="https://morgancaron.github.io/CppUtils/en/guides/getting-started/"><font size="+1">Getting Started</font></a></strong> &nbsp;&bull;&nbsp;
  <strong><a href="https://morgancaron.github.io/CppUtils/en/reference/"><font size="+1">API Reference</font></a></strong> &nbsp;&bull;&nbsp;
  <strong><a href="https://discord.gg/mxZvun4"><font size="+1">Discord Community</font></a></strong>
</p>

### Project Health

<p align="center">
  <img src="https://img.shields.io/github/actions/workflow/status/MorganCaron/CppUtils/ci-cpp-windows.yml?branch=master&style=for-the-badge&logo=windows&logoColor=white&label=Windows" alt="CI Windows"/>
  <img src="https://img.shields.io/github/actions/workflow/status/MorganCaron/CppUtils/ci-cpp-linux.yml?branch=master&style=for-the-badge&logo=linux&logoColor=white&label=Linux" alt="CI Linux"/>
  <img src="https://img.shields.io/github/actions/workflow/status/MorganCaron/CppUtils/ci-cpp-macos.yml?branch=master&style=for-the-badge&logo=macos&logoColor=white&label=MacOS" alt="CI MacOS"/>
  <img src="https://img.shields.io/codacy/grade/49d265d3b8934ec398322a0a82c71184?style=for-the-badge&logo=codacy" alt="Codacy grade"/>
</p>

---

### Project Activity

![Alt](https://repobeats.axiom.co/api/embed/5ee3902d41c9a270bed3f8aa8dba9dd6298fd5ef.svg "Repobeats analytics image")

## Library Features

### 📦 Containers
- [`BidirectionalMap`](modules/Container/BidirectionalMap.mpp) - Two-way key-value mapping
- [`BTree`](modules/Container/BTree.mpp) - Balanced tree for large sorted datasets with efficient range queries and low memory access
- [`DependencyGraph`](modules/Container/DependencyGraph.mpp) - Dependency graph for managing dependencies between objects
- [`FlatHashMap`](modules/Container/FlatHashMap.mpp) - Constexpr associative hash map with O(1) lookup and cache-friendly contiguous storage
- [`MeshNetwork`](modules/Container/MeshNetwork.mpp) - Graph of shared objects without hierarchy
- [`MultiKeyMap`](modules/Container/MultiKeyMap.mpp) - Associative container with multi-axis O(1) lookup and contiguous storage
- [`NetworkPtr`](modules/Container/NetworkPtr.mpp) - Intrusive smart pointer representing graph-aware nodes with automatic cycle breaking
- [`SafeShared`](modules/Container/SafeShared.mpp) - Thread-safe smart pointer with monitor-object encapsulation and compile-time (`constexpr`) evaluation support
- [`Size`](modules/Container/Size.mpp) - Semantic wrapper around `std::size_t` for representing 2D (`Size2`: width/height) and 3D (`Size3`: width/height/depth) sizes
- [`Stack`](modules/Container/Stack.mpp) - Type-specific stack without predefined type list (unlike [`TypedStack`](modules/Container/TypedStack.mpp))
- [`Tree`](modules/Container/Tree.mpp) - Hierarchical node structure with parent-child links
- [`TypedStack`](modules/Container/TypedStack.mpp) - Multi-type stack with exact object size layout, suitable for argument passing and VM stacks
- [`Vec2`](modules/Container/Vec2.mpp) / [`Vec3`](modules/Container/Vec3.mpp) - 2D/3D math vectors with operators and common functions

### 🔐 Crypto
- [`HMAC`](modules/Crypto/HMAC.mpp) - Keyed-Hash Message Authentication Code (HMAC-SHA256)
- [`SHA256`](modules/Crypto/SHA256.mpp) - SHA-256 cryptographic hashing implementation

### 🎯 Execution
- [`Event`](modules/Execution/Event.mpp) - An event for thread synchronization
- [`EventDispatcher`](modules/Execution/EventDispatcher.mpp) - Event system to subscribe functions and trigger actions by event name
- [`EventQueue`](modules/Execution/EventQueue.mpp) - Thread-safe event queue running on a dedicated thread for asynchronous event processing (preserves event order)
- [`Pipeline`](modules/Execution/Pipeline.mpp) - Monadic execution pipeline with short-circuiting on `std::expected` failures
- [`Retry`](modules/Execution/Retry.mpp) - Retries operations with configurable attempts and delay
- [`ScopeGuard`](modules/Execution/ScopeGuard.mpp) - RAII utility to execute a function when leaving a scope, ensuring resource cleanup
- [`TryExecute`](modules/Execution/TryExecute.mpp) - Executes operations and captures exceptions as `std::expected`

### 📁 Filesystem
- [`Directory`](modules/FileSystem/Directory.mpp) - Directory traversal (`forFiles`, `forDirectories`) and RAII temporary directories
- [`File`](modules/FileSystem/File.mpp) - Binary and text file I/O, including optimized block-by-block reading
- [`FileRange`](modules/FileSystem/FileRange.mpp) - Lazy line-by-line file streaming view
- [`FileStaging`](modules/FileSystem/FileStaging.mpp) - File isolation and lifecycle management
- [`IndexedStorage`](modules/FileSystem/IndexedStorage.mpp) - Thread-safe indexed storage engine for storing and retrieving serializable types in chunked binary files
- [`MeshPager`](modules/FileSystem/MeshPager.mpp) - Paging mechanism for loading and saving `MeshNetwork` nodes on-demand using `IndexedStorage`
- [`Watcher`](modules/FileSystem/Watcher.mpp) - File modification watcher

### ⛓️ Flow
- [`Flow`](modules/Language/XML/Flow.mpp) - Declarative data transformation and file orchestration with staging, watching, and parallel chunking
- [`FlowRunner`](modules/Language/XML/FlowRunner.mpp) - Orchestrator discovering, indexing, and executing XML flows in parallel
- [`Pipeline`](modules/Language/XML/Pipeline.mpp) - Lazy C++20 range pipeline compiled from XML tags
- [`Tags`](modules/Language/XML/Tags.mpp) - Standard pipeline tags (`<Filter>`, `<Validate>`, `<Transform>`, `<Operation>`, `<Call>`, `<When>`, `<Let>`, `<Log>`, `<Rejected>`, `<Scope>`, `<Include>`)

### 🧠 Functional
- [`LambdaCalculus`](modules/Functional/LambdaCalculus.mpp) - Compile-time utilities for lambda calculus manipulation

### 🔣 Languages (Parsers, Compilers, VM)
- Tools to create parsers and compilers (CSS, INI, HTML, JSON, Markdown, XML) (work in progress <img src="resources/loading.gif" width="12" height="12"/>)
- [`CSV Lexer`](modules/Language/CSV/CSVLexer.mpp) - [RFC 4180](https://www.rfc-editor.org/rfc/rfc4180) compliant CSV lexer and table parser with grammar interpretation
- [`CSV Mapping`](modules/Language/CSV/Mapping.mpp) - Type-safe CSV to struct mapping with support for custom conversion functions and CSV generation
- [`CSV Parsing`](modules/Language/CSV/Parsing.mpp) - Stream-based CSV line parser
- [`CLikeCompiler`](modules/Language/CLikeCompiler.mpp) - Compiler for C-inspired language
- [`GrammarParser`](modules/Language/GrammarParser.mpp) - A parser for defining and interpreting custom grammars, used for building language parsers
- [`JSON Lexer`](modules/Language/JSON/JsonLexer.mpp) - Grammar-based JSON lexer with compile-time AST generation
- [`JSON Mapping`](modules/Language/JSON/Mapping.mpp) - Type-safe bidirectional mapping between C++ structs and JSON with compile-time reflection
- [`JSON Parsing`](modules/Language/JSON/Parsing.mpp) - High-performance typed JSON parser
- [`MetaCircularVirtualMachine`](modules/Language/MetaCircularVirtualMachine.mpp) - Homoiconic meta-circular virtual machine with extensible reflexivity
- [`MetaCircularParser`](modules/Language/MetaCircularParser.mpp) - Extension of `MetaCircularVirtualMachine` for building parsers with input stream reading capabilities
- [`ASTParser`](modules/Language/ASTParser.mpp) - AST parser (work in progress <img src="resources/loading.gif" width="12" height="12"/>)
- [`VirtualMachine`](modules/Language/VirtualMachine.mpp) - Generic virtual machine

### 📝 Logging & Benchmarking
- [`Logger`](modules/Log/Logger.mpp) - Asynchronous, configurable and formattable logger with support for colors and customizable log types
- [`FileSink`](modules/Log/FileSink.mpp) - Pluggable sink for automated file logging with integrated rotation and file size management
- [`LogRotate`](modules/Log/LogRotate.mpp) - Log file rotation based on maximum file size
- [`ChronoLogger`](modules/Log/ChronoLogger.mpp) - RAII timer that logs elapsed time at scope exit

### 🧮 Math
- [`BigInt`](modules/Math/BigInt.mpp) - Arbitrary-precision integer arithmetic
- [`Easing`](modules/Math/Easing.mpp) - Collection of easing functions for smooth animation
- [`Random`](modules/Math/Random.mpp) - Pseudorandom number generation
- [`Utility`](modules/Math/Utility.mpp) - Floating-point comparison with epsilon tolerance

### 🔠 String
- [`Encoding`](modules/String/Encoding.mpp) - UTF-8 and UTF-32 conversion and character display width utilities
- [`Hash`](modules/String/Hash.mpp) - Hashing utilities for strings

### 🌐 Networking
- [`Client`](modules/Network/Client.mpp) - TCP client with synchronous and asynchronous modes
- [`Server`](modules/Network/Server.mpp) - TCP server with multi-client support

### 🧩 Patterns
- [`Singleton`](modules/Pattern/Singleton.mpp) - Generic Meyers Singleton implementation (thread-safe and lazy)
- [`Multiton`](modules/Pattern/Multiton.mpp) - Generic Multiton implementation based on the Meyers Singleton

### 📶 Ranges
- [`Drain`](modules/Ranges/Drain.mpp) - Consuming range adaptor draining elements from a range
- [`Expected`](modules/Ranges/Expected.mpp) - Range adaptors for `std::expected` streams
- [`Inspect`](modules/Ranges/Inspect.mpp) - Range element inspection for side-effects and logging
- [`Parallel`](modules/Ranges/Parallel.mpp) - Multithreaded range pipelining

### 💻 Terminal
- [`Area`](modules/Terminal/Area.mpp) - Dedicated drawing context for a single child widget
- [`Canvas`](modules/Terminal/Canvas.mpp) - Terminal-based 2D drawing surface
- [`Cursor`](modules/Terminal/Cursor.mpp) - Terminal cursor manipulation
- [`Layout`](modules/Terminal/Layout.mpp) - Container widget for dynamic children positioning
- [`Primitive`](modules/Terminal/Primitive.mpp) - Basic drawing primitives for terminal (lines, rectangles, circles, ellipses)
- [`ProgressBar`](modules/Terminal/ProgressBar.mpp) - Dynamically updating terminal progress bar
- [`RawTerminal`](modules/Terminal/RawTerminal.mpp) - Raw input handling (no buffering or echo)
- [`Scrollable`](modules/Terminal/Scrollable.mpp) - Scrollable viewport for viewing large content areas
- [`Size`](modules/Terminal/Size.mpp) - Terminal size utilities
- [`Spinner`](modules/Terminal/Spinner.mpp) - Animated widget displaying a sequence of frames
- [`TextColor`](modules/Terminal/TextColor.mpp) - Terminal text color utilities
- [`TextModifier`](modules/Terminal/TextModifier.mpp) - Utilities to style and color terminal text
- [`Title`](modules/Terminal/Title.mpp) - Terminal title utilities
- [`Viewport`](modules/Terminal/Viewport.mpp) - Rectangular region with clipping and intersection logic
- [`Widget`](modules/Terminal/Widget.mpp) - UI interface for rendering and update lifecycle
- [`WidgetManager`](modules/Terminal/WidgetManager.mpp) - Central hub for widget events and update scheduling

### 🚦 Multithreading & Synchronization
- [`Scheduler`](modules/Thread/Scheduler.mpp) - Simple scheduler to run delayed functions on separate thread
- [`ScheduledEventDispatcher`](modules/Thread/ScheduledEventDispatcher.mpp) - Dispatches events asynchronously with timed (delay/when) execution
- [`AsyncEventDispatcher`](modules/Thread/AsyncEventDispatcher.mpp) - Event dispatcher that uses a ThreadPool for immediate asynchronous event execution
- [`ThreadLoop`](modules/Thread/ThreadLoop.mpp) - Thread loop with exception handling
- [`ThreadPool`](modules/Thread/ThreadPool.mpp) - Fixed-size thread pool for parallel task execution
- [`TryAsync`](modules/Thread/TryAsync.mpp) - Launches a function asynchronously, forwards exception to caller
- [`UniqueLocker`](modules/Thread/UniqueLocker.mpp) - RAII wrapper holding a value with exclusive access
- [`SharedLocker`](modules/Thread/SharedLocker.mpp) - RAII wrapper holding a value with shared/exclusive access
- [`SharedPtr`](modules/Thread/SharedPtr.mpp) - Thread-safe reference-counting smart pointer (`SharedPtr`, `WeakPtr`, `makeShared`, `ownerEqual`) with compile-time (`constexpr`) and runtime atomic support
- [`Accessor`](modules/Thread/UniqueLocker.mpp) - RAII accessor for reading/writing an `UniqueLocker` or exclusive access to a `SharedLocker`
- [`ReadOnlyAccessor`](modules/Thread/SharedLocker.mpp) - RAII accessor for shared (non-exclusive) reading of a `SharedLocker`, allowing parallel access
- [`MultipleAccessor`](modules/Thread/UniqueLocker.mpp) - RAII accessor for multiple lockers, safely acquiring them to avoid deadlocks and data races

### 🏷️ Type
- [`Concept`](modules/Type/Concept.mpp) - Extensions to `<type_traits>` and `<concepts>` providing additional compile-time checks and utilities
- [`Enum`](modules/Type/Enum.mpp) - Generic enum-to-string conversion
- [`Mapping`](modules/Type/Mapping.mpp) - Generic compile-time mapping between values
- [`ReadWriteAdapter`](modules/Type/ReadWriteAdapter.mpp) - `write` and `read` CPOs (Customization Point Objects) mapping stream and buffer operations (std::ostream, std::istream, std::vector<std::byte>, std::span<const std::byte>, etc)
- [`Serializer`](modules/Type/Serializer.mpp) - `serialize` and `deserialize` CPOs (Customization Point Objects) and custom Binary/Text Serializers
- [`Tuple`](modules/Type/Tuple.mpp) - Visitor for `std::tuple`
- [`VariadicTemplate`](modules/Type/VariadicTemplate.mpp) - Metaprogramming on variadic parameters
- [`Variant`](modules/Type/Variant.mpp) - Generic print and comparison operators for `std::variant`

### 🧪 Unit Testing
- [`LifetimeChecker`](modules/UnitTest/LifetimeChecker.mpp) - Object that prints construction, destruction, copy and move operations, while counting copies and moves for test verification
- [`UnitTest`](modules/UnitTest/UnitTest.mpp) - Minimal test framework with assertions and filters

### 🧩 Miscellaneous
- Many other functions

---

## Installation

Add CppUtils to your `xmake.lua`:

```lua
add_repositories("xmake-repo https://github.com/MorganCaron/xmake-repo.git")
add_requires("CppUtils 0.1.*") -- or add_requires("CppUtils") for latest

target("YourProject", function()
	add_packages("CppUtils", {public = true})
end)
```

> For step-by-step instructions, compiler prerequisites, and modular C++26 code examples, see the **[Getting Started Guide](https://morgancaron.github.io/CppUtils/en/guides/getting-started/)**.

---

## Contributing

Contributions are welcome! Please refer to **[CONTRIBUTING.md](CONTRIBUTING.md)** for development guidelines, test execution, and build instructions.

---
