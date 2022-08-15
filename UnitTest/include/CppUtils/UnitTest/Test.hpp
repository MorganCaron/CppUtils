#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <functional>

#include <CppUtils/Hash/Token.hpp>
#include <CppUtils/Type/Named.hpp>
#include <CppUtils/Log/ChronoLogger.hpp>
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

		auto pass(const TestSettings& settings) const -> bool
		{
			using namespace std::literals;
			using namespace Hash::Literals;

			auto logger = Log::Logger{std::cout};
			if (settings.verbose)
				logger
					<< Terminal::TextColor::TextColorEnum::Blue
					<< std::string(settings.terminalSize.width, '-') + '\n' + std::data(getName()) + ":\n";
			try
			{
				try
				{
					auto chronoLogger = Log::ChronoLogger{"Test", settings.verbose && settings.chrono};
					m_function();
					chronoLogger.stop();
				}
				catch (const TestException& exception)
				{
					std::throw_with_nested(std::runtime_error{"The following test didn't pass: "s + std::data(getName())});
				}
				catch (const std::exception& exception)
				{
					std::throw_with_nested(std::runtime_error{"An exception occurred during the test: "s + std::data(getName())});
				}
			}
			catch (const std::exception& exception)
			{
				Log::logException(exception, std::cout);
				return false;
			}
			if (settings.verbose)
			{
				auto logger = Log::Logger{std::cout};
				logger
					<< Terminal::TextColor::TextColorEnum::Green
					<< std::string{getName()} + " passed\n";
			}
			return true;
		}

	protected:
		std::function<void()> m_function;
	};
}
