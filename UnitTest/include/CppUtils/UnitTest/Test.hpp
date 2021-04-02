#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <functional>

#include <CppUtils/Type/Named.hpp>
#include <CppUtils/Log/Logger.hpp>
#include <CppUtils/Parameters/ParametersLexer.hpp>
#include <CppUtils/UnitTest/TestException.hpp>
#include <CppUtils/UnitTest/TestSettings.hpp>

namespace CppUtils::UnitTest
{
	class Test final: public Type::Named
	{
	public:
		Test(std::string name, std::function<void()> function):
			Named{std::move(name)},
			m_function{std::move(function)}
		{}

		bool pass(const TestSettings& settings) const
		{
			using namespace std::string_literals;
			using namespace Type::Literals;

			auto oldLoggerState = Log::Logger::state;
			if (settings.verbose)
				Log::Logger::logImportant(std::string(50, '_') + '\n' + getName().data() + ':');
			Log::Logger::state.setAll(settings.verbose);
			Log::Logger::state.set("Information"_token, settings.verbose && settings.detail);
			Log::Logger::state.set("Detail"_token, settings.verbose && settings.detail);
			try
			{
				auto chronoLogger = Log::ChronoLogger{"Test", settings.verbose && settings.chrono};
				m_function();
				chronoLogger.stop();
			}
			catch (const TestException& exception)
			{
				Log::Logger::state = oldLoggerState;
				Log::Logger::logError("The following test didn't pass:\n"s + getName().data() + "\n" + exception.what());
				return false;
			}
			catch (const std::exception& exception)
			{
				Log::Logger::state = oldLoggerState;
				Log::Logger::logError("An exception occurred during tests:\n"s + getName().data() + "\n" + exception.what());
				return false;
			}
			Log::Logger::state = oldLoggerState;
			if (settings.verbose)
				Log::Logger::logSuccess(std::string{getName()} + " passed");
			return true;
		}

	protected:
		std::function<void()> m_function;
	};
}
