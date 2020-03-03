#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <functional>

using namespace std::string_literals;

#define ASSERT(condition) void((condition) ? 0 : throw CppUtils::UnitTest::TestException("ASSERT(" #condition ")", __FILE__, __LINE__))

namespace CppUtils::UnitTest
{
	class TestException : public std::runtime_error
	{
	public:
		explicit TestException(std::string message, std::string filename, int line) noexcept
			: std::runtime_error(std::move(message)), m_filename(std::move(filename)), m_line(line)
		{}
		virtual ~TestException() noexcept {}

		virtual const char* what() const noexcept override
		{
			auto message = std::stringstream();
			message << std::runtime_error::what() << "\nat line " << std::to_string(m_line) << " in " << m_filename;
			return message.str().c_str();
		}

	protected:
		std::string m_filename;
		int m_line;
	};

	class Test
	{
	public:
		explicit Test(std::string name, std::function<void()> function)
			: m_name(std::move(name)), m_function(std::move(function))
		{}

		bool pass() const
		{
			try
			{
				m_function();
			}
			catch (const TestException& exception)
			{
				std::cout << "The following test didn't pass:\n" << m_name << "\n" << exception.what() << std::endl;
				return false;
			}
			catch (const std::exception& exception)
			{
				std::cout << "An exception occurred during tests:\n" << m_name << "\n" << exception.what() << std::endl;
				return false;
			}
			return true;
		}

		static int executeTests(const std::vector<Test>& tests)
		{
			for (const auto& test : tests)
			{
				if (!test.pass())
				{
					std::cout << "The tests failed" << std::endl;
					return EXIT_FAILURE;
				}
			}
			
			std::cout << "The tests passed" << std::endl;
			return EXIT_SUCCESS;
		}

	protected:
		std::string m_name;
		std::function<void()> m_function;
	};
}