#pragma once

#include <stdexcept>
#include <string_view>

#define TEST_ASSERT(condition) \
	void((condition) ? 0 : throw CppUtils::UnitTest::TestException("TEST_ASSERT(" #condition ")", __FILE__, __LINE__))

namespace CppUtils::UnitTest
{
	class TestException: public std::runtime_error
	{
	public:
		explicit TestException(std::string_view message, std::string filename, std::size_t line) noexcept:
			std::runtime_error{std::string{message} + "\nat line " + std::to_string(line) + " in " + filename},
			m_filename{std::move(filename)},
			m_line{line}
		{}

	protected:
		std::string m_filename;
		std::size_t m_line;
	};
}
