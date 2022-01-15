#include <CppUtils/External/SharedLibrary.hpp>

namespace CppUtils::External
{
#if defined(OS_WINDOWS)

	void SharedLibrary::open(std::string_view path)
	{
		m_library = ::LoadLibraryA(path.data());
		if (!m_library)
		{
			std::stringstream stream;
			stream << "Couldn't load dynamic library at [" << path.data() << "]: ";
			stream << "Error code: " << std::hex << GetLastError();
			throw std::runtime_error(stream.str());
		}
	}

	void SharedLibrary::close() noexcept
	{
		if (!m_library)
			return;
		UINT oldMode = ::SetErrorMode(SEM_FAILCRITICALERRORS);

		try
		{
			::FreeLibrary(static_cast<HMODULE>(m_library));
		}
		catch (const std::exception&)
		{
			std::cerr << "Couldn't unload dynamic library" << std::endl;
		}
		::SetErrorMode(oldMode);
		m_library = nullptr;
	}

#elif defined(OS_LINUX) || defined(OS_MACOS)

	void SharedLibrary::open(std::string_view path)
	{
		m_library = dlopen(path.data(), RTLD_LAZY);

		if (!m_library)
		{
			std::stringstream stream;
			stream << "Couldn't load dynamic library at [" << path.data() << "]: ";
			stream << "Error code: " << std::hex << dlerror();
			throw std::runtime_error(stream.str());
		}
	}

	void SharedLibrary::close() noexcept
	{
		try
		{
			dlclose(m_library);
		}
		catch (const std::exception&)
		{
			std::cerr << "Couldn't unload dynamic library" << std::endl;
		}
	}

#endif
}
