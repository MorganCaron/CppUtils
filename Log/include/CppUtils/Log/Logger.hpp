#pragma once

#include <vector>
#include <ostream>
#include <iostream>
#include <algorithm>
#include <exception>
#include <unordered_map>

#include <CppUtils/Hash/Token.hpp>
#include <CppUtils/External/DllExport.hpp>
#include <CppUtils/Terminal/TextModifier.hpp>

namespace CppUtils::Log
{
	using namespace Hash::Literals;

	class State final
	{
	public:
		State(): m_loggerStates{
				{ "Information"_token, true },
				{ "Important"_token, true },
				{ "Success"_token, true },
				{ "Debug"_token, true },
				{ "Detail"_token, true },
				{ "Warning"_token, true },
				{ "Error"_token, true }
			}
		{}

		[[nodiscard]] bool exists(Hash::Token logType) const noexcept
		{
			return (m_loggerStates.find(logType) != m_loggerStates.end());
		}

		void set(Hash::Token logType, bool value)
		{
			m_loggerStates[logType] = value;
		}

		[[nodiscard]] bool get(Hash::Token logType) const noexcept
		{
			return exists(logType) ? m_loggerStates.at(logType) : false;
		}

		void setAll(bool value) noexcept
		{
			for (auto& pair : m_loggerStates)
				pair.second = value;
		}

	private:
		std::unordered_map<Hash::Token, bool> m_loggerStates;
	};

	class DLL_PUBLIC Logger final
	{
	public:
		enum class OutputType: char
		{
			Cerr = 0,
			Cout,
			Clog
		};

		void addLogger(OutputType loggerOutput, std::ostream* os)
		{
			m_outputs[loggerOutput] = os;
		}

		[[nodiscard]] static OutputType getLoggerOutputType(Hash::Token logType)
		{
			if (m_loggerOutputs.find(logType) == m_loggerOutputs.end())
				throw Hash::TokenException{"getLoggerOutputType(): Unknown logger type", logType};
			return m_loggerOutputs.at(logType);
		}

		[[nodiscard]] static std::ostream& getLoggerOutputStream(Hash::Token logType)
		{
			const auto outputType = getLoggerOutputType(logType);
			if (m_outputs.find(outputType) == m_outputs.end())
				throw Hash::TokenException{"getLoggerOutputStream(): Unknown output type", logType};
			return *m_outputs.at(outputType);
		}

		[[nodiscard]] static Terminal::TextColor::TextColorEnum getLoggerColor(Hash::Token logType)
		{
			return m_colors.at(logType);
		}

		static void log(Hash::Token logType, std::string_view message, Terminal::TextColor::TextColorEnum textColor, bool newLine = true);
		
		static void log(Hash::Token logType, std::string_view message, bool newLine = true)
		{
			log(logType, message, getLoggerColor(logType), newLine);
		}

		static void logInformation(std::string_view message, bool newLine = true)
		{
			log("Information"_token, message, newLine);
		}

		static void logImportant(std::string_view message, bool newLine = true)
		{
			log("Important"_token, message, newLine);
		}

		static void logSuccess(std::string_view message, bool newLine = true)
		{
			log("Success"_token, message, newLine);
		}

		static void logDebug(std::string_view message, bool newLine = true)
		{
			log("Debug"_token, message, newLine);
		}

		static void logDetail(std::string_view message, bool newLine = true)
		{
			log("Detail"_token, message, newLine);
		}

		static void logWarning(std::string_view message, bool newLine = true)
		{
			log("Warning"_token, message, newLine);
		}

		static void logError(std::string_view message, bool newLine = true)
		{
			log("Error"_token, message, newLine);
		}

		static void logException(const std::exception& exception, std::size_t depth = 0)
		{
			CppUtils::Log::Logger::logError(std::string(depth, ' ') + exception.what());
			try
			{
				std::rethrow_if_nested(exception);
			}
			catch (const std::exception& nestedException)
			{
				logException(nestedException, depth + 1);
			}
			catch (...)
			{}
		}

		static State state;

	private:
		static std::unordered_map<OutputType, std::ostream*> m_outputs;
		static std::unordered_map<Hash::Token, OutputType> m_loggerOutputs;
		static std::unordered_map<Hash::Token, Terminal::TextColor::TextColorEnum> m_colors;
	};
}
