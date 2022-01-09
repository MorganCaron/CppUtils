#pragma once

#include <CppUtils/Platform/OS.hpp>

#if defined(OS_LINUX)

#	include <unistd.h>
#	include <sys/mman.h>

#	include <string>
#	include <cstring>
#	include <stdexcept>
#	include <string_view>

namespace CppUtils::System
{
	class FileDescriptor final
	{
	public:
		explicit FileDescriptor(int fileDescriptor):
			m_fileDescriptor{fileDescriptor}
		{}
		~FileDescriptor()
		{
			close(m_fileDescriptor);
		}

		[[nodiscard]] constexpr int get() const noexcept
		{
			return m_fileDescriptor;
		}

	private:
		int m_fileDescriptor;
	};

	[[nodiscard]] inline FileDescriptor createAnonymousFile(std::string_view name, unsigned int flags = 0)
	{
		using namespace std::literals;
		auto fileDescriptor = memfd_create(name.data(), flags);
		if (fileDescriptor == -1)
			throw std::runtime_error{"Failed to create an anonymous file:\n"s + std::strerror(errno)};
		return FileDescriptor{fileDescriptor};
	}

	inline void closeFileDescriptor(int fileDescriptor)
	{
		close(fileDescriptor);
	}
}

#endif
