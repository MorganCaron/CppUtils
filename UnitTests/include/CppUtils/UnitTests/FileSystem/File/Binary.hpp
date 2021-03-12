#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::FileSystem::File::Binary
{
	TEST_GROUP("FileSystem/File/Binary")
	{
		addTest("WriteRead", [] {
			const auto filePath = std::filesystem::path{"test.tmp"};
			const auto originalString = 12345;
			CppUtils::FileSystem::File::Binary::write(filePath, originalString);
			const auto fileContent = CppUtils::FileSystem::File::Binary::read<decltype(originalString)>(filePath);
			CppUtils::Log::Logger::logInformation(std::to_string(fileContent));
			CppUtils::FileSystem::File::deleteFile(filePath);
			ASSERT(fileContent == originalString);
		});

		addTest("WriteReadVector", [] {
			const auto filePath = std::filesystem::path{"test.tmp"};
			const auto originalVector = std::vector<int>{ 1, 2, 3, 4, 5 };
			CppUtils::FileSystem::File::Binary::writeVector(filePath, originalVector);
			const auto vector = CppUtils::FileSystem::File::Binary::readVector<int>(filePath);
			for (const auto& nb : vector)
				CppUtils::Log::Logger::logInformation(std::to_string(nb), false);
			CppUtils::Log::Logger::logInformation("");
			CppUtils::FileSystem::File::deleteFile(filePath);
			ASSERT(vector == originalVector);
		});
	}
}
