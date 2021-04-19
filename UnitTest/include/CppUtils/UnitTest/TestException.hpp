#pragma once

#include <stdexcept>
#include <string_view>

#define ASSERT(condition) \
	void((condition) ? 0 : throw CppUtils::UnitTest::TestException("ASSERT(" #condition ")", __FILE__, __LINE__))

namespace CppUtils::UnitTest
{
	class TestException: public std::runtime_error
	{
	public:
		explicit TestException(std::string_view message, std::string filename, int line) noexcept:
			std::runtime_error{std::string{message} + "\nat line " + std::to_string(line) + " in " + filename},
			m_filename{std::move(filename)},
			m_line{line}
		{}

	protected:
		std::string m_filename;
		int m_line;
	};
}
