#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <functional>

#include <Log/Logger.hpp>

using namespace std::string_literals;

#define ASSERT(condition) void((condition) ? 0 : throw CppUtils::Test::TestException("ASSERT(" #condition ")", __FILE__, __LINE__))

namespace CppUtils::Test
{
	class TestException: public std::runtime_error
	{
	public:
		explicit TestException(std::string message, std::string filename, int line) noexcept:
			std::runtime_error{message + "\nat line " + std::to_string(line) + " in " + filename},
			m_filename{std::move(filename)},
			m_line{line}
		{}

	protected:
		std::string m_filename;
		int m_line;
	};

	class UnitTest
	{
	public:
		explicit UnitTest(std::string name, std::function<void()> function):
			m_name{std::move(name)},
			m_function{std::move(function)}
		{}

		bool pass() const
		{
			try
			{
				m_function();
				CppUtils::Logger::resetEnabledTypes();
			}
			catch (const TestException& exception)
			{
				CppUtils::Logger::resetEnabledTypes();
				CppUtils::Logger::logError("The following test didn't pass:\n"s + m_name + "\n" + exception.what());
				return false;
			}
			catch (const std::exception& exception)
			{
				CppUtils::Logger::resetEnabledTypes();
				CppUtils::Logger::logError("An exception occurred during tests:\n"s + m_name + "\n" + exception.what());
				return false;
			}
			return true;
		}

		static int executeTests(const std::vector<UnitTest>& tests)
		{
			for (const auto& test : tests)
			{
				if (!test.pass())
				{
					CppUtils::Logger::logError("The tests failed");
					return EXIT_FAILURE;
				}
			}
			
			CppUtils::Logger::logSuccess("The tests passed");
			return EXIT_SUCCESS;
		}

	protected:
		std::string m_name;
		std::function<void()> m_function;
	};
}
