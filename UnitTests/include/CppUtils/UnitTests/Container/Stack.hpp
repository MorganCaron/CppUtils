#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTest::Container::Stack
{
	auto _ = TestSuite{"Container/Stack", [](auto& suite) {
		using namespace std::literals;
		using Logger = CppUtils::Logger<"CppUtils">;

		suite.addTest("Empty Stack", [&] {
			auto Stack = CppUtils::Container::Stack<int>{};
			suite.expect(std::empty(Stack));
		});

		suite.addTest("One value", [&] {
			auto Stack = CppUtils::Container::Stack<int>{42};
			suite.expectEqual(std::size(Stack), 1uz);
		});

		suite.addTest("One type, multiple values", [&] {
			auto Stack = CppUtils::Container::Stack<int>{1, 2, 3};
			suite.expectEqual(std::size(Stack), 3uz);
		});

		suite.addTest("Multiple types", [&] {
			auto Stack = CppUtils::Container::Stack<bool, int, float, std::string_view>{true, 42, 3.14f, "Hello World!"sv};
			
			suite.expectEqual(std::size(Stack), 4uz);
			
			Logger::print("{}\n", Stack.get<bool>(0));
			Logger::print("{}\n", Stack.get<int>(1));
			Logger::print("{}\n", Stack.get<float>(2));
			Logger::print("{}\n", Stack.get<std::string_view>(3));
		});

		suite.addTest("push", [&] {
			auto Stack = CppUtils::Container::Stack<int>{};
			
			suite.expect(std::empty(Stack));
			suite.expectEqual(std::size(Stack), 0uz);

			Stack.push(42);

			suite.expect(!std::empty(Stack));
			suite.expectEqual(std::size(Stack), 1uz);
		});

		suite.addTest("get", [&] {
			auto Stack = CppUtils::Container::Stack<int>{42};
			
			suite.expect(!std::empty(Stack));
			suite.expectEqual(std::size(Stack), 1uz);
			
			suite.expectEqual(Stack.get<int>(0), 42);

			suite.expect(!std::empty(Stack));
			suite.expectEqual(std::size(Stack), 1uz);
		});

		suite.addTest("pop", [&] {
			auto Stack = CppUtils::Container::Stack<int>{42};

			suite.expect(!std::empty(Stack));
			suite.expectEqual(std::size(Stack), 1uz);

			suite.expectEqual(Stack.pop<int>(), 42);

			suite.expect(std::empty(Stack));
			suite.expectEqual(std::size(Stack), 0uz);
		});

		suite.addTest("copy", [&] {
			auto Stack = CppUtils::Container::Stack<int>{0, 42};
			
			Stack.copy<int>(1, 0);
			Stack.drop<int>();

			suite.expectEqual(Stack.get<int>(0), 42);
		});

	}};
}
