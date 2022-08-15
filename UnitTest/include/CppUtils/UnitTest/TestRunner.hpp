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
			std::move(tests.begin(), tests.end(), std::back_inserter(m_tests));
		}

		int executeTests(TestSettings settings)
		{
			auto logger = Log::Logger{std::cout};
			if (!settings.filter.empty())
			{
				auto allTests = std::move(m_tests);
				m_tests = allTests;
				m_tests.erase(std::remove_if(m_tests.begin(), m_tests.end(),
					[&settings](const Test& test) -> bool {
						return test.getName().substr(0, settings.filter.size()) != settings.filter;
					}
				), m_tests.end());
				auto newSettings = settings;
				newSettings.filter = "";
				const auto result = executeTests(newSettings);
				m_tests = std::move(allTests);
				return result;
			}

			if (m_tests.empty())
			{
				logger
					<< Terminal::TextColor::TextColorEnum::Yellow
					<< "No tests found.\n"
					<< Terminal::TextColor::TextColorEnum::Blue
					<< "Create a unit test or adjust the filter.\n";
			}
			else
			{
				logger
					<< Terminal::TextColor::TextColorEnum::Default
					<< std::to_string(m_tests.size()) + " tests found. Execution:\n";
				auto nbSuccess = std::size_t{0};
				auto nbFail = std::size_t{0};
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
								logger
									<< Terminal::TextColor::TextColorEnum::Blue
									<< "Stopping tests execution (failFast mode enabled)\n";
							break;
						}
					}
				}
				logger
					<< Terminal::TextColor::TextColorEnum::Default
					<< std::string(settings.terminalSize.width, '-') + "\nTest results\n";
				if (nbFail == 0)
				{
					logger
						<< Terminal::TextColor::TextColorEnum::Green
						<< "All tests passed successfully\n";
					return EXIT_SUCCESS;
				}
				logger
					<< Terminal::TextColor::TextColorEnum::Red
					<< "The tests failed:\n";
				if (nbSuccess > 0)
					logger
						<< Terminal::TextColor::TextColorEnum::Green
						<< "- " + std::to_string(nbSuccess) + " successful tests\n";
				else
					logger
						<< Terminal::TextColor::TextColorEnum::Red
						<< "- 0 successful tests\n";
				logger
					<< Terminal::TextColor::TextColorEnum::Red
					<< "- " + std::to_string(nbFail) + " failed tests\n";
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
