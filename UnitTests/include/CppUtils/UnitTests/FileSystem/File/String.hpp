#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTest::FileSystem::File::String
{
	auto _ = TestSuite{"FileSystem/File/String", [](auto& suite) {
		using Logger = CppUtils::Logger<"CppUtils">;

		suite.addTest("WriteRead", [&] {
			const auto filePath = std::filesystem::path{"test.tmp"};
			const auto originalString = "Hello world!";
			CppUtils::FileSystem::File::String::write(filePath, originalString);
			const auto fileContent = CppUtils::FileSystem::File::String::read(filePath);
			Logger::print("{}\n", fileContent);
			CppUtils::FileSystem::File::deleteFile(filePath);
			suite.expect(fileContent == originalString);
		});

		suite.addTest("Append", [&] {
			const auto filePath = std::filesystem::path{"test.tmp"};
			const auto firstString = "Hello ";
			const auto secondString = "world!";
			CppUtils::FileSystem::File::String::append(filePath, firstString);
			CppUtils::FileSystem::File::String::append(filePath, secondString);
			const auto fileContent = CppUtils::FileSystem::File::String::read(filePath);
			Logger::print("{}\n", fileContent);
			CppUtils::FileSystem::File::deleteFile(filePath);
			suite.expect(fileContent == std::string{firstString} + secondString);
		});
	}};
}
