#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTest::Language::VirtualMachine
{
	auto _ = TestSuite{"Language/VirtualMachine", [](auto& suite) {
		using namespace std::literals;
		using Logger = CppUtils::Logger<"CppUtils">;
		namespace VM = CppUtils::Language::VirtualMachine;

		suite.addTest("empty source", [&] {
			constexpr auto source = u8""sv;
			constexpr auto result = VM::execute<int>(source);
			suite.expectEqual(result, 0);
		});

		suite.addTest("return value (int)", [&] {
			constexpr auto source = u8"42"sv;
			constexpr auto result = VM::execute<int>(source);
			suite.expectEqual(result, 42);
		});

		suite.addTest("return value (char8_t)", [&] {
			constexpr auto source = u8R"(\A)"sv;
			constexpr auto result = VM::execute<char8_t>(source);
			suite.expectEqual(result, 'A');
		});

		suite.addTest("reset value", [&] {
			constexpr auto source = u8"42_"sv;
			constexpr auto result = VM::execute<int>(source);
			suite.expectEqual(result, 0);
		});

		suite.addTest("write stack info", [&] {
			constexpr auto source = u8"42, 0W"sv;
			auto result = VM::execute<int, std::size_t>(source);
			suite.expectEqual(result, 42);
		});

		suite.addTest("inspect stack", [&] {
			constexpr auto source = u8"42, 1, 1;, 2; I)))"sv;
			constexpr auto input = "Hello World!"sv;
			auto result = VM::execute<int, std::size_t, std::string_view, const std::string_view*>(source, input, &input);
			suite.expectEqual(result, 42);
		});

		suite.addTest("exit with error code", [&] {
			constexpr auto source = u8"0, 0:21X_"sv;
			constexpr auto result = VM::execute<int, std::size_t>(source);
			suite.expectEqual(result, 21);
		});

		suite.addTest("addition", [&] {
			constexpr auto source = u8"40, 0:2+"sv;
			constexpr auto result = VM::execute<int, std::size_t>(source);
			suite.expectEqual(result, 42);
		});

		suite.addTest("substraction", [&] {
			constexpr auto source = u8"44, 0:2-"sv;
			constexpr auto result = VM::execute<int, std::size_t>(source);
			suite.expectEqual(result, 42);
		});

		suite.addTest("multiplication", [&] {
			constexpr auto source = u8"21, 0:2*"sv;
			constexpr auto result = VM::execute<int, std::size_t>(source);
			suite.expectEqual(result, 42);
		});

		suite.addTest("division", [&] {
			constexpr auto source = u8"84, 0:2/"sv;
			constexpr auto result = VM::execute<int, std::size_t>(source);
			suite.expectEqual(result, 42);
		});

		suite.addTest("not", [&] {
			{
				constexpr auto source = u8"0!"sv;
				constexpr auto result = VM::execute<bool>(source);
				suite.expectEqual(result, 1);
			}
			{
				constexpr auto source = u8"1!"sv;
				constexpr auto result = VM::execute<bool>(source);
				suite.expectEqual(result, 0);
			}
		});

		suite.addTest("equal", [&] {
			{
				constexpr auto source = u8"0, 2:40, 2:2+, 2:42="sv;
				constexpr auto result = VM::execute<bool, std::size_t, int>(source);
				suite.expect(result);
			}
			{
				constexpr auto source = u8"0, 2:0, 2:42="sv;
				constexpr auto result = VM::execute<bool, std::size_t, int>(source);
				suite.expect(!result);
			}
		});

		suite.addTest("inferior", [&] {
			{
				constexpr auto source = u8"0, 2:21, 2:42<"sv;
				constexpr auto result = VM::execute<bool, std::size_t, int>(source);
				suite.expect(result);
			}
			{
				constexpr auto source = u8"0, 2:42, 2:21<"sv;
				constexpr auto result = VM::execute<bool, std::size_t, int>(source);
				suite.expect(!result);
			}
		});

		suite.addTest("superior", [&] {
			{
				constexpr auto source = u8"0, 2:21, 2:42>"sv;
				constexpr auto result = VM::execute<bool, std::size_t, int>(source);
				suite.expect(!result);
			}
			{
				constexpr auto source = u8"0, 2:42, 2:21>"sv;
				constexpr auto result = VM::execute<bool, std::size_t, int>(source);
				suite.expect(result);
			}
		});

		suite.addTest("and", [&] {
			{
				constexpr auto source = u8"1, 0:1&"sv;
				constexpr auto result = VM::execute<bool, std::size_t>(source);
				suite.expect(result);
			}
			{
				constexpr auto source = u8"0, 0:1&"sv;
				constexpr auto result = VM::execute<bool, std::size_t>(source);
				suite.expect(!result);
			}
		});

		suite.addTest("or", [&] {
			{
				constexpr auto source = u8"0, 0:1|"sv;
				constexpr auto result = VM::execute<bool, std::size_t>(source);
				suite.expect(result);
			}
			{
				constexpr auto source = u8"0, 0:0|"sv;
				constexpr auto result = VM::execute<bool, std::size_t>(source);
				suite.expect(!result);
			}
		});

		suite.addTest("xor", [&] {
			{
				constexpr auto source = u8"0, 0:1^"sv;
				constexpr auto result = VM::execute<bool, std::size_t>(source);
				suite.expect(result);
			}
			{
				constexpr auto source = u8"1, 0:1^"sv;
				constexpr auto result = VM::execute<bool, std::size_t>(source);
				suite.expect(!result);
			}
		});

		suite.addTest("external prvalue", [&] {
			constexpr auto source = u8"0, 1;"sv;
			constexpr auto result = VM::execute<int, std::size_t>(source, 42);
			suite.expectEqual(result, 42);
		});

		suite.addTest("external lvalue", [&] {
			constexpr auto source = u8"0, 1;"sv;
			constexpr auto number = 42;
			constexpr auto result = VM::execute<int, std::size_t>(source, number);
			suite.expectEqual(result, number);
		});

		suite.addTest("external pointer", [&] {
			constexpr auto source = u8"0, 1;R"sv;
			constexpr auto number = 42;
			auto result = VM::execute<int, std::size_t, const int*>(source, &number);
			suite.expectEqual(result, number);
		});

		suite.addTest("external lambda", [&] {
			constexpr auto source = u8"0, 1;"sv;
			constexpr auto function = []() -> int { return 42; };
			constexpr auto result = VM::execute<int, std::size_t>(source, +function);
			suite.expectEqual(result, function());
		});

		suite.addTest("external function", [&] {
			constexpr auto source = u8"0, 1;, 2;"sv;
			constexpr auto input = u8"Hello World!"sv;
			auto result = VM::execute<std::size_t, std::u8string_view>(source, input, std::size<decltype(input)>);
			suite.expectEqual(result, std::size(input));
		});

		suite.addTest("external member function", [&] {
			constexpr auto source = u8"0, 1;, 2;)"sv;
			constexpr auto input = u8"Hello World!"sv;
			auto result = VM::execute<std::size_t, const std::u8string_view*>(source, &input, &std::u8string_view::size);
			suite.expectEqual(result, std::size(input));
		});

		suite.addTest("condition", [&] {
			{
				constexpr auto source = u8"0, 1, 4? 42X 21"sv;
				constexpr auto result = VM::execute<int, std::size_t>(source);
				suite.expectEqual(result, 42);
			}
			{
				constexpr auto source = u8"0, 0, 4? 21X 42"sv;
				constexpr auto result = VM::execute<int, std::size_t>(source);
				suite.expectEqual(result, 42);
			}
		});

		suite.addTest("char", [&] {
			constexpr auto source = u8R"(0, 2:\A, 1;)"sv;
			constexpr auto function = [](char8_t c) -> bool {
				return c == 'A';
			};
			constexpr auto result = VM::execute<bool, std::size_t, char8_t>(source, +function);
			suite.expect(result);
		});

		suite.addTest("position", [&] {
			constexpr auto source = u8"   P   "sv;
			constexpr auto result = VM::execute<std::size_t>(source);
			suite.expectEqual(result, 3uz);
		});

		suite.addTest("direct jump", [&] {
			constexpr auto source = u8"0, 1:0, 11JX 42"sv;
			constexpr auto result = VM::execute<std::size_t, bool>(source);
			suite.expectEqual(result, 42uz);
		});

		suite.addTest("indirect jump", [&] {
			constexpr auto source = u8"11, 1:1, 0JX_42"sv;
			auto result = VM::execute<std::size_t, bool>(source);
			suite.expectEqual(result, 42uz);
		});

		suite.addTest("copy", [&] {
			constexpr auto source = u8"21, 0:0, 1, 0C+"sv;
			constexpr auto result = VM::execute<std::size_t>(source);
			suite.expectEqual(result, 42uz);
		});

		suite.addTest("cast", [&] {
			constexpr auto source = u8"0, 1:42, 0, 1C)"sv;
			constexpr auto result = VM::execute<bool, std::size_t>(source);
			suite.expect(result);
		});

		suite.addTest("return string.at", [&] {
			constexpr auto source = u8"0, 1;, 6, 2;)"sv;
			constexpr auto input = u8"Hello World!"sv;
			auto result = VM::execute<char8_t, std::size_t, const std::u8string_view*>(source, &input, &std::u8string_view::at);
			suite.expectEqual(result, 'W');
		});

		suite.addTest("compare string.at", [&] {
			constexpr auto source = u8R"(0, 2:0, 1;, 1:6, 2;), 2:\W =)"sv;
			constexpr auto input = u8"Hello World!"sv;
			auto result = VM::execute<bool, std::size_t, char8_t, const std::u8string_view*>(source, &input, &std::u8string_view::at);
			suite.expect(result);
		});

		suite.addTest("print string_view", [&] {
			constexpr auto source = u8R"(
				0, 1;, 2;
			)"sv;

			constexpr auto input = "Hello World!"sv;

			constexpr auto print = [](std::string_view text) -> void {
				Logger::print("{}\n", text);
			};

			auto result = VM::execute<std::size_t, std::string_view>(source, input, +print);
			suite.expectEqual(result, 0uz);
		});

		suite.addTest("test loops", [&] {
			constexpr auto source = u8R"(
				1;, 1+ get nb plus one
				P position for loop
				(0, 2, 0C copy nb
				(1- minus one
				(0, 2C copy nb
				(0=, 4? (0X quit if nb equal zero
				(2; get input
				(3; execute print
				(1:1, 0J goto position for loop
			)"sv;

			constexpr auto nb = std::size_t{3};

			constexpr auto input = "Hello World!"sv;

			constexpr auto print = [](std::string_view text) -> void {
				Logger::print("{}\n", text);
			};

			auto result = VM::execute<std::size_t, bool, std::string_view>(source, nb, input, +print);
			suite.expectEqual(result, 0uz);
		});

		suite.addTest("compile labels", [&] {
			constexpr auto source = u8R"(
				
			)"sv;
			constexpr auto input = u8R"(Hello World!)"sv;
			auto output = ""s;
			auto result = VM::execute<std::size_t, std::u8string_view, std::string*>(source, input, &output, std::size<decltype(input)>, &std::u8string_view::at);
			suite.expectEqual(result, 0uz);
		});

		suite.addTest("wip 2", [&] {
			constexpr auto source = u8"0, 0, 1;, 6, 2;), 3;"sv;
			constexpr auto input = u8"Hello World!"sv;
			constexpr auto compare = [](char8_t c) -> std::intptr_t {
				std::wcout << static_cast<wchar_t>(c) << std::endl;
				return c == 'W';
			};
			auto result = VM::execute<std::intptr_t, bool, char8_t, std::size_t, const std::u8string_view*>(source, &input, &std::u8string_view::at, +compare);
			suite.expect(result);
		});

	}};
}
