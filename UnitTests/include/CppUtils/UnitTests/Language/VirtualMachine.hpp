#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::VirtualMachine
{
	auto print(std::string_view text) -> void
	{
		CppUtils::Log::Logger{std::cout} << text << '\n';
	}

	TEST_GROUP("Language/VirtualMachine")
	{
		using namespace std::literals;

		namespace VM = CppUtils::Language::VirtualMachine;

		addTest("empty source", [] {
			constexpr auto src = u8""sv;
			auto result = VM::execute<std::intptr_t>(src);
			EXPECT_EQUAL(result, 0);
		});

		addTest("return value", [] {
			constexpr auto src = u8"42"sv;
			auto result = VM::execute<std::intptr_t>(src);
			EXPECT_EQUAL(result, 42);
		});

		addTest("reset value", [] {
			constexpr auto src = u8"42_"sv;
			auto result = VM::execute<std::intptr_t>(src);
			EXPECT_EQUAL(result, 0);
		});

		addTest("exit with error code", [] {
			constexpr auto src = u8"0, 21X_"sv;
			auto result = VM::execute<std::intptr_t>(src);
			EXPECT_EQUAL(result, 21);
		});

		addTest("addition", [] {
			constexpr auto src = u8"40, 2+"sv;
			auto result = VM::execute<std::intptr_t>(src);
			EXPECT_EQUAL(result, 42);
		});

		addTest("substraction", [] {
			constexpr auto src = u8"44, 2-"sv;
			auto result = VM::execute<std::intptr_t>(src);
			EXPECT_EQUAL(result, 42);
		});

		addTest("multiplication", [] {
			constexpr auto src = u8"21, 2*"sv;
			auto result = VM::execute<std::intptr_t>(src);
			EXPECT_EQUAL(result, 42);
		});

		addTest("division", [] {
			constexpr auto src = u8"84, 2/"sv;
			auto result = VM::execute<std::intptr_t>(src);
			EXPECT_EQUAL(result, 42);
		});

		addTest("not", [] {
			{
				constexpr auto src = u8"0!"sv;
				auto result = VM::execute<std::intptr_t>(src);
				EXPECT_EQUAL(result, 1);
			}
			{
				constexpr auto src = u8"1!"sv;
				auto result = VM::execute<std::intptr_t>(src);
				EXPECT_EQUAL(result, 0);
			}
		});

		addTest("equal", [] {
			{
				constexpr auto src = u8"40, 2+, 42="sv;
				auto result = VM::execute<std::intptr_t>(src);
				EXPECT(result);
			}
			{
				constexpr auto src = u8"0, 42="sv;
				auto result = VM::execute<std::intptr_t>(src);
				EXPECT(!result);
			}
		});

		addTest("inferior", [] {
			{
				constexpr auto src = u8"21, 42<"sv;
				auto result = VM::execute<std::intptr_t>(src);
				EXPECT(result);
			}
			{
				constexpr auto src = u8"42, 21<"sv;
				auto result = VM::execute<std::intptr_t>(src);
				EXPECT(!result);
			}
		});

		addTest("superior", [] {
			{
				constexpr auto src = u8"21, 42>"sv;
				auto result = VM::execute<std::intptr_t>(src);
				EXPECT(!result);
			}
			{
				constexpr auto src = u8"42, 21>"sv;
				auto result = VM::execute<std::intptr_t>(src);
				EXPECT(result);
			}
		});

		addTest("and", [] {
			{
				constexpr auto src = u8"1, 1&"sv;
				auto result = VM::execute<std::intptr_t>(src);
				EXPECT(result);
			}
			{
				constexpr auto src = u8"0, 1&"sv;
				auto result = VM::execute<std::intptr_t>(src);
				EXPECT(!result);
			}
		});

		addTest("or", [] {
			{
				constexpr auto src = u8"0, 1|"sv;
				auto result = VM::execute<std::intptr_t>(src);
				EXPECT(result);
			}
			{
				constexpr auto src = u8"0, 0|"sv;
				auto result = VM::execute<std::intptr_t>(src);
				EXPECT(!result);
			}
		});

		addTest("xor", [] {
			{
				constexpr auto src = u8"0, 1^"sv;
				auto result = VM::execute<std::intptr_t>(src);
				EXPECT(result);
			}
			{
				constexpr auto src = u8"1, 1^"sv;
				auto result = VM::execute<std::intptr_t>(src);
				EXPECT(!result);
			}
		});

		addTest("external prvalue", [] {
			constexpr auto src = u8"0, 1;"sv;
			auto result = VM::execute<std::intptr_t>(src, std::intptr_t{42});
			EXPECT_EQUAL(result, 42);
		});

		addTest("external lvalue", [] {
			constexpr auto src = u8"0, 1;"sv;
			constexpr auto number = std::intptr_t{42};
			auto result = VM::execute<std::intptr_t>(src, number);
			EXPECT_EQUAL(result, number);
		});

		addTest("external pointer", [] {
			constexpr auto src = u8"0, 1;R"sv;
			constexpr auto number = std::intptr_t{42};
			auto result = VM::execute<std::intptr_t>(src, &number);
			EXPECT_EQUAL(result, number);
		});

		addTest("external lambda", [] {
			constexpr auto src = u8"0, 1;"sv;
			constexpr auto function = []() -> std::intptr_t { return std::intptr_t{42}; };
			auto result = VM::execute<std::intptr_t>(src, +function);
			EXPECT_EQUAL(result, function());
		});

		addTest("external function", [] {
			constexpr auto src = u8"0, 1;, 2;"sv;
			constexpr auto input = u8"Hello World!"sv;
			auto result = VM::execute<std::intptr_t>(src, input, std::size<decltype(input)>);
			EXPECT_EQUAL(result, std::size(input));
		});

		addTest("external member function", [] {
			constexpr auto src = u8"0, 1;, 2;"sv;
			constexpr auto input = u8"Hello World!"sv;
			auto result = VM::execute<std::intptr_t>(src, &input, &std::u8string_view::size);
			EXPECT_EQUAL(result, std::size(input));
		});

		addTest("condition", [] {
			{
				constexpr auto src = u8"1, 5?, 42X, 0"sv;
				auto result = VM::execute<std::intptr_t>(src);
				EXPECT_EQUAL(result, 42);
			}
			{
				constexpr auto src = u8"0, 4?, 0X, 42"sv;
				auto result = VM::execute<std::intptr_t>(src);
				EXPECT_EQUAL(result, 42);
			}
		});

		addTest("char", [] {
			constexpr auto src = u8R"(0, \A, 1;)"sv;
			constexpr auto function = [](char8_t c) -> std::intptr_t {
				return c == 'A';
			};
			auto result = VM::execute<std::intptr_t>(src, +function);
			EXPECT(result);
		});

		addTest("position", [] {
			constexpr auto src = u8"   P   "sv;
			auto result = VM::execute<std::intptr_t>(src);
			EXPECT_EQUAL(result, 3);
		});

		addTest("jump", [] {
			constexpr auto src = u8"0, 5JX 42"sv;
			auto result = VM::execute<std::intptr_t>(src);
			EXPECT_EQUAL(result, 42);
		});

		addTest("copy", [] {
			constexpr auto src = u8"21, 0, 1, 0C+"sv;
			auto result = VM::execute<std::intptr_t>(src);
			EXPECT_EQUAL(result, 42);
		});

		addTest("test string.at", [] {
			constexpr auto src = u8R"(0, 2:, 1;, 1:6, 2;, \W, =)"sv; // 0, 2:, 1;, 1:6, 2;, \W, =
			constexpr auto input = u8"Hello World!"sv;
			auto result = VM::execute<std::intptr_t, std::size_t, char8_t>(src, input, &std::u8string_view::at);
			EXPECT(result);
		});

		addTest("compile labels", [] {
			constexpr auto src = u8R"(
				0, 1;, 3;
			)"sv;
			constexpr auto input = u8R"(Hello World!)"sv;
			auto output = ""s;
			auto result = VM::execute<std::intptr_t>(src, input, &output, std::size<decltype(input)>, &std::u8string_view::at);
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
				0, 1;, 3;,
				0=, 4? (0X),
				3 P,
				0, 2, 0C,
				0=, 4? (0X),
				0, 2, 0C, 1-, 0, 2C),
				0, 1;, 4;
				0, 1, 0C J
			)"sv;

			constexpr auto input = u8R"(
				Hello World!
			)"sv;

			auto output = ""s;

			auto result = VM::execute<std::intptr_t>(src, input, &output, std::size<decltype(input)>, print);
			CppUtils::Log::Logger{std::cout} << result << '\n' << output << '\n';

			EXPECT_EQUAL(result, 0);
		});

		addTest("wip 2", [] {
			constexpr auto src = u8"0, 0, 1;, 6, 2;, 3;"sv;
			constexpr auto input = u8"Hello World!"sv;
			constexpr auto compare = [](char8_t c) -> std::intptr_t {
				std::wcout << static_cast<wchar_t>(c) << std::endl;
				return c == 'W';
			};
			auto result = VM::execute<std::intptr_t>(src, &input, &std::u8string_view::at, +compare);
			EXPECT(result);
		});
	}
}
