#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <functional>

#include <CppUtils/Log/Logger.hpp>

#define ASSERT(condition) void((condition) ? 0 : throw CppUtils::TestException("ASSERT(" #condition ")", __FILE__, __LINE__))

namespace CppUtils
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

	struct TestSettings final
	{
		bool verbose = true;
		bool detail = true;
		bool failFast = false;
		bool chrono = false;
		bool abort = false;
		std::string filter;
	};

	class Test final
	{
	public:
		explicit Test(std::string name, std::function<void()> function):
			m_name{std::move(name)},
			m_function{std::move(function)}
		{}

		[[nodiscard]] inline std::string_view getName() const noexcept
		{
			return m_name;
		}

		bool pass(const TestSettings& settings) const
		{
			using namespace std::string_literals;
			using namespace Type::Literals;

			auto loggersState = Switch::getValues("Logger"_token);
			if (settings.verbose)
				Log::Logger::logImportant(std::string(50, '_') + '\n' + m_name + ':');
			Switch::setValue("Logger"_token, settings.verbose);
			Switch::setValue(Log::Logger::InformationType, settings.verbose && settings.detail);
			Switch::setValue(Log::Logger::DetailType, settings.verbose && settings.detail);
			try
			{
				auto chronoLogger = Log::ChronoLogger{"Test", settings.verbose && settings.chrono};
				m_function();
				chronoLogger.stop();
			}
			catch (const TestException& exception)
			{
				Switch::setValues(loggersState);
				Log::Logger::logError("The following test didn't pass:\n"s + m_name + "\n" + exception.what());
				return false;
			}
			catch (const std::exception& exception)
			{
				Switch::setValues(loggersState);
				Log::Logger::logError("An exception occurred during tests:\n"s + m_name + "\n" + exception.what());
				return false;
			}
			Switch::setValues(loggersState);
			if (settings.verbose)
				Log::Logger::logSuccess(m_name + " passed");
			return true;
		}

		static int executeTests(std::vector<Test> tests, TestSettings settings)
		{
			auto nbSuccess = std::size_t{0};
			auto nbFail = std::size_t{0};
			
			if (!settings.filter.empty())
				tests.erase(std::remove_if(tests.begin(), tests.end(),
					[&settings](const Test& test) -> bool {
						return test.getName().substr(0, settings.filter.size()) != settings.filter;
					}
				), tests.end());
			Log::Logger::logImportant(std::to_string(tests.size()) + " tests found. Execution:");
			for (const auto& test : tests)
			{
				if (test.pass(settings))
					++nbSuccess;
				else
					++nbFail;
			}
			
			Log::Logger::logImportant(std::string(50, '_') + "\nTest results");
			if (nbFail == 0)
			{
				Log::Logger::logSuccess("All tests passed successfully");
				return EXIT_SUCCESS;
			}
			Log::Logger::logError("The tests failed:");
			if (nbSuccess > 0)
				Log::Logger::logSuccess("- " + std::to_string(nbSuccess) + " successful tests");
			else
				Log::Logger::logError("- 0 successful tests");
			Log::Logger::logError("- " + std::to_string(nbFail) + " failed tests");
			return EXIT_FAILURE;
		}

		[[nodiscard]] static TestSettings executeCommands(const int argc, const char *argv[])
		{
			using namespace std::literals;
			
			const auto parametersLexer = Language::Lexer::ParametersLexer{};
			auto settings = TestSettings{};
			
			settings.abort = parametersLexer.executeCommands(argc, argv, {
				{
					"help",
					[]([[maybe_unused]] auto value) -> bool {
						Log::Logger::logImportant("The parameters available are:\n- help\n- mode[debug|test|prod]\n- verbose[boolean]\n- detail[boolean]\n- failFast[boolean]\n- chrono[boolean]\n- filter[string]\n- chrono\n\nUsage: ./UnitTests mode[debug] filter[Log]");
						return true;
					}
				},
				{
					"mode",
					[&settings]([[maybe_unused]] auto value) -> bool {
						if (value == "test")
							settings.chrono = true;
						else if (value == "prod")
						{
							settings.verbose = false;
							settings.detail = false;
							settings.failFast = true;
						}
						else
							return false;
						return false;
					}
				},
				{
					"verbose",
					[&settings]([[maybe_unused]] auto value) -> bool {
						settings.verbose = (value.empty() || value == "1" || value == "true");
						return false;
					}
				},
				{
					"detail",
					[&settings]([[maybe_unused]] auto value) -> bool {
						settings.detail = (value.empty() || value == "1" || value == "true");
						return false;
					}
				},
				{
					"failFast",
					[&settings]([[maybe_unused]] auto value) -> bool {
						settings.failFast = (value.empty() || value == "1" || value == "true");
						return false;
					}
				},
				{
					"chrono",
					[&settings]([[maybe_unused]] auto value) -> bool {
						settings.chrono = (value.empty() || value == "1" || value == "true");
						return false;
					}
				},
				{
					"filter",
					[&settings]([[maybe_unused]] auto value) -> bool {
						settings.filter = value;
						return false;
					}
				}
			});

			return settings;
		}

	protected:
		std::string m_name;
		std::function<void()> m_function;
	};
}
