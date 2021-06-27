#pragma once

#include <vector>
#include <ostream>
#include <iostream>
#include <algorithm>
#include <unordered_map>

#include <CppUtils/Type/Token.hpp>
#include <CppUtils/Terminal/TextModifier/TextModifier.hpp>

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

		static void log(OutputType loggerOutput, Type::Token logType, std::string_view message, bool newLine = true);

		static inline void logInformation(std::string_view message, bool newLine = true)
		{
			log(OutputType::Cout, "Information"_token, message, newLine);
		}

		static inline void logImportant(std::string_view message, bool newLine = true)
		{
			log(OutputType::Cout, "Important"_token, message, newLine);
		}

		static inline void logSuccess(std::string_view message, bool newLine = true)
		{
			log(OutputType::Cout, "Success"_token, message, newLine);
		}

		static inline void logDebug(std::string_view message, bool newLine = true)
		{
			log(OutputType::Cout, "Debug"_token, message, newLine);
		}

		static inline void logDetail(std::string_view message, bool newLine = true)
		{
			log(OutputType::Cout, "Detail"_token, message, newLine);
		}

		static inline void logWarning(std::string_view message, bool newLine = true)
		{
			log(OutputType::Cout, "Warning"_token, message, newLine);
		}

		static inline void logError(std::string_view message, bool newLine = true)
		{
			log(OutputType::Cerr, "Error"_token, message, newLine);
		}

		static State state;

	private:
		static std::unordered_map<OutputType, std::ostream*> m_outputs;
		static std::unordered_map<Type::Token, Terminal::TextModifier::TextColor::TextColorEnum, Type::Token::hash_fn> m_colors;
	};
}
