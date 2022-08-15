#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::FileSystem::File::Binary
{
	TEST_GROUP("FileSystem/File/Binary")
	{
		addTest("WriteRead", [] {
			const auto filePath = std::filesystem::path{"test.tmp"};
			const auto originalValue = 12345;
			CppUtils::FileSystem::File::Binary::write(filePath, originalValue);
			const auto fileContent = CppUtils::FileSystem::File::Binary::read<decltype(originalValue)>(filePath);
			CppUtils::Log::Logger{std::cout} << std::to_string(fileContent) << '\n';
			CppUtils::FileSystem::File::deleteFile(filePath);
			TEST_ASSERT(fileContent == originalValue);
		});

		addTest("WriteReadVector", [] {
			const auto filePath = std::filesystem::path{"test.tmp"};
			auto originalVector = std::vector<int>{};
			originalVector.resize(256);
			std::iota(originalVector.begin(), originalVector.end(), 1);
			CppUtils::FileSystem::File::Binary::writeVector(filePath, originalVector);
			const auto vector = CppUtils::FileSystem::File::Binary::readVector<int>(filePath);
			auto logger = CppUtils::Log::Logger{std::cout};
			for (const auto& nb : vector)
				logger << std::to_string(nb) + ' ';
			logger << '\n';
			CppUtils::FileSystem::File::deleteFile(filePath);
			TEST_ASSERT(vector == originalVector);
		});
	}
}
