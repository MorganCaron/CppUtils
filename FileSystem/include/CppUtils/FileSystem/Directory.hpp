#pragma once

#include <filesystem>
#include <string_view>

#include <CppUtils/Type/Concept.hpp>

namespace CppUtils::FileSystem
{
	class TemporaryDirectory final
	{
	public:
		TemporaryDirectory(std::string_view directoryName = "temp")
		{
			auto nbTemporaryFiles = 0u;
			do
			{
				m_tempDirectory = std::filesystem::temp_directory_path() / std::format("{}{}", directoryName, nbTemporaryFiles++);
			}
			while (std::filesystem::exists(m_tempDirectory));
			std::filesystem::create_directory(m_tempDirectory);
		}

		TemporaryDirectory(auto&& function, auto&&... args):
			TemporaryDirectory{}
		{
			static_assert(not Type::Concept::HasReturnValue<decltype(function)>);
			function(m_tempDirectory, std::forward<decltype(args)>(args)...);
		}

		TemporaryDirectory(std::string_view directoryName, auto&& function, auto&&... args):
			TemporaryDirectory{directoryName}
		{
			static_assert(not Type::Concept::HasReturnValue<decltype(function)>);
			function(m_tempDirectory, std::forward<decltype(args)>(args)...);
		}

		~TemporaryDirectory()
		{
			std::filesystem::remove_all(m_tempDirectory);
		}

		auto execute(auto&& function, auto&&... args) -> decltype(auto)
		{
			if constexpr (Type::Concept::HasReturnValue<decltype(function)>)
				return function(m_tempDirectory, std::forward<decltype(args)>(args)...);
			else
				function(m_tempDirectory, std::forward<decltype(args)>(args)...);
		}

	private:
		std::filesystem::path m_tempDirectory;
	};
}
