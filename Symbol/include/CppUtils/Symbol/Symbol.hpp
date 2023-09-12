#pragma once

#include <filesystem>
#include <string_view>

#include <CppUtils/Platform/OS.hpp>
#include <CppUtils/Platform/Library.hpp>
#include <CppUtils/Type/Concept.hpp>

namespace CppUtils::Symbol
{
#if defined(OS_WINDOWS)

	template<Type::Concept::Function Function = void(*)()>
	[[nodiscard]] auto getFunction(std::string_view signature, HMODULE library = nullptr) -> Function
	{
		using namespace std::literals;
		auto handle = GetModuleHandle(nullptr);
		auto* functionPointer = GetProcAddress(handle, std::data(signature));
		if (!functionPointer)
			throw std::runtime_error("Couldn't load function "s + std::data(signature) + "\nError code: " + GetLastError());
		return reinterpret_cast<Function>(functionPointer);
	}

#elif defined(OS_LINUX) || defined(OS_MACOS)

	template<Type::Concept::Function Function = void(*)()>
	[[nodiscard]] auto getFunction(std::string_view signature, [[maybe_unused]] void* library = nullptr) -> Function // retirer le unused
	{
		using namespace std::literals;
		auto* handle = dlopen(nullptr, RTLD_LAZY);
		if (handle == nullptr) {
			throw std::runtime_error(dlerror());
		}
		auto* functionPointer = dlsym(handle, std::data(signature));
		if (!functionPointer)
			throw std::runtime_error("Couldn't load function "s + std::data(signature) + '\n' + dlerror());
		return reinterpret_cast<Function>(functionPointer);
	}

#endif
}
