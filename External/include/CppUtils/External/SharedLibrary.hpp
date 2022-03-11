#pragma once

#include <string>
#include <sstream>
#include <utility>
#include <iostream>
#include <string_view>

#include <CppUtils/Platform/OS.hpp>
#include <CppUtils/Platform/Windows.hpp>
#include <CppUtils/External/DllExport.hpp>

#if defined(OS_LINUX) || defined(OS_MACOS)
#	include <dlfcn.h>
#endif

namespace CppUtils::External
{
	using namespace std::literals;
	
	class DLL_PUBLIC SharedLibrary final
	{
	public:
	#if defined(OS_WINDOWS)
		static constexpr auto ext = ".dll"sv;
	#elif defined(OS_LINUX)
		static constexpr auto ext = ".so"sv;
	#elif defined(OS_MACOS)
		static constexpr auto ext = ".dylib"sv;
	#endif

		SharedLibrary() = default;
		SharedLibrary(const SharedLibrary&) = delete;
		SharedLibrary(SharedLibrary&& src) noexcept:
			m_library(std::exchange(src.m_library, nullptr))
		{}
		SharedLibrary& operator=(const SharedLibrary&) = delete;
		SharedLibrary& operator=(SharedLibrary&& rhs) noexcept
		{
			m_library = std::exchange(rhs.m_library, nullptr);
			return *this;
		}

		explicit SharedLibrary(std::string_view path)
		{
			open(path);
		}
		~SharedLibrary() noexcept
		{
			close();
		}

	#if defined(OS_WINDOWS)

		template<typename Signature>
		Signature getSymbol(std::string_view name)
		{
			auto oldMode = ::SetErrorMode(SEM_FAILCRITICALERRORS);
			static_cast<void>(::GetLastError());
			auto symbol = reinterpret_cast<Signature>(::GetProcAddress(static_cast<HMODULE>(m_library), name.data()));
			auto error = ::GetLastError();
			::SetErrorMode(oldMode);
			if (error != ERROR_SUCCESS && !symbol)
			{
				std::stringstream stream;
				stream << "Couldn't load function [" << name.data() << "]: ";
				stream << "Error code: " << std::hex << error;
				throw std::runtime_error(stream.str());
			}
			return symbol;
		}

	#elif defined(OS_LINUX) || defined(OS_MACOS)

		template<typename Signature>
		Signature getSymbol(std::string_view name)
		{
			dlerror();
			auto symbol = reinterpret_cast<Signature>(dlsym(m_library, name.data()));
			auto error = dlerror();
			if (error && !symbol)
			{
				std::stringstream stream;
				stream << "Couldn't load symbol [" << name.data() << "]: ";
				stream << "Error code: " << std::hex << std::string(error);
				throw std::runtime_error(stream.str());
			}
			return symbol;
		}

	#endif

	private:
		void open(std::string_view path);
		void close() noexcept;

	#if defined(OS_WINDOWS)
		HMODULE m_library = nullptr;
	#else
		void* m_library = nullptr;
	#endif
	};
}
