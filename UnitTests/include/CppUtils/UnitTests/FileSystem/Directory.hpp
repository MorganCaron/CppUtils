#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTest::FileSystem::Directory
{
	auto _ = TestSuite{"FileSystem/Directory", [](auto& suite) {
		using Logger = CppUtils::Logger<"CppUtils">;

		suite.addTest("TemporaryDirectory", [&] {
			auto functionCalled = false;
			CppUtils::FileSystem::TemporaryDirectory{[&functionCalled, &suite](const auto& directory) -> void {
				functionCalled = true;
				suite.expect(std::filesystem::exists(directory));
				Logger::print("{}\n", directory.string());
			}};
			suite.expect(functionCalled);
		});
	}};
}
