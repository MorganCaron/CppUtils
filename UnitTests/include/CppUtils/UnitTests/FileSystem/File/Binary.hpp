#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTest::FileSystem::File::Binary
{
	auto _ = TestSuite{"FileSystem/File/Binary", [](auto& suite) {
		using Logger = CppUtils::Logger<"CppUtils">;

		suite.addTest("WriteRead", [&] {
			auto filePath = std::filesystem::path{"test.tmp"};
			constexpr auto originalValue = 12345;
			CppUtils::FileSystem::File::Binary::write(filePath, originalValue);
			auto fileContent = CppUtils::FileSystem::File::Binary::read<decltype(originalValue)>(filePath);
			Logger::print("{}\n", fileContent);
			CppUtils::FileSystem::File::deleteFile(filePath);
			suite.expect(fileContent == originalValue);
		});

		suite.addTest("WriteReadVector", [&] {
			auto filePath = std::filesystem::path{"test.tmp"};
			auto originalVector = std::vector<int>{};
			originalVector.resize(10);
			std::iota(originalVector.begin(), originalVector.end(), 1);
			CppUtils::FileSystem::File::Binary::writeVector(filePath, originalVector);
			auto vector = CppUtils::FileSystem::File::Binary::readVector<int>(filePath);
			for (auto nb : vector)
				Logger::print("{}\n", nb);
			CppUtils::FileSystem::File::deleteFile(filePath);
			suite.expect(vector == originalVector);
		});

	}};
}
