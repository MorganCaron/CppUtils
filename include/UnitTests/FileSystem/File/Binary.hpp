#pragma once

#include <CppUtils.hpp>

namespace UnitTests::FileSystem::File::Binary
{
	const auto tests = std::vector<CppUtils::Test::UnitTest>{

		CppUtils::Test::UnitTest("FileSystem/File/Binary/WriteRead", [] {
			const auto filePath = std::filesystem::path{"test.tmp"};
			const auto originalString = 12345;
			CppUtils::FileSystem::File::Binary::write(filePath, originalString);
			const auto fileContent = CppUtils::FileSystem::File::Binary::read<decltype(originalString)>(filePath);
			CppUtils::Logger::logInformation(std::to_string(fileContent));
			CppUtils::FileSystem::File::deleteFile(filePath);
			ASSERT(fileContent == originalString);
		})

	};
}
