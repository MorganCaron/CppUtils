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
			suite.expectEqual(std::size(stack), 1uz);
		});

		suite.addTest("One type, multiple values", [&] {
			auto stack = CppUtils::Container::Stack<int>{1, 2, 3};
			suite.expectEqual(std::size(stack), 3uz);
		});

		suite.addTest("Multiple types", [&] {
			auto stack = CppUtils::Container::Stack<bool, int, float, std::string_view>{true, 42, 3.14f, "Hello World!"sv};
			
			suite.expectEqual(std::size(stack), 4uz);
			
			Logger::print("{}\n", stack.get<bool>(0));
			Logger::print("{}\n", stack.get<int>(1));
			Logger::print("{}\n", stack.get<float>(2));
			Logger::print("{}\n", stack.get<std::string_view>(3));
		});

		suite.addTest("push", [&] {
			auto stack = CppUtils::Container::Stack<int>{};
			
			suite.expect(std::empty(stack));
			suite.expectEqual(std::size(stack), 0uz);

			stack.push(42);

			suite.expect(!std::empty(stack));
			suite.expectEqual(std::size(stack), 1uz);
		});

		suite.addTest("get", [&] {
			auto stack = CppUtils::Container::Stack<int>{42};
			
			suite.expect(!std::empty(stack));
			suite.expectEqual(std::size(stack), 1uz);
			
			suite.expectEqual(stack.get<int>(0), 42);

			suite.expect(!std::empty(stack));
			suite.expectEqual(std::size(stack), 1uz);
		});

		suite.addTest("top", [&] {
			auto stack = CppUtils::Container::Stack<int>{42};

			suite.expect(!std::empty(stack));
			suite.expectEqual(std::size(stack), 1uz);

			suite.expectEqual(stack.top<int>(), 42);

			suite.expect(!std::empty(stack));
			suite.expectEqual(std::size(stack), 1uz);
		});

		suite.addTest("pop", [&] {
			auto stack = CppUtils::Container::Stack<int>{42};

			suite.expect(!std::empty(stack));
			suite.expectEqual(std::size(stack), 1uz);

			suite.expectEqual(stack.pop<int>(), 42);

			suite.expect(std::empty(stack));
			suite.expectEqual(std::size(stack), 0uz);
		});

		suite.addTest("copy", [&] {
			auto stack = CppUtils::Container::Stack<int>{0, 42};
			
			stack.copy<int>(1, 0);
			stack.drop<int>();

			suite.expectEqual(stack.get<int>(0), 42);
		});

		suite.addTest("visit", [&] {
			auto stack = CppUtils::Container::Stack<bool, int, float, std::string_view>{true, 42, 3.14f, "Hello World!"sv};

			for (auto i = 0uz; i < std::size(stack); ++i)
				stack.visit(i, [](auto&& value) -> void {
					Logger::print("{}\n", value);
				});
			
			suite.expectEqual(std::size(stack), 4uz);
		});

	}};
}
