#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTest::Language::VirtualMachine
{
	auto _ = TestSuite{"Language/VirtualMachine", [](auto& suite) {
		using namespace std::literals;
		using Logger = CppUtils::Logger<"CppUtils">;
		namespace VM = CppUtils::Language::VirtualMachine;

		suite.addTest("empty source", [&] {
			constexpr auto src = u8""sv;
			constexpr auto result = VM::execute<int>(src);
			suite.expectEqual(result, 0);
		});

		suite.addTest("return value (int)", [&] {
			constexpr auto src = u8"42"sv;
			constexpr auto result = VM::execute<int>(src);
			suite.expectEqual(result, 42);
		});

		suite.addTest("return value (char8_t)", [&] {
			constexpr auto src = u8R"(\A)"sv;
			constexpr auto result = VM::execute<char8_t>(src);
			suite.expectEqual(result, 'A');
		});

		suite.addTest("reset value", [&] {
			constexpr auto src = u8"42_"sv;
			constexpr auto result = VM::execute<int>(src);
			suite.expectEqual(result, 0);
		});

		suite.addTest("write stack info", [&] {
			constexpr auto src = u8"42, 0W"sv;
			auto result = VM::execute<int, std::size_t>(src);
			suite.expectEqual(result, 42);
		});

		suite.addTest("inspect stack", [&] {
			constexpr auto src = u8"42, 1, 1;, 2; I)))"sv;
			constexpr auto input = "Hello World!"sv;
			auto result = VM::execute<int, std::size_t, std::string_view, const std::string_view*>(src, input, &input);
			suite.expectEqual(result, 42);
		});

		suite.addTest("exit with error code", [&] {
			constexpr auto src = u8"0, 0:21X_"sv;
			constexpr auto result = VM::execute<int, std::size_t>(src);
			suite.expectEqual(result, 21);
		});

		suite.addTest("addition", [&] {
			constexpr auto src = u8"40, 0:2+"sv;
			constexpr auto result = VM::execute<int, std::size_t>(src);
			suite.expectEqual(result, 42);
		});

		suite.addTest("substraction", [&] {
			constexpr auto src = u8"44, 0:2-"sv;
			constexpr auto result = VM::execute<int, std::size_t>(src);
			suite.expectEqual(result, 42);
		});

		suite.addTest("multiplication", [&] {
			constexpr auto src = u8"21, 0:2*"sv;
			constexpr auto result = VM::execute<int, std::size_t>(src);
			suite.expectEqual(result, 42);
		});

		suite.addTest("division", [&] {
			constexpr auto src = u8"84, 0:2/"sv;
			constexpr auto result = VM::execute<int, std::size_t>(src);
			suite.expectEqual(result, 42);
		});

		suite.addTest("not", [&] {
			{
				constexpr auto src = u8"0!"sv;
				constexpr auto result = VM::execute<bool>(src);
				suite.expectEqual(result, 1);
			}
			{
				constexpr auto src = u8"1!"sv;
				constexpr auto result = VM::execute<bool>(src);
				suite.expectEqual(result, 0);
			}
		});

		suite.addTest("equal", [&] {
			{
				constexpr auto src = u8"(2:40, 2:2+, 2:42="sv;
				constexpr auto result = VM::execute<bool, std::size_t, int>(src);
				suite.expect(result);
			}
			{
				constexpr auto src = u8"(2:0, 2:42="sv;
				constexpr auto result = VM::execute<bool, std::size_t, int>(src);
				suite.expect(!result);
			}
		});

		suite.addTest("inferior", [&] {
			{
				constexpr auto src = u8"(2:21, 2:42<"sv;
				constexpr auto result = VM::execute<bool, std::size_t, int>(src);
				suite.expect(result);
			}
			{
				constexpr auto src = u8"(2:42, 2:21<"sv;
				constexpr auto result = VM::execute<bool, std::size_t, int>(src);
				suite.expect(!result);
			}
		});

		suite.addTest("superior", [&] {
			{
				constexpr auto src = u8"(2:21, 2:42>"sv;
				constexpr auto result = VM::execute<bool, std::size_t, int>(src);
				suite.expect(!result);
			}
			{
				constexpr auto src = u8"(2:42, 2:21>"sv;
				constexpr auto result = VM::execute<bool, std::size_t, int>(src);
				suite.expect(result);
			}
		});

		suite.addTest("and", [&] {
			{
				constexpr auto src = u8"1, 0:1&"sv;
				constexpr auto result = VM::execute<bool, std::size_t>(src);
				suite.expect(result);
			}
			{
				constexpr auto src = u8"0, 0:1&"sv;
				constexpr auto result = VM::execute<bool, std::size_t>(src);
				suite.expect(!result);
			}
		});

		suite.addTest("or", [&] {
			{
				constexpr auto src = u8"0, 0:1|"sv;
				constexpr auto result = VM::execute<bool, std::size_t>(src);
				suite.expect(result);
			}
			{
				constexpr auto src = u8"0, 0:0|"sv;
				constexpr auto result = VM::execute<bool, std::size_t>(src);
				suite.expect(!result);
			}
		});

		suite.addTest("xor", [&] {
			{
				constexpr auto src = u8"0, 0:1^"sv;
				constexpr auto result = VM::execute<bool, std::size_t>(src);
				suite.expect(result);
			}
			{
				constexpr auto src = u8"1, 0:1^"sv;
				constexpr auto result = VM::execute<bool, std::size_t>(src);
				suite.expect(!result);
			}
		});

		suite.addTest("external prvalue", [&] {
			constexpr auto src = u8"0, 1;"sv;
			constexpr auto result = VM::execute<int, std::size_t>(src, 42);
			suite.expectEqual(result, 42);
		});

		suite.addTest("external lvalue", [&] {
			constexpr auto src = u8"0, 1;"sv;
			constexpr auto number = 42;
			constexpr auto result = VM::execute<int, std::size_t>(src, number);
			suite.expectEqual(result, number);
		});

		suite.addTest("external pointer", [&] {
			constexpr auto src = u8"0, 1;R"sv;
			constexpr auto number = 42;
			auto result = VM::execute<int, std::size_t, const int*>(src, &number);
			suite.expectEqual(result, number);
		});

		suite.addTest("external lambda", [&] {
			constexpr auto src = u8"0, 1;"sv;
			constexpr auto function = []() -> int { return 42; };
			constexpr auto result = VM::execute<int, std::size_t>(src, +function);
			suite.expectEqual(result, function());
		});

		suite.addTest("external function", [&] {
			constexpr auto src = u8"0, 1;, 2;"sv;
			constexpr auto input = u8"Hello World!"sv;
			auto result = VM::execute<std::size_t, std::u8string_view>(src, input, std::size<decltype(input)>);
			suite.expectEqual(result, std::size(input));
		});

		suite.addTest("external member function", [&] {
			constexpr auto src = u8"0, 1;, 2;"sv;
			constexpr auto input = u8"Hello World!"sv;
			auto result = VM::execute<std::size_t, const std::u8string_view*>(src, &input, &std::u8string_view::size);
			suite.expectEqual(result, std::size(input));
		});

		suite.addTest("condition", [&] {
			{
				constexpr auto src = u8"0, 1, 4? 42X 21"sv;
				constexpr auto result = VM::execute<int, std::size_t>(src);
				suite.expectEqual(result, 42);
			}
			{
				constexpr auto src = u8"0, 0, 4? 21X 42"sv;
				constexpr auto result = VM::execute<int, std::size_t>(src);
				suite.expectEqual(result, 42);
			}
		});

		suite.addTest("char", [&] {
			constexpr auto src = u8R"(0, 2:\A, 1;)"sv;
			constexpr auto function = [](char8_t c) -> bool {
				return c == 'A';
			};
			constexpr auto result = VM::execute<bool, std::size_t, char8_t>(src, +function);
			suite.expect(result);
		});

		suite.addTest("position", [&] {
			constexpr auto src = u8"   P   "sv;
			constexpr auto result = VM::execute<std::size_t>(src);
			suite.expectEqual(result, 3uz);
		});

		suite.addTest("jump", [&] {
			constexpr auto src = u8"0, 5JX) 42"sv;
			constexpr auto result = VM::execute<int, std::size_t>(src);
			suite.expectEqual(result, 42);
		});

		suite.addTest("copy", [&] {
			constexpr auto src = u8"21, 0:0, 1, 0C+"sv;
			constexpr auto result = VM::execute<int, std::size_t>(src);
			suite.expectEqual(result, 42);
		});

		suite.addTest("cast", [&] {
			constexpr auto src = u8"0, 1:42, 0, 1C)"sv;
			constexpr auto result = VM::execute<bool, std::size_t>(src);
			suite.expectEqual(result, 1);
		});

		suite.addTest("return string.at", [&] {
			constexpr auto src = u8"0, 1;, 6, 2;"sv;
			constexpr auto input = u8"Hello World!"sv;
			auto result = VM::execute<char8_t, std::size_t, const std::u8string_view*>(src, &input, &std::u8string_view::at);
			suite.expectEqual(result, 'W');
		});

		suite.addTest("compare string.at", [&] {
			constexpr auto src = u8R"(0, 2:0, 1;, 1:6, 2;, 2:\W =)"sv;
			constexpr auto input = u8"Hello World!"sv;
			auto result = VM::execute<bool, std::size_t, char8_t, const std::u8string_view*>(src, &input, &std::u8string_view::at);
			suite.expect(result);
		});

		suite.addTest("print string_view", [&] {
			constexpr auto src = u8R"(
				0, 1;, 2;
			)"sv;

			constexpr auto input = "Hello World!"sv;

			constexpr auto print = [](std::string_view text) -> void {
				Logger::print("{}\n", text);
			};

			auto result = VM::execute<std::size_t, std::string_view>(src, input, +print);
			suite.expectEqual(result, 0uz);
		});

		/*
			_3<_L <_D <_3SP <_4!	_<E
			_3<_R <_167D <_21!	_4<_L <_1SDA _<87J
			_3<_R <_42D <_51!	_4<_60A_95A_49A_83A_66A_95A_82A_60A_95A_83A_66A
			_3<_R <_4SA
			_3<_1J _<1B
		*/

		suite.addTest("test loops", [&] {
			constexpr auto src = u8R"(
				1;, 1+ P,
				0, 2, 0C,
				1-,
				0, 2C,
				0=, 4? (0X,
				2;, 3;
				J
			)"sv;

			constexpr auto nb = std::size_t{3};

			constexpr auto input = "Hello World!"sv;

			constexpr auto print = [](std::string_view text) -> void {
				Logger::print("{}\n", text);
			};

			auto result = VM::execute<std::size_t, bool, std::string_view>(src, nb, input, +print);
			suite.expectEqual(result, 0uz);
		});

		suite.addTest("compile labels", [&] {
			constexpr auto src = u8R"(
				
			)"sv;
			constexpr auto input = u8R"(Hello World!)"sv;
			auto output = ""s;
			auto result = VM::execute<std::size_t, std::u8string_view, std::string*>(src, input, &output, std::size<decltype(input)>, &std::u8string_view::at);
			suite.expectEqual(result, 0uz);
		});

		suite.addTest("wip 2", [&] {
			constexpr auto src = u8"0, 0, 1;, 6, 2;, 3;"sv;
			constexpr auto input = u8"Hello World!"sv;
			constexpr auto compare = [](char8_t c) -> std::intptr_t {
				std::wcout << static_cast<wchar_t>(c) << std::endl;
				return c == 'W';
			};
			auto result = VM::execute<std::intptr_t, bool, char8_t, std::size_t, const std::u8string_view*>(src, &input, &std::u8string_view::at, +compare);
			suite.expect(result);
		});

	}};
}
