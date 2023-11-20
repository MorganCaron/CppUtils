#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <functional>

#include <CppUtils/Type/Named.hpp>
#include <CppUtils/Log/Logger.hpp>
#include <CppUtils/Parameters/ParametersLexer.hpp>
#include <CppUtils/UnitTest/Test.hpp>
#include <CppUtils/UnitTest/TestException.hpp>
#include <CppUtils/UnitTest/TestSettings.hpp>

namespace CppUtils::UnitTest
{
	class TestRunner final
	{
	private:
		TestRunner() = default;

	public:
		static TestRunner& instance()
		{
			static auto testRunner = TestRunner{};
			return testRunner;
		}

		void addTests(std::vector<Test> tests)
		{
			std::move(std::begin(tests), std::end(tests), std::back_inserter(m_tests));
		}

		int executeTests(TestSettings settings)
		{
			using Logger = Logger<"CppUtils">;

			if (!settings.filter.empty())
			{
				auto allTests = std::move(m_tests);
				m_tests = allTests;
				m_tests.erase(std::remove_if(std::begin(m_tests), std::end(m_tests),
					[&settings](const Test& test) -> bool {
						return test.getName().substr(0, settings.filter.size()) != settings.filter;
					}
				), std::end(m_tests));
				auto newSettings = settings;
				newSettings.filter = "";
				const auto result = executeTests(newSettings);
				m_tests = std::move(allTests);
				return result;
			}

			if (m_tests.empty())
			{
				Logger::print<"warning">("No tests found.");
				Logger::print<"detail">("Create a unit test or adjust the filter.");
			}
			else
			{
				Logger::print<"info">("{} tests found. Execution:", std::size(m_tests));
				auto nbSuccess = 0uz;
				auto nbFail = 0uz;
				for (const auto& test : m_tests)
				{
					if (test.pass(settings))
						++nbSuccess;
					else
					{
						++nbFail;
						if (settings.failFast)
						{
							if (settings.detail)
								Logger::print<"detail">("Stopping tests execution (failFast mode enabled)");
							break;
						}
					}
				}
				Logger::printSeparator<"detail">();
				Logger::print<"detail">("Test results");
				if (nbFail == 0)
				{
					Logger::print<"success">("All tests passed successfully");
					return EXIT_SUCCESS;
				}
				Logger::print<"error">("The tests failed:");
				if (nbSuccess > 0)
					Logger::print<"success">("- {} successful tests", nbSuccess);
				[[maybe_unused]] auto textModifier = Terminal::TextModifier{stdout, Terminal::TextColor::TextColorEnum::Red};
				if (nbSuccess == 0)
					Logger::print("- 0 successful tests");
				Logger::print("- {} failed tests", nbFail);
			}
			
			return EXIT_FAILURE;
		}

	private:
		std::vector<Test> m_tests;
	};

	inline int executeTests(TestSettings settings)
	{
		return CppUtils::UnitTest::TestRunner::instance().executeTests(std::move(settings));
	}
}
