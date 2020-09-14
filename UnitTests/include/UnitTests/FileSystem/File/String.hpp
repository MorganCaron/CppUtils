#pragma once

#include <CppUtils.hpp>

namespace UnitTests::FileSystem::File::String
{
	const auto tests = std::vector<CppUtils::Test::UnitTest>{

		CppUtils::Test::UnitTest("FileSystem/File/String/WriteRead", [] {
			const auto filePath = std::filesystem::path{"test.tmp"};
			const auto originalString = "Hello world!";
			CppUtils::FileSystem::File::String::write(filePath, originalString);
			const auto fileContent = CppUtils::FileSystem::File::String::read(filePath);
			CppUtils::Log::Logger::logInformation(fileContent);
			CppUtils::FileSystem::File::deleteFile(filePath);
			ASSERT(fileContent == originalString);
		}),

		CppUtils::Test::UnitTest("FileSystem/File/String/Append", [] {
			const auto filePath = std::filesystem::path{"test.tmp"};
			const auto firstString = "Hello ";
			const auto secondString = "world!";
			CppUtils::FileSystem::File::String::append(filePath, firstString);
			CppUtils::FileSystem::File::String::append(filePath, secondString);
			const auto fileContent = CppUtils::FileSystem::File::String::read(filePath);
			CppUtils::Log::Logger::logInformation(fileContent);
			CppUtils::FileSystem::File::deleteFile(filePath);
			ASSERT(fileContent == std::string{firstString} + secondString);
		})

	};
}
