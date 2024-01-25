#pragma once

#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>

#include <CppUtils/Type/Concept.hpp>

#if defined(OS_LINUX) || defined(OS_MACOS)
#	include <dlfcn.h>
#endif

namespace CppUtils::External
{
#if defined(OS_WINDOWS)
	using Library = HMODULE;
#elif defined(OS_LINUX) or defined(OS_MACOS)
	using Library = void*;
#else
#	error unsupported platform
#endif

	class DLL_PUBLIC SharedLibrary final
	{
	public:
#if defined(OS_WINDOWS)
		static constexpr auto ext = std::string_view{".dll"};
#elif defined(OS_LINUX)
		static constexpr auto ext = std::string_view{".so"};
#elif defined(OS_MACOS)
		static constexpr auto ext = std::string_view{".dylib"};
#else
#	error unsupported platform=
#endif

		SharedLibrary() = default;
		SharedLibrary(const SharedLibrary&) = delete;
		SharedLibrary(SharedLibrary&& src) noexcept:
			m_library{std::exchange(src.m_library, nullptr)}
		{}
		SharedLibrary& operator=(const SharedLibrary&) = delete;
		SharedLibrary& operator=(SharedLibrary&& rhs) noexcept
		{
			m_library = std::exchange(rhs.m_library, nullptr);
			return *this;
		}

		explicit SharedLibrary(const std::filesystem::path& libraryPath)
		{
#if defined(OS_WINDOWS)
			auto wstringPath = libraryPath.generic_wstring();
			m_library = LoadLibraryExW(std::data(wstringPath), nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
			if (!m_library)
				throw std::runtime_error{"Couldn't load library " + wstringPath + "\n" + GetLastError()};
#elif defined(OS_LINUX) or defined(OS_MACOS)
			auto stringPath = libraryPath.string();
			m_library = dlopen(std::data(stringPath), RTLD_LAZY);
			if (!m_library)
				throw std::runtime_error{"Couldn't load library " + stringPath + "\n" + dlerror()};
#endif
		}
		~SharedLibrary() noexcept
		{
#if defined(OS_WINDOWS)
			static_cast<void>(FreeLibrary(m_library));
#elif defined(OS_LINUX) or defined(OS_MACOS)
			static_cast<void>(dlclose(m_library));
#endif
		}

		template<Type::Concept::Function Function = void(*)()>
		[[nodiscard]] auto getFunction(std::string_view signature) -> Function
		{
			using namespace std::literals;
#if defined(OS_WINDOWS)
			auto handle = GetModuleHandle(nullptr);
			auto* functionPointer = GetProcAddress(handle, std::data(signature));
			if (!functionPointer)
				throw std::runtime_error("Couldn't load function "s + std::data(signature) + "\nError code: " + GetLastError());
#elif defined(OS_LINUX) or defined(OS_MACOS)
			auto* handle = dlopen(nullptr, RTLD_LAZY);
			if (handle == nullptr)
				throw std::runtime_error(dlerror());
			auto* functionPointer = dlsym(handle, std::data(signature));
			if (!functionPointer)
				throw std::runtime_error("Couldn't load function "s + std::data(signature) + '\n' + dlerror());
#endif
			return reinterpret_cast<Function>(functionPointer);
		}

	private:
		Library m_library = nullptr;
	};
}
