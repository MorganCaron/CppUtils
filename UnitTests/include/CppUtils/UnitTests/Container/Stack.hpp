#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTest::Container::Stack
{
	auto _ = TestSuite{"Container/Stack", [](auto& suite) {
		using namespace std::literals;
		using Logger = CppUtils::Logger<"CppUtils">;

		suite.addTest("Empty Stack", [&] {
			auto stack = CppUtils::Container::Stack<int>{};
			suite.expect(std::empty(stack));
		});

		suite.addTest("One value", [&] {
			auto stack = CppUtils::Container::Stack<int>{42};
			suite.expect(!std::empty(stack));
			suite.expectEqual(std::size(stack), 1uz);
		});

		suite.addTest("One type, multiple values", [&] {
			auto stack = CppUtils::Container::Stack<int>{1, 2, 3};
			suite.expectEqual(std::size(stack), 3uz);
		});

		suite.addTest("Multiple types", [&] {
			auto stack = CppUtils::Container::Stack<bool, int, float, std::string_view>{true, 42, 3.14f, "Hello World!"sv};
			suite.expectEqual(std::size(stack), 4uz);
			
			stack.print();

			Logger::print("{}\n", stack.get<bool>(0));
			Logger::print("{}\n", stack.get<int>(1));
			Logger::print("{}\n", stack.get<float>(2));
			Logger::print("{}\n", stack.get<std::string_view>(3));
		});

		suite.addTest("push/top", [&] {
			auto stack = CppUtils::Container::Stack<bool, int, float>{};
			stack.push(true);
			suite.expectEqual(stack.top<bool>(), true);
			stack.push(42);
			suite.expectEqual(stack.top<int>(), 42);
			stack.push(3.14f);
			suite.expectEqual(stack.top<float>(), 3.14f);
		});

		suite.addTest("pushType", [&] {
			auto stack = CppUtils::Container::Stack<bool, int, float>{};
			stack.pushType(1);
			suite.expectEqual(stack.top<int>(), 0);
		});

		suite.addTest("get", [&] {
			auto stack = CppUtils::Container::Stack<int>{42};
			suite.expectEqual(stack.get<int>(0), 42);
		});

		suite.addTest("pop", [&] {
			auto stack = CppUtils::Container::Stack<int>{42};
			suite.expectEqual(stack.pop<int>(), 42);
			suite.expect(std::empty(stack));
		});

		suite.addTest("copy", [&] {
			auto stack = CppUtils::Container::Stack<int>{0, 42};
			stack.copy(1, 0);
			stack.drop();
			suite.expectEqual(stack.get<int>(0), 42);
		});

		suite.addTest("visit", [&] {
			auto stack = CppUtils::Container::Stack<bool, int, float, std::string_view>{true, 42, 3.14f, "Hello World!"sv};

			for (auto i = 0uz; i < std::size(stack); ++i)
				stack.visit(i, [&stack, i](auto&& value) -> void {
					stack.print(i);
					Logger::print("{}\n", value);
				});
			
			suite.expectEqual(std::size(stack), 4uz);
		});

	}};
}
