#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTest::FileSystem::File
{
	auto _ = TestSuite{"FileSystem/File", [](auto& suite) {
		using Logger = CppUtils::Logger<"CppUtils">;

		suite.addTest("forFiles", [&] {
			CppUtils::FileSystem::TemporaryDirectory{[&suite](const auto& directory) -> void {
				CppUtils::FileSystem::String::write(directory / "file.txt", "test");
				CppUtils::FileSystem::String::write(directory / "file.log", "test");
				auto filesCounter = 0u;
				CppUtils::FileSystem::forFiles(directory, [&filesCounter](const auto& filePath) -> void {
					Logger::print("{}\n", filePath.string());
					++filesCounter;
				});
				suite.expectEqual(filesCounter, 2u);
			}};
		});

		suite.addTest("forFilesWithExtension", [&] {
			CppUtils::FileSystem::TemporaryDirectory{[&suite](const auto& directory) -> void {
				CppUtils::FileSystem::String::write(directory / "file.txt", "test");
				CppUtils::FileSystem::String::write(directory / "file.log", "test");
				auto filesCounter = 0u;
				CppUtils::FileSystem::forFilesWithExtension(directory, ".log", [&filesCounter](const auto& filePath) -> void {
					Logger::print("{}\n", filePath.string());
					++filesCounter;
				});
				suite.expectEqual(filesCounter, 1u);
			}};
		});
	}};

	namespace Binary
	{
		auto _ = TestSuite{"FileSystem/File/Binary", [](auto& suite) {
			using Logger = CppUtils::Logger<"CppUtils">;

			suite.addTest("WriteRead", [&] {
				CppUtils::FileSystem::TemporaryDirectory{[&suite](const auto& directory) -> void {
					auto filePath = directory / "test.tmp";
					auto originalValue = 12345;
					CppUtils::FileSystem::Binary::write(filePath, originalValue);
					auto fileContent = CppUtils::FileSystem::Binary::read<decltype(originalValue)>(filePath);
					Logger::print("{}\n", fileContent);
					suite.expect(fileContent == originalValue);
				}};
			});

			suite.addTest("WriteReadVector", [&] {
				CppUtils::FileSystem::TemporaryDirectory{[&suite](const auto& directory) -> void {
					auto filePath = directory / "test.tmp";
					auto originalVector = std::vector<int>{};
					originalVector.resize(10);
					std::iota(originalVector.begin(), originalVector.end(), 1);
					CppUtils::FileSystem::Binary::writeVector(filePath, originalVector);
					auto vector = CppUtils::FileSystem::Binary::readVector<int>(filePath);
					for (auto nb : vector)
						Logger::print("{}\n", nb);
					suite.expect(vector == originalVector);
				}};
			});
		}};
	}

	namespace String
	{
		auto _ = TestSuite{"FileSystem/File/String", [](auto& suite) {
			using Logger = CppUtils::Logger<"CppUtils">;

			suite.addTest("WriteRead", [&] {
				CppUtils::FileSystem::TemporaryDirectory{[&suite](const auto& directory) -> void {
					auto filePath = directory / "test.tmp";
					auto originalString = "Hello World!";
					CppUtils::FileSystem::String::write(filePath, originalString);
					auto fileContent = CppUtils::FileSystem::String::read(filePath);
					Logger::print("{}\n", fileContent);
					suite.expect(fileContent == originalString);
				}};
			});

			suite.addTest("Append", [&] {
				CppUtils::FileSystem::TemporaryDirectory{[&suite](const auto& directory) -> void {
					auto filePath = directory / "test.tmp";
					auto firstString = "Hello ";
					auto secondString = "World!";
					CppUtils::FileSystem::String::append(filePath, firstString);
					CppUtils::FileSystem::String::append(filePath, secondString);
					auto fileContent = CppUtils::FileSystem::String::read(filePath);
					Logger::print("{}\n", fileContent);
					suite.expect(fileContent == std::string{firstString} + secondString);
				}};
			});
		}};
	}
}
