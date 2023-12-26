#pragma once

#include <string>

#include <CppUtils/Log/Logger.hpp>
#include <CppUtils/UnitTest/TestRunner.hpp>

namespace CppUtils::UnitTest
{
	/*
	class TestSettingsLexer final
	{
	public:
		[[nodiscard]] TestSettings& executeCommands(const int argc, const char *argv[])
		{
			m_settings = TestSettings{};
			m_settings.abort = m_parametersLexer.executeCommands(argc, argv, {
				{
					"help",
					[]([[maybe_unused]] auto value) -> bool {
						Log::Logger::logImportant("The parameters available are:\n- help\n- mode[debug|test|prod]\n- verbose[boolean]\n- detail[boolean]\n- failFast[boolean]\n- chrono[boolean]\n- filter[string]\n- chrono\n\nUsage: ./UnitTests mode[debug] filter[Log]");
						return true;
					}
				},
				{
					"mode",
					[this]([[maybe_unused]] auto value) -> bool {
						if (value == "test")
							m_settings.chrono = true;
						else if (value == "prod")
						{
							m_settings.verbose = false;
							m_settings.detail = false;
							m_settings.failFast = true;
						}
						else
							return false;
						return false;
					}
				},
				{
					"verbose",
					[this]([[maybe_unused]] auto value) -> bool {
						m_settings.verbose = (value.empty() || value == "1" || value == "true");
						return false;
					}
				},
				{
					"detail",
					[this]([[maybe_unused]] auto value) -> bool {
						m_settings.detail = (value.empty() || value == "1" || value == "true");
						return false;
					}
				},
				{
					"failFast",
					[this]([[maybe_unused]] auto value) -> bool {
						m_settings.failFast = (value.empty() || value == "1" || value == "true");
						return false;
					}
				},
				{
					"chrono",
					[this]([[maybe_unused]] auto value) -> bool {
						m_settings.chrono = (value.empty() || value == "1" || value == "true");
						return false;
					}
				},
				{
					"filter",
					[this]([[maybe_unused]] auto value) -> bool {
						m_settings.filter = value;
						return false;
					}
				}
			});
			return m_settings;
		}

	private:
		const Parameters::ParametersLexer m_parametersLexer;
		TestSettings m_settings;
	};

	[[nodiscard]] TestSettings& executeCommands(const int argc, const char *argv[])
	{
		static auto testSettingsLexer = TestSettingsLexer{};
		return testSettingsLexer.executeCommands(argc, argv);
	}
	*/
}
