#pragma once

#include <vector>
#include <ostream>
#include <iostream>
#include <algorithm>
#include <unordered_map>

#include <CppUtils/Type/Token.hpp>
#include <CppUtils/Terminal/TextModifier.hpp>

namespace CppUtils::Log
{
	using namespace Type::Literals;

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

		[[nodiscard]] inline bool exists(Type::Token logType) const noexcept
		{
			return (m_loggerStates.find(logType) != m_loggerStates.end());
		}

		inline void set(Type::Token logType, bool value)
		{
			m_loggerStates[logType] = value;
		}

		[[nodiscard]] inline bool get(Type::Token logType) const noexcept
		{
			return exists(logType) ? m_loggerStates.at(logType) : false;
		}

		inline void setAll(bool value) noexcept
		{
			for (auto& pair : m_loggerStates)
				pair.second = value;
		}

	private:
		std::unordered_map<Type::Token, bool, Type::Token::hash_fn> m_loggerStates;
	};

	class Logger final
	{
	public:
		enum class OutputType: char
		{
			Cerr = 0,
			Cout,
			Clog
		};

		inline void addLogger(OutputType loggerOutput, std::ostream* os)
		{
			m_outputs[loggerOutput] = os;
		}

		[[nodiscard]] static inline OutputType getLoggerOutputType(Type::Token logType)
		{
			if (m_loggerOutputs.find(logType) == m_loggerOutputs.end())
				throw std::out_of_range{"getLoggerOutputType(): Unknown logger type " + std::string{logType.name}};
			return m_loggerOutputs.at(logType);
		}

		[[nodiscard]] static inline std::ostream& getLoggerOutputStream(Type::Token logType)
		{
			const auto outputType = getLoggerOutputType(logType);
			if (m_outputs.find(outputType) == m_outputs.end())
				throw std::out_of_range{"getLoggerOutputStream(): Unknown output type"};
			return *m_outputs.at(outputType);
		}

		[[nodiscard]] static inline Terminal::TextColor::TextColorEnum getLoggerColor(Type::Token logType)
		{
			return m_colors.at(logType);
		}

		static void log(Type::Token logType, std::string_view message, Terminal::TextColor::TextColorEnum textColor, bool newLine = true);
		
		static void log(Type::Token logType, std::string_view message, bool newLine = true)
		{
			log(logType, message, getLoggerColor(logType), newLine);
		}

		static inline void logInformation(std::string_view message, bool newLine = true)
		{
			log("Information"_token, message, newLine);
		}

		static inline void logImportant(std::string_view message, bool newLine = true)
		{
			log("Important"_token, message, newLine);
		}

		static inline void logSuccess(std::string_view message, bool newLine = true)
		{
			log("Success"_token, message, newLine);
		}

		static inline void logDebug(std::string_view message, bool newLine = true)
		{
			log("Debug"_token, message, newLine);
		}

		static inline void logDetail(std::string_view message, bool newLine = true)
		{
			log("Detail"_token, message, newLine);
		}

		static inline void logWarning(std::string_view message, bool newLine = true)
		{
			log("Warning"_token, message, newLine);
		}

		static inline void logError(std::string_view message, bool newLine = true)
		{
			log("Error"_token, message, newLine);
		}

		static State state;

	private:
		static std::unordered_map<OutputType, std::ostream*> m_outputs;
		static std::unordered_map<Type::Token, OutputType, Type::Token::hash_fn> m_loggerOutputs;
		static std::unordered_map<Type::Token, Terminal::TextColor::TextColorEnum, Type::Token::hash_fn> m_colors;
	};
}
