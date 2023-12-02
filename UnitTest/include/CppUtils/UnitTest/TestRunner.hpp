#pragma once

#include <format>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <functional>
// Todo C++23: #include <stacktrace>
#include <source_location>

#include <CppUtils/Log/Logger.hpp>
#include <CppUtils/String/String.hpp>
#include <CppUtils/Parameters/ParametersLexer.hpp>

namespace CppUtils::UnitTest
{
	struct TestSettings final
	{
		bool verbose = true;
		bool detail = true;
		bool failFast = true;
		bool chrono = false;
		bool abort = false;
		std::string filter;
	};

	struct Test final
	{
	public:
		std::string name;
		std::function<void()> function;
	};

	class TestException: public std::runtime_error
	{
	public:
		TestException(std::string message):
			std::runtime_error{std::move(message)}
		{}
	};

	class TestRunner final
	{
	public:
		auto addTestSuite(std::string_view name, std::vector<Test> tests) -> void
		{
			for (auto& test : tests)
				test.name = std::format("{}/{}", name, test.name);
			m_tests.insert(std::end(m_tests), std::make_move_iterator(std::begin(tests)), std::make_move_iterator(std::end(tests)));
		}

		auto executeTest(const Test& test, const TestSettings& settings) const -> bool
		{
			using namespace std::literals;
			using namespace Hashing::Literals;
			using Logger = Logger<"CppUtils">;
			
			if (settings.verbose)
			{
				Logger::printSeparator<"detail">();
				Logger::print<"detail">("{}:", test.name);
			}
			try
			{
				try
				{
					auto chronoLogger = Log::ChronoLogger{"Test", settings.verbose && settings.chrono};
					test.function();
					chronoLogger.stop();
				}
				catch (const TestException&)
				{
					std::throw_with_nested(std::runtime_error{std::format("The following test didn't pass: {}", test.name)});
				}
				catch (const std::exception&)
				{
					std::throw_with_nested(std::runtime_error{std::format("An exception occurred during the test: {}", test.name)});
				}
			}
			catch (const std::exception& exception)
			{
				logException(exception);
				return false;
			}
			if (settings.verbose)
				Logger::print<"success">("{} passed", test.name);
			return true;
		}

		auto executeTests(TestSettings settings) -> int
		{
			using Logger = Logger<"CppUtils">;

			if (!std::empty(settings.filter))
			{
				m_tests.erase(std::remove_if(std::begin(m_tests), std::end(m_tests),
					[&settings](const Test& test) -> bool {
						return test.name.substr(0, settings.filter.size()) != settings.filter;
					}
				), std::end(m_tests));
				auto newSettings = settings;
				newSettings.filter = "";
				const auto result = executeTests(newSettings);
				return result;
			}

			if (std::empty(m_tests))
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
					if (executeTest(test, settings))
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
	
	static constinit auto testRunner = TestRunner{};

	struct TestSuite final
	{
		explicit TestSuite(std::string name, std::function<void(TestSuite&)> function)
		{
			function(*this);
			testRunner.addTestSuite(std::move(name), std::move(tests));
		}

		auto addTest(std::string name, std::function<void()> function) -> void
		{
			tests.emplace_back(std::move(name), function);
		}

		// Todo C++23: std::stacktrace stacktrace = std::current_stacktrace()
		auto expect(bool condition, std::source_location sourceLocation = std::source_location::current()) -> void
		{
			if (!condition) [[unlikely]]
				throw TestException{std::format("In {}\nAt line {}, column {}\nIn expect(condition)",
					sourceLocation.file_name(),
					sourceLocation.line(), sourceLocation.column())};
		}

		auto expectEqual(auto lhs, auto rhs, std::source_location sourceLocation = std::source_location::current()) -> void
		{
			if (lhs != rhs) [[unlikely]]
			{
				throw TestException{std::format("In {}\nAt line {}, column {}\nIn expectEqual(lhs, rhs)\nwith lhs = {}\nand  rhs = {}",
					sourceLocation.file_name(),
					sourceLocation.line(), sourceLocation.column(),
					(std::formattable<decltype(lhs), char>) ? String::formatValue(lhs) : "<non printable>",
					(std::formattable<decltype(rhs), char>) ? String::formatValue(rhs) : "<non printable>")};
			}
		}

		std::vector<Test> tests;
	};

	inline auto executeTests(TestSettings settings) -> int
	{
		return testRunner.executeTests(std::move(settings));
	}
}
