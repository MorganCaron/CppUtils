#pragma once

#include <string>
#include <sstream>
#include <utility>
#include <iostream>
#include <string_view>

#if defined(_WIN32) || defined(_WIN64)
# include <windows.h>
#elif defined(__linux__) || defined(__APPLE__)
# include <dlfcn.h>
#endif

#include <CppUtils/External/DllExport.hpp>

namespace CppUtils::External
{
	using namespace std::literals;
	
	class DLL_PUBLIC DynamicLibrary final
	{
	public:
		#if defined(_WIN32) || defined(_WIN64)
		static constexpr auto ext = ".dll"sv;
		#elif defined(__linux__)
		static constexpr auto ext = ".so"sv;
		#elif defined(__APPLE__)
		static constexpr auto ext = ".dylib"sv;
		#endif

		DynamicLibrary() = default;
		DynamicLibrary(const DynamicLibrary&) = delete;
		DynamicLibrary(DynamicLibrary&& src) noexcept:
			m_library(std::exchange(src.m_library, nullptr))
		{}
		DynamicLibrary& operator=(const DynamicLibrary&) = delete;
		DynamicLibrary& operator=(DynamicLibrary&& rhs) noexcept
		{
			m_library = std::exchange(rhs.m_library, nullptr);
			return *this;
		}

		explicit DynamicLibrary(std::string_view path)
		{
			open(path);
		}
		~DynamicLibrary() noexcept
		{
			close();
		}

		#if defined(_WIN32) || defined(_WIN64)

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

	#else

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

	#if defined(_WIN32) || defined(_WIN64)
		HMODULE m_library = nullptr;
	#else
		void* m_library = nullptr;
	#endif
	};
}
