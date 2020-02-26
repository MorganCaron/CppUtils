#pragma once

#include <string>
#include <sstream>
#include <utility>
#include <iostream>

#ifdef _WIN32
# include <windows.h>
#elif defined(__linux__)
# include <dlfcn.h>
#endif

#include "DllExport.hpp"

namespace CppUtils::External
{
	class DLL_PUBLIC DynamicLibrary
	{
	public:
	#ifdef _WIN32
		static constexpr const char* ext = ".dll";
	#elif defined(__linux__)
		static constexpr const char* ext = ".so";
	#endif

		DynamicLibrary() = default;
		DynamicLibrary(const DynamicLibrary&) = delete;
		DynamicLibrary(DynamicLibrary&& src) noexcept : m_library(std::exchange(src.m_library, nullptr))
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
		virtual ~DynamicLibrary() noexcept
		{
			close();
		}

	#ifdef _WIN32

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

	#elif defined(__linux__)

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

	#ifdef _WIN32
		HMODULE m_library = nullptr;
	#elif defined(__linux__)
		void* m_library = nullptr;
	#endif
	};
}
