#pragma once

#include <ostream>
#include <iostream>
#include <unordered_map>
#include <experimental/memory>

#include "Terminal/TextModifier.hpp"

namespace CppUtils
{
	class Logger
	{
	public:
		enum OutputType
		{
			Cerr = 0,
			Cout,
			Clog
		};

		enum class MessageType
		{
			Information = 0,
			Important,
			Success,
			Debug,
			Warning,
			Error,
		};

		static inline void addLogger(OutputType loggerOutput, std::experimental::observer_ptr<std::ostream> os)
		{
			m_outputs[loggerOutput] = os;
		}

		static void log(OutputType loggerOutput, MessageType logType, std::string_view message, bool newLine = true);

		static inline void logInformation(std::string_view message, bool newLine = true)
		{
			log(OutputType::Cout, MessageType::Information, message, newLine);
		}

		static inline void logImportant(std::string_view message, bool newLine = true)
		{
			log(OutputType::Cout, MessageType::Important, message, newLine);
		}

		static inline void logSuccess(std::string_view message, bool newLine = true)
		{
			log(OutputType::Cout, MessageType::Success, message, newLine);
		}

		static inline void logDebug(std::string_view message, bool newLine = true)
		{
			log(OutputType::Cout, MessageType::Debug, message, newLine);
		}

		static inline void logWarning(std::string_view message, bool newLine = true)
		{
			log(OutputType::Cout, MessageType::Warning, message, newLine);
		}

		static inline void logError(std::string_view message, bool newLine = true)
		{
			log(OutputType::Cerr, MessageType::Error, message, newLine);
		}

	private:
		static std::unordered_map<OutputType, std::experimental::observer_ptr<std::ostream>> m_outputs;
	};
}
