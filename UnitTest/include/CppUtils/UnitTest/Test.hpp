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

			auto loggersState = Switch::getValues("Logger"_token);
			if (settings.verbose)
				Log::Logger::logImportant(std::string(50, '_') + '\n' + getName().data() + ':');
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
				Log::Logger::logError("The following test didn't pass:\n"s + getName().data() + "\n" + exception.what());
				return false;
			}
			catch (const std::exception& exception)
			{
				Switch::setValues(loggersState);
				Log::Logger::logError("An exception occurred during tests:\n"s + getName().data() + "\n" + exception.what());
				return false;
			}
			Switch::setValues(loggersState);
			if (settings.verbose)
				Log::Logger::logSuccess(std::string{getName()} + " passed");
			return true;
		}

	protected:
		std::function<void()> m_function;
	};
}
