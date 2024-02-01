#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTest::FileSystem::FileWatcher
{
	auto _ = TestSuite{"FileSystem/FileWatcher", [](auto& suite) {
		using namespace std::chrono_literals;
		using Logger = CppUtils::Logger<"CppUtils">;

		suite.addTest("No file", [&] {
			CppUtils::FileSystem::TemporaryDirectory{[&suite](const auto& directory) -> void {
				auto fileWatcher = CppUtils::FileSystem::FileWatcher{10ms};
				fileWatcher.watchFile(directory / "test.tmp", [&suite](const std::filesystem::path& filePath, [[maybe_unused]] CppUtils::FileSystem::FileStatus status) -> void {
					Logger::print("{}\n", filePath.string());
					suite.expect(false);
				});
				std::this_thread::sleep_for(100ms);
			}};
		});

		suite.addTest("No event", [&] {
			CppUtils::FileSystem::TemporaryDirectory{[&suite](const auto& directory) -> void {
				auto fileWatcher = CppUtils::FileSystem::FileWatcher{10ms};
				auto filePath = directory / "test.tmp";
				CppUtils::FileSystem::String::write(filePath, "Hello World!");
				fileWatcher.watchFile(filePath, [&suite](const std::filesystem::path& filePath, [[maybe_unused]] CppUtils::FileSystem::FileStatus status) -> void {
					Logger::print("{}\n", filePath.string());
					suite.expect(false);
				});
				std::this_thread::sleep_for(100ms);
			}};
		});

		suite.addTest("Watch file creation", [&] {
			CppUtils::FileSystem::TemporaryDirectory{[&suite](const auto& directory) -> void {
				auto eventsTriggered = 0uz;
				auto fileWatcher = CppUtils::FileSystem::FileWatcher{10ms};
				auto filePath = directory / "test.tmp";

				fileWatcher.watchFile(filePath, [&eventsTriggered, &suite](const std::filesystem::path& filePath, CppUtils::FileSystem::FileStatus status) -> void {
					++eventsTriggered;
					Logger::print("{}\n", filePath.string());
					suite.expectEqual(status, CppUtils::FileSystem::FileStatus::Created);
				});

				CppUtils::FileSystem::String::write(filePath, "Hello World!");
				std::this_thread::sleep_for(100ms);

				suite.expectEqual(eventsTriggered, 1uz);
			}};
		});

		suite.addTest("Watch file modification", [&] {
			CppUtils::FileSystem::TemporaryDirectory{[&suite](const auto& directory) -> void {
				auto eventsTriggered = 0uz;
				auto fileWatcher = CppUtils::FileSystem::FileWatcher{10ms};
				auto filePath = directory / "test.tmp";
				CppUtils::FileSystem::String::write(filePath, "Foo");

				fileWatcher.watchFile(filePath, [&eventsTriggered, &suite](const std::filesystem::path& filePath, CppUtils::FileSystem::FileStatus status) -> void {
					++eventsTriggered;
					Logger::print("{}\n", filePath.string());
					suite.expectEqual(status, CppUtils::FileSystem::FileStatus::Modified);
				});

				std::this_thread::sleep_for(100ms);
				CppUtils::FileSystem::String::write(filePath, "Bar");
				std::this_thread::sleep_for(100ms);

				suite.expectEqual(eventsTriggered, 1uz);
			}};
		});

		suite.addTest("Watch file deletion", [&] {
			CppUtils::FileSystem::TemporaryDirectory{[&suite](const auto& directory) -> void {
				auto eventsTriggered = 0uz;
				auto fileWatcher = CppUtils::FileSystem::FileWatcher{10ms};
				auto filePath = directory / "test.tmp";
				CppUtils::FileSystem::String::write(filePath, "Hello World!");

				fileWatcher.watchFile(filePath, [&eventsTriggered, &suite](const std::filesystem::path& filePath, CppUtils::FileSystem::FileStatus status) -> void {
					++eventsTriggered;
					Logger::print("{}\n", filePath.string());
					suite.expectEqual(status, CppUtils::FileSystem::FileStatus::Deleted);
				});

				std::filesystem::remove(filePath);
				std::this_thread::sleep_for(100ms);

				suite.expectEqual(eventsTriggered, 1uz);
			}};
		});

		suite.addTest("Watch directory creation", [&] {
			CppUtils::FileSystem::TemporaryDirectory{[&suite](const auto& directory) -> void {
				auto eventsTriggered = 0uz;
				auto fileWatcher = CppUtils::FileSystem::FileWatcher{10ms};

				fileWatcher.watchDirectory(directory, [&eventsTriggered, &suite](const std::filesystem::path& filePath, CppUtils::FileSystem::FileStatus status) -> void {
					++eventsTriggered;
					Logger::print("{}\n", filePath.string());
					suite.expectEqual(status, CppUtils::FileSystem::FileStatus::Created);
				});

				std::this_thread::sleep_for(100ms);
				CppUtils::FileSystem::String::write(directory / "test.tmp", "Hello World!");
				std::this_thread::sleep_for(100ms);

				suite.expectEqual(eventsTriggered, 1uz);
			}};
		});

		suite.addTest("Watch directory modification", [&] {
			CppUtils::FileSystem::TemporaryDirectory{[&suite](const auto& directory) -> void {
				auto eventsTriggered = 0uz;
				auto fileWatcher = CppUtils::FileSystem::FileWatcher{10ms};
				auto filePath = directory / "test.tmp";
				CppUtils::FileSystem::String::write(filePath, "Foo");

				fileWatcher.watchDirectory(directory, [&eventsTriggered, &suite](const std::filesystem::path& filePath, CppUtils::FileSystem::FileStatus status) -> void {
					++eventsTriggered;
					Logger::print("{}\n", filePath.string());
					suite.expectEqual(status, CppUtils::FileSystem::FileStatus::Modified);
				});

				std::this_thread::sleep_for(100ms);
				CppUtils::FileSystem::String::write(filePath, "Bar");
				std::this_thread::sleep_for(100ms);

				suite.expectEqual(eventsTriggered, 1uz);
			}};
		});

		suite.addTest("Watch directory deletion", [&] {
			CppUtils::FileSystem::TemporaryDirectory{[&suite](const auto& directory) -> void {
				auto eventsTriggered = 0uz;
				auto fileWatcher = CppUtils::FileSystem::FileWatcher{10ms};
				auto filePath = directory / "test.tmp";
				CppUtils::FileSystem::String::write(filePath, "Hello World!");

				fileWatcher.watchDirectory(directory, [&eventsTriggered, &suite](const std::filesystem::path& filePath, CppUtils::FileSystem::FileStatus status) -> void {
					++eventsTriggered;
					Logger::print("{}\n", filePath.string());
					suite.expectEqual(status, CppUtils::FileSystem::FileStatus::Deleted);
				});

				std::this_thread::sleep_for(100ms);
				std::filesystem::remove(filePath);
				std::this_thread::sleep_for(100ms);

				suite.expectEqual(eventsTriggered, 1uz);
			}};
		});
	}};
}
