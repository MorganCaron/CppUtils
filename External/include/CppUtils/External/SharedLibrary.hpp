#pragma once

#include <string>
#include <sstream>
#include <utility>
#include <iostream>
#include <string_view>

#include <CppUtils/Platform/Library.hpp>
#include <CppUtils/Symbol/Symbol.hpp>

namespace CppUtils::External
{
#if defined(OS_WINDOWS)

	[[nodiscard]] inline auto openLibrary(const std::filesystem::path& libraryPath) -> Platform::Library
	{
		auto wstringPath = libraryPath.generic_wstring();
		auto library = LoadLibraryExW(std::data(wstringPath), nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
		if (!library)
			throw std::runtime_error{"Couldn't load library " + wstringPath + "\n" + GetLastError()};
		return library;
	}

#elif defined(OS_LINUX) || defined(OS_MACOS)

	[[nodiscard]] inline auto openLibrary(const std::filesystem::path& libraryPath) -> Platform::Library
	{
		auto stringPath = libraryPath.string();
		auto library = dlopen(std::data(stringPath), RTLD_LAZY);
		if (!library)
			throw std::runtime_error{"Couldn't load library " + stringPath + "\n" + dlerror()};
		return library;
	}

#endif

#if defined(OS_WINDOWS)

	inline auto closeLibrary(Platform::Library library) -> void
	{
		static_cast<void>(FreeLibrary(library));
	}

#elif defined(OS_LINUX) || defined(OS_MACOS)

	inline auto closeLibrary(Platform::Library library) -> void
	{
		static_cast<void>(dlclose(library));
	}

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
			m_library = openLibrary(libraryPath);
		}
		~SharedLibrary() noexcept
		{
			closeLibrary(m_library);
		}

		template<Type::Concept::Function Function = void(*)()>
		[[nodiscard]] auto getFunction(std::string_view signature) -> Function
		{
			Symbol::getFunction<Function>(signature, m_library);
		}

	private:
		Platform::Library m_library = nullptr;
	};
}
