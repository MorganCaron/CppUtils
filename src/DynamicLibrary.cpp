#include "DynamicLibrary.hpp"

#ifdef _WIN32

void DynamicLibrary::open(std::string_view path)
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

void DynamicLibrary::close() noexcept
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

#elif defined(__linux__)

void DynamicLibrary::open()
{
	m_library = dlopen(m_path.c_str(), RTLD_LAZY);

	if (!m_library)
	{
		std::stringstream stream;
		stream << "Couldn't load dynamic library at [" << m_path << "]: ";
		stream << "Error code: " << std::hex << dlerror();
		std::cerr << stream.str() << std::endl;
		throw std::runtime_error(stream.str());
	}
	}

void DynamicLibrary::close() noexcept
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
