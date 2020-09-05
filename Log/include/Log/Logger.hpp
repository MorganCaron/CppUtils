#pragma once

#include <vector>
#include <ostream>
#include <iostream>
#include <algorithm>
#include <unordered_map>

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
			Detail,
			Warning,
			Error
		};

		static inline void addLogger(OutputType loggerOutput, std::ostream* os)
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

		static inline void logDetail(std::string_view message, bool newLine = true)
		{
			log(OutputType::Cout, MessageType::Detail, message, newLine);
		}

		static inline void logWarning(std::string_view message, bool newLine = true)
		{
			log(OutputType::Cout, MessageType::Warning, message, newLine);
		}

		static inline void logError(std::string_view message, bool newLine = true)
		{
			log(OutputType::Cerr, MessageType::Error, message, newLine);
		}

		static inline void enableType(const MessageType logType)
		{
			if (std::find(m_enabledTypes.begin(), m_enabledTypes.end(), logType) == m_enabledTypes.end())
				m_enabledTypes.push_back(logType);
		}

		static inline void disableType(const MessageType logType)
		{
			const auto it = std::find(m_enabledTypes.begin(), m_enabledTypes.end(), logType);
			if (it != m_enabledTypes.end())
				m_enabledTypes.erase(it);
		}

		template<typename... Types>
		static inline void enableType(const MessageType logType, Types... types)
		{
			enableType(logType);
			enableType(types...);
		}
		template<typename... Types>
		static inline void disableType(const MessageType logType, Types... types)
		{
			disableType(logType);
			disableType(types...);
		}

		static inline void resetEnabledTypes()
		{
			m_enabledTypes = {
				Logger::MessageType::Information,
				Logger::MessageType::Important,
				Logger::MessageType::Success,
				Logger::MessageType::Debug,
				Logger::MessageType::Detail,
				Logger::MessageType::Warning,
				Logger::MessageType::Error
			};
		}

	private:
		static std::vector<MessageType> m_enabledTypes;
		static std::unordered_map<OutputType, std::ostream*> m_outputs;
	};
}
