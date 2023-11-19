#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::FileSystem::File::String
{
	TEST_GROUP("FileSystem/File/String")
	{
		using Logger = CppUtils::Logger<"CppUtils">;
		
		addTest("WriteRead", [] {
			const auto filePath = std::filesystem::path{"test.tmp"};
			const auto originalString = "Hello world!";
			CppUtils::FileSystem::File::String::write(filePath, originalString);
			const auto fileContent = CppUtils::FileSystem::File::String::read(filePath);
			Logger::print("{}", fileContent);
			CppUtils::FileSystem::File::deleteFile(filePath);
			EXPECT(fileContent == originalString);
		});

		addTest("Append", [] {
			const auto filePath = std::filesystem::path{"test.tmp"};
			const auto firstString = "Hello ";
			const auto secondString = "world!";
			CppUtils::FileSystem::File::String::append(filePath, firstString);
			CppUtils::FileSystem::File::String::append(filePath, secondString);
			const auto fileContent = CppUtils::FileSystem::File::String::read(filePath);
			Logger::print("{}", fileContent);
			CppUtils::FileSystem::File::deleteFile(filePath);
			EXPECT(fileContent == std::string{firstString} + secondString);
		});
	}
}
