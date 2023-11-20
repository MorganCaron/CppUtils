#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <functional>

#include <CppUtils/Hash.hpp>
#include <CppUtils/Log/Logger.hpp>
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
			using namespace Hashing::Literals;
			using Logger = Logger<"CppUtils">;
			
			if (settings.verbose)
			{
				Logger::printSeparator<"detail">();
				Logger::print<"detail">("{}:", getName());
			}
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
				logException(exception);
				return false;
			}
			if (settings.verbose)
				Logger::print<"success">("{} passed", getName());
			return true;
		}

	protected:
		std::function<void()> m_function;
	};
}
