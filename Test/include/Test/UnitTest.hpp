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
			CppUtils::Log::Logger::logImportant(std::string(50, '_') + '\n' + m_name + ':');
			auto switchIds = CppUtils::Switch::getEnabledIds();
			try
			{
				m_function();
				CppUtils::Switch::setEnabledIds(std::move(switchIds));
			}
			catch (const TestException& exception)
			{
				CppUtils::Switch::setEnabledIds(std::move(switchIds));
				CppUtils::Log::Logger::logError("The following test didn't pass:\n"s + m_name + "\n" + exception.what());
				return false;
			}
			catch (const std::exception& exception)
			{
				CppUtils::Switch::setEnabledIds(std::move(switchIds));
				CppUtils::Log::Logger::logError("An exception occurred during tests:\n"s + m_name + "\n" + exception.what());
				return false;
			}
			CppUtils::Log::Logger::logSuccess(m_name + " passed");
			return true;
		}

		static int executeTests(const std::vector<UnitTest>& tests)
		{
			auto nbSuccess = std::size_t{0};
			auto nbFail = std::size_t{0};
			
			CppUtils::Log::Logger::logImportant(std::to_string(tests.size()) + " unit tests found. Execution:");
			for (const auto& test : tests)
			{
				if (test.pass())
					++nbSuccess;
				else
					++nbFail;
			}
			
			CppUtils::Log::Logger::logImportant(std::string(50, '_'));
			if (nbFail == 0)
			{
				CppUtils::Log::Logger::logSuccess("All tests passed successfully");
				return EXIT_SUCCESS;
			}
			CppUtils::Log::Logger::logError("The tests failed:\n- " + std::to_string(nbSuccess) + " successful tests\n- " + std::to_string(nbFail) + " failed tests");
			return EXIT_FAILURE;
		}

	protected:
		std::string m_name;
		std::function<void()> m_function;
	};
}
