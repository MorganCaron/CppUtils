# CppUtils

<p align="center"><img src="resources/logo.svg" alt="Logo CppUtils" width="200" height="200" /></p>

## *C++ Utilities Library*

![Github Stars](https://img.shields.io/github/stars/MorganCaron/CppUtils?style=for-the-badge)
![Github Forks](https://img.shields.io/github/forks/MorganCaron/CppUtils?style=for-the-badge)
[![Discord](https://img.shields.io/discord/268838260153909249?label=Chat&logo=Discord&style=for-the-badge)](https://discord.gg/mxZvun4)

### Project Health
[![Codacy](https://img.shields.io/codacy/grade/2fab3fcb24a34138a9e77f847da68b28?logo=Codacy&style=for-the-badge)](https://www.codacy.com/manual/MorganCaron/CppUtils)
[![Github Actions](https://img.shields.io/github/workflow/status/MorganCaron/CppUtils/CI%20C++?logo=Github&style=for-the-badge)](https://github.com/MorganCaron/CppUtils/actions?query=workflow%3A%22CI+C%2B%2B%22)

---

## Getting Started

This library is used in my C++ projects, but you can also use it in your projects.
Just follow the installation steps and consult the documentation for each feature you need.

[![Install](https://img.shields.io/badge/-Install-blue?style=for-the-badge)](INSTALL.md)
[![Contribute](https://img.shields.io/badge/-Contribute-blue?style=for-the-badge)](CONTRIBUTING.md)

### Build command
```console
meson build -Ddefault_library=static
ninja -C build
```

### Test command
```console
ninja -C build test
```

---

## Documentation

- **[Chrono](Chrono/README.md)**
	- [Chronometer](Chrono/README.md#Chronometer)
- **[Container](Container/README.md)**
	- [Vector](Container/README.md#Vector)
- **[External](External/README.md)**
	- [DllExport](External/README.md#DllExport)
	- [DynamicLibrary](External/README.md#DynamicLibrary)
- **[FileSystem](FileSystem/README.md)**
	- [File](FileSystem/README.md#File)
- **[Functional](Functional/README.md)**
	- [Function](Functional/README.md#Function)
- **[Graph](Graph/README.md)**
	- [MeshNode](Graph/README.md#MeshNode)
	- [TreeNode](Graph/README.md#TreeNode)
- **[Hash](Hash/README.md)**
- **[Language](Language/README.md)**
	- [ASM](Language/README.md#ASM)
	- [Compiler](Language/README.md#Compiler)
	- [Interpreter](Language/README.md#Interpreter)
	- [Lexer](Language/README.md#Lexer)
	- [Parser](Language/README.md#Parser)
- **[Log](Log/README.md)**
	- [ChronoLogger](Log/README.md#ChronoLogger)
	- [Logger](Log/README.md#Logger)
- **[Platform](Platform/README.md)**
	- [OS](Platform/README.md#OS)
- **[String](String/README.md)**
- **[Switch](Switch/README.md)**
- **[Terminal](Terminal/README.md)**
	- [TextModifier](Terminal/TextModifier/README.md)
- **[Test](Test/README.md)**
	- [UnitTest](Test/README.md#UnitTest)
- **[Thread](Thread/README.md)**
	- [AsyncIStreamListener](Thread/README.md#AsyncStreamListener)
	- [LoopThread](Thread/README.md#LoopThread)
- **[Type](Type/README.md)**
	- [Cloneable](Type/README.md#Cloneable)
	- [Named](Type/README.md#Named)
	- [NonCopyable](Type/README.md#NonCopyable)
	- [NonInstanciable](Type/README.md#NonInstanciable)
	- [NonMovable](Type/README.md#NonMovable)
	- [Typed](Type/README.md#Typed)
	- [TypeId](Type/README.md#TypeId)

---

## Additional Informations
![Top Language](https://img.shields.io/github/languages/top/MorganCaron/CppUtils?style=for-the-badge)
[![License](https://img.shields.io/github/license/MorganCaron/CppUtils?style=for-the-badge)](https://github.com/MorganCaron/CppUtils/blob/master/LICENSE)
