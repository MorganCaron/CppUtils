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
			auto logger = Log::Logger{std::cout};
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
					<< std::to_string(std::size(m_tests)) + " tests found. Execution:\n";
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
					<< Terminal::TextColor::TextColorEnum::Blue
					<< Log::getSeparatorLine() << "\nTest results\n";
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
						<< "- " << nbSuccess << " successful tests\n";
				else
					logger
						<< Terminal::TextColor::TextColorEnum::Red
						<< "- 0 successful tests\n";
				logger
					<< Terminal::TextColor::TextColorEnum::Red
					<< "- " << nbFail << " failed tests\n";
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
