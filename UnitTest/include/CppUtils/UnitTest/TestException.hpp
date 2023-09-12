#pragma once

#include <stdexcept>
#include <string_view>
#include <source_location>

#define EXPECT(condition) CppUtils::UnitTest::assert(condition, "EXPECT(" #condition ")");
#define EXPECT_EQUAL(lhs, rhs) CppUtils::UnitTest::assert(lhs == rhs, "EXPECT_EQUAL(" #lhs ", " #rhs ") with:\n\t" #lhs " = " + std::to_string(lhs) + "\n\t" #rhs " = " + std::to_string(rhs) + '\n');

namespace CppUtils::UnitTest
{
	class TestException: public std::runtime_error
	{
	public:
		explicit TestException(std::string_view message, std::source_location source_location) noexcept:
			std::runtime_error{std::string{"In "} + source_location.file_name() +
				"\nAt line " + std::to_string(source_location.line()) +
				", column " + std::to_string(source_location.column()) +
				"\nIn " + source_location.function_name() +
				'\n' + std::string{message}},
			m_source_location{source_location}
		{}

	protected:
		std::source_location m_source_location;
	};

	auto assert(
		bool condition,
		std::string_view message,
		std::source_location source_location = std::source_location::current()) -> void
	{
		if (!condition)
			throw CppUtils::UnitTest::TestException{message, std::move(source_location)};
	}
}
