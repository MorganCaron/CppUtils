#include "Log/Logger.hpp"

namespace CppUtils
{
	std::vector<Logger::MessageType> Logger::m_enabledTypes{
		Logger::MessageType::Information,
		Logger::MessageType::Important,
		Logger::MessageType::Success,
		Logger::MessageType::Debug,
		Logger::MessageType::Detail,
		Logger::MessageType::Warning,
		Logger::MessageType::Error
	};

	std::unordered_map<Logger::OutputType, std::ostream*> Logger::m_outputs{
		{ Logger::OutputType::Cout, &std::cout },
		{ Logger::OutputType::Cerr, &std::cerr },
		{ Logger::OutputType::Clog, &std::clog }
	};

	void Logger::log(OutputType loggerOutput, MessageType logType, std::string_view message, bool newLine)
	{
		if (std::find(m_enabledTypes.begin(), m_enabledTypes.end(), logType) == m_enabledTypes.end())
			return;
		
		std::ostream& stream = *(m_outputs[loggerOutput]);

		if (message.empty())
		{
			if (newLine)
				stream << std::endl;
			return;
		}

		if (logType == MessageType::Information)
		{
			stream << ((newLine) ? (message.data() + "\n"s) : message) << std::flush;
			return;
		}

#if defined(OS_WINDOWS)
		const auto attributes = Terminal::TextModifier::getTextColor(stream);
#endif
		switch (logType)
		{
			case MessageType::Important:
				CppUtils::Terminal::TextModifier::colorize(stream, Terminal::TextModifier::TextColor::TextColorEnum::Cyan);
				break;
			case MessageType::Success:
				CppUtils::Terminal::TextModifier::colorize(stream, Terminal::TextModifier::TextColor::TextColorEnum::Green);
				break;
			case MessageType::Debug:
				CppUtils::Terminal::TextModifier::colorize(stream, Terminal::TextModifier::TextColor::TextColorEnum::Magenta);
				break;
			case MessageType::Detail:
				CppUtils::Terminal::TextModifier::colorize(stream, Terminal::TextModifier::TextColor::TextColorEnum::Blue);
				break;
			case MessageType::Warning:
				CppUtils::Terminal::TextModifier::colorize(stream, Terminal::TextModifier::TextColor::TextColorEnum::Yellow);
				break;
			case MessageType::Error:
				CppUtils::Terminal::TextModifier::colorize(stream, Terminal::TextModifier::TextColor::TextColorEnum::Red);
				break;
			default:
				CppUtils::Terminal::TextModifier::reset(stream);
				break;
		}
		stream << ((newLine) ? (message.data() + "\n"s) : message) << std::flush;
#if defined(OS_WINDOWS)
		SetConsoleTextAttribute(Terminal::getTerminalHandle(stream), attributes);
#elif defined(OS_LINUX) || defined(OS_MACOS)
		CppUtils::Terminal::TextModifier::reset(stream);
#endif
	}
}
