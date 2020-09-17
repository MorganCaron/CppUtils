#pragma once

#include <vector>
#include <ostream>
#include <iostream>
#include <algorithm>
#include <unordered_map>

#include <Terminal/TextModifier/TextModifier.hpp>
#include <Switch/Switch.hpp>

namespace CppUtils::Log
{
	class Logger
	{
	public:
		enum class OutputType
		{
			Cerr = 0,
			Cout,
			Clog
		};

		using MessageType = Switch::Id;
		[[maybe_unused]] static const MessageType InformationType;
		[[maybe_unused]] static const MessageType ImportantType;
		[[maybe_unused]] static const MessageType SuccessType;
		[[maybe_unused]] static const MessageType DebugType;
		[[maybe_unused]] static const MessageType DetailType;
		[[maybe_unused]] static const MessageType WarningType;
		[[maybe_unused]] static const MessageType ErrorType;

		static inline void addLogger(OutputType loggerOutput, std::ostream* os)
		{
			m_outputs[loggerOutput] = os;
		}

		static void log(OutputType loggerOutput, MessageType logType, std::string_view message, bool newLine = true);

		static inline void logInformation(std::string_view message, bool newLine = true)
		{
			log(OutputType::Cout, InformationType, message, newLine);
		}

		static inline void logImportant(std::string_view message, bool newLine = true)
		{
			log(OutputType::Cout, ImportantType, message, newLine);
		}

		static inline void logSuccess(std::string_view message, bool newLine = true)
		{
			log(OutputType::Cout, SuccessType, message, newLine);
		}

		static inline void logDebug(std::string_view message, bool newLine = true)
		{
			log(OutputType::Cout, DebugType, message, newLine);
		}

		static inline void logDetail(std::string_view message, bool newLine = true)
		{
			log(OutputType::Cout, DetailType, message, newLine);
		}

		static inline void logWarning(std::string_view message, bool newLine = true)
		{
			log(OutputType::Cout, WarningType, message, newLine);
		}

		static inline void logError(std::string_view message, bool newLine = true)
		{
			log(OutputType::Cerr, ErrorType, message, newLine);
		}

	private:
		static std::unordered_map<OutputType, std::ostream*> m_outputs;
		static std::unordered_map<MessageType, Terminal::TextModifier::TextColor::TextColorEnum> m_colors;
	};
}
