#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::VirtualMachine
{
	TEST_GROUP("Language/VirtualMachine")
	{
		using namespace std::literals;
		using Logger = CppUtils::Logger<"CppUtils">;
		namespace VM = CppUtils::Language::VirtualMachine;

		addTest("empty source", [] {
			constexpr auto src = u8""sv;
			constexpr auto result = VM::execute<int>(src);
			EXPECT_EQUAL(result, 0);
		});

		addTest("return value (int)", [] {
			constexpr auto src = u8"42"sv;
			constexpr auto result = VM::execute<int>(src);
			EXPECT_EQUAL(result, 42);
		});

		addTest("return value (char8_t)", [] {
			constexpr auto src = u8R"(\A)"sv;
			constexpr auto result = VM::execute<char8_t>(src);
			EXPECT_EQUAL(result, 'A');
		});

		addTest("reset value", [] {
			constexpr auto src = u8"42_"sv;
			constexpr auto result = VM::execute<int>(src);
			EXPECT_EQUAL(result, 0);
		});

		addTest("write stack info", [] {
			constexpr auto src = u8"42, 0W"sv;
			auto result = VM::execute<int, std::size_t>(src);
			EXPECT_EQUAL(result, 42);
		});

		addTest("inspect stack", [] {
			constexpr auto src = u8"42, 1, 1;, 2; I)))"sv;
			constexpr auto input = "Hello World!"sv;
			auto result = VM::execute<int, std::size_t, std::string_view, const std::string_view*>(src, input, &input);
			EXPECT_EQUAL(result, 42);
		});

		addTest("exit with error code", [] {
			constexpr auto src = u8"0, 0:21X_"sv;
			constexpr auto result = VM::execute<int, std::size_t>(src);
			EXPECT_EQUAL(result, 21);
		});

		addTest("addition", [] {
			constexpr auto src = u8"40, 0:2+"sv;
			constexpr auto result = VM::execute<int, std::size_t>(src);
			EXPECT_EQUAL(result, 42);
		});

		addTest("substraction", [] {
			constexpr auto src = u8"44, 0:2-"sv;
			constexpr auto result = VM::execute<int, std::size_t>(src);
			EXPECT_EQUAL(result, 42);
		});

		addTest("multiplication", [] {
			constexpr auto src = u8"21, 0:2*"sv;
			constexpr auto result = VM::execute<int, std::size_t>(src);
			EXPECT_EQUAL(result, 42);
		});

		addTest("division", [] {
			constexpr auto src = u8"84, 0:2/"sv;
			constexpr auto result = VM::execute<int, std::size_t>(src);
			EXPECT_EQUAL(result, 42);
		});

		addTest("not", [] {
			{
				constexpr auto src = u8"0!"sv;
				constexpr auto result = VM::execute<bool>(src);
				EXPECT_EQUAL(result, 1);
			}
			{
				constexpr auto src = u8"1!"sv;
				constexpr auto result = VM::execute<bool>(src);
				EXPECT_EQUAL(result, 0);
			}
		});

		addTest("equal", [] {
			{
				constexpr auto src = u8"(2:40, 2:2+, 2:42="sv;
				constexpr auto result = VM::execute<bool, std::size_t, int>(src);
				EXPECT(result);
			}
			{
				constexpr auto src = u8"(2:0, 2:42="sv;
				constexpr auto result = VM::execute<bool, std::size_t, int>(src);
				EXPECT(!result);
			}
		});

		addTest("inferior", [] {
			{
				constexpr auto src = u8"(2:21, 2:42<"sv;
				constexpr auto result = VM::execute<bool, std::size_t, int>(src);
				EXPECT(result);
			}
			{
				constexpr auto src = u8"(2:42, 2:21<"sv;
				constexpr auto result = VM::execute<bool, std::size_t, int>(src);
				EXPECT(!result);
			}
		});

		addTest("superior", [] {
			{
				constexpr auto src = u8"(2:21, 2:42>"sv;
				constexpr auto result = VM::execute<bool, std::size_t, int>(src);
				EXPECT(!result);
			}
			{
				constexpr auto src = u8"(2:42, 2:21>"sv;
				constexpr auto result = VM::execute<bool, std::size_t, int>(src);
				EXPECT(result);
			}
		});

		addTest("and", [] {
			{
				constexpr auto src = u8"1, 0:1&"sv;
				constexpr auto result = VM::execute<bool, std::size_t>(src);
				EXPECT(result);
			}
			{
				constexpr auto src = u8"0, 0:1&"sv;
				constexpr auto result = VM::execute<bool, std::size_t>(src);
				EXPECT(!result);
			}
		});

		addTest("or", [] {
			{
				constexpr auto src = u8"0, 0:1|"sv;
				constexpr auto result = VM::execute<bool, std::size_t>(src);
				EXPECT(result);
			}
			{
				constexpr auto src = u8"0, 0:0|"sv;
				constexpr auto result = VM::execute<bool, std::size_t>(src);
				EXPECT(!result);
			}
		});

		addTest("xor", [] {
			{
				constexpr auto src = u8"0, 0:1^"sv;
				constexpr auto result = VM::execute<bool, std::size_t>(src);
				EXPECT(result);
			}
			{
				constexpr auto src = u8"1, 0:1^"sv;
				constexpr auto result = VM::execute<bool, std::size_t>(src);
				EXPECT(!result);
			}
		});

		addTest("external prvalue", [] {
			constexpr auto src = u8"0, 1;"sv;
			constexpr auto result = VM::execute<int, std::size_t>(src, 42);
			EXPECT_EQUAL(result, 42);
		});

		addTest("external lvalue", [] {
			constexpr auto src = u8"0, 1;"sv;
			constexpr auto number = 42;
			constexpr auto result = VM::execute<int, std::size_t>(src, number);
			EXPECT_EQUAL(result, number);
		});

		addTest("external pointer", [] {
			constexpr auto src = u8"0, 1;R"sv;
			constexpr auto number = 42;
			auto result = VM::execute<int, std::size_t, const int*>(src, &number);
			EXPECT_EQUAL(result, number);
		});

		addTest("external lambda", [] {
			constexpr auto src = u8"0, 1;"sv;
			constexpr auto function = []() -> int { return 42; };
			constexpr auto result = VM::execute<int, std::size_t>(src, +function);
			EXPECT_EQUAL(result, function());
		});

		addTest("external function", [] {
			constexpr auto src = u8"0, 1;, 2;"sv;
			constexpr auto input = u8"Hello World!"sv;
			auto result = VM::execute<std::size_t, std::u8string_view>(src, input, std::size<decltype(input)>);
			EXPECT_EQUAL(result, std::size(input));
		});

		addTest("external member function", [] {
			constexpr auto src = u8"0, 1;, 2;"sv;
			constexpr auto input = u8"Hello World!"sv;
			auto result = VM::execute<std::size_t, const std::u8string_view*>(src, &input, &std::u8string_view::size);
			EXPECT_EQUAL(result, std::size(input));
		});

		addTest("condition", [] {
			{
				constexpr auto src = u8"0, 1, 4? 42X 21"sv;
				constexpr auto result = VM::execute<int, std::size_t>(src);
				EXPECT_EQUAL(result, 42);
			}
			{
				constexpr auto src = u8"0, 0, 4? 21X 42"sv;
				constexpr auto result = VM::execute<int, std::size_t>(src);
				EXPECT_EQUAL(result, 42);
			}
		});

		addTest("char", [] {
			constexpr auto src = u8R"(0, 2:\A, 1;)"sv;
			constexpr auto function = [](char8_t c) -> bool {
				return c == 'A';
			};
			constexpr auto result = VM::execute<bool, std::size_t, char8_t>(src, +function);
			EXPECT(result);
		});

		addTest("position", [] {
			constexpr auto src = u8"   P   "sv;
			constexpr auto result = VM::execute<std::size_t>(src);
			EXPECT_EQUAL(result, 3);
		});

		addTest("jump", [] {
			constexpr auto src = u8"0, 5JX) 42"sv;
			constexpr auto result = VM::execute<int, std::size_t>(src);
			EXPECT_EQUAL(result, 42);
		});

		addTest("copy", [] {
			constexpr auto src = u8"21, 0:0, 1, 0C+"sv;
			constexpr auto result = VM::execute<int, std::size_t>(src);
			EXPECT_EQUAL(result, 42);
		});

		addTest("cast", [] {
			constexpr auto src = u8"0, 1:42, 0, 1C)"sv;
			constexpr auto result = VM::execute<bool, std::size_t>(src);
			EXPECT_EQUAL(result, 1);
		});

		addTest("return string.at", [] {
			constexpr auto src = u8"0, 1;, 6, 2;"sv;
			constexpr auto input = u8"Hello World!"sv;
			auto result = VM::execute<char8_t, std::size_t, const std::u8string_view*>(src, &input, &std::u8string_view::at);
			EXPECT_EQUAL(result, 'W');
		});

		addTest("compare string.at", [] {
			constexpr auto src = u8R"(0, 2:0, 1;, 1:6, 2;, 2:\W =)"sv;
			constexpr auto input = u8"Hello World!"sv;
			auto result = VM::execute<bool, std::size_t, char8_t, const std::u8string_view*>(src, &input, &std::u8string_view::at);
			EXPECT(result);
		});

		addTest("print string_view", [] {
			constexpr auto src = u8R"(
				0, 1;, 2;
			)"sv;

			constexpr auto input = "Hello World!\n"sv;

			constexpr auto print = [](std::string_view text) -> void {
				Logger::print(text);
			};

			auto result = VM::execute<std::size_t, std::string_view>(src, input, +print);

			EXPECT_EQUAL(result, 0);
		});

		/*
			_3<_L <_D <_3SP <_4!	_<E
			_3<_R <_167D <_21!	_4<_L <_1SDA _<87J
			_3<_R <_42D <_51!	_4<_60A_95A_49A_83A_66A_95A_82A_60A_95A_83A_66A
			_3<_R <_4SA
			_3<_1J _<1B
		*/

		addTest("test loops", [] {
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

			constexpr auto input = "Hello World!\n"sv;

			constexpr auto print = [](std::string_view text) -> void {
				Logger::print(text);
			};

			auto result = VM::execute<std::size_t, bool, std::string_view>(src, nb, input, +print);

			EXPECT_EQUAL(result, 0);
		});

		addTest("compile labels", [] {
			constexpr auto src = u8R"(
				
			)"sv;
			constexpr auto input = u8R"(Hello World!)"sv;
			auto output = ""s;
			auto result = VM::execute<std::size_t, std::u8string_view, std::string*>(src, input, &output, std::size<decltype(input)>, &std::u8string_view::at);
			EXPECT_EQUAL(result, 0);
		});

		addTest("wip 2", [] {
			constexpr auto src = u8"0, 0, 1;, 6, 2;, 3;"sv;
			constexpr auto input = u8"Hello World!"sv;
			constexpr auto compare = [](char8_t c) -> std::intptr_t {
				std::wcout << static_cast<wchar_t>(c) << std::endl;
				return c == 'W';
			};
			auto result = VM::execute<std::intptr_t, bool, char8_t, std::size_t, const std::u8string_view*>(src, &input, &std::u8string_view::at, +compare);
			EXPECT(result);
		});
	}
}
