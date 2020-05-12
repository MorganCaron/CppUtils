#include "Log/Logger.hpp"

namespace CppUtils
{
	std::unordered_map<Logger::OutputType, std::experimental::observer_ptr<std::ostream>> Logger::m_outputs{
		{ Logger::OutputType::Cout, std::experimental::make_observer(&std::cout) },
		{ Logger::OutputType::Cerr, std::experimental::make_observer(&std::cerr) }
	};

	void Logger::log(OutputType loggerOutput, MessageType logType, std::string_view message)
	{
		std::ostream& stream = *(m_outputs[loggerOutput].get());
#if defined(OS_WINDOWS)
		const auto attributes = Terminal::TextModifier::getTextColor(stream);
#endif

		switch (logType)
		{
			case MessageType::Information:
				CppUtils::Terminal::TextModifier::colorize(stream, Terminal::TextModifier::TextColor::TextColorEnum::Cyan);
				break;
			case MessageType::Debug:
				CppUtils::Terminal::TextModifier::colorize(stream, Terminal::TextModifier::TextColor::TextColorEnum::Magenta);
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
		stream << message << std::endl;
#if defined(OS_WINDOWS)
		SetConsoleTextAttribute(Terminal::getTerminalHandle(stream), attributes);
#elif defined(OS_LINUX) || defined(OS_MACOS)
		CppUtils::Terminal::TextModifier::reset(stream);
#endif
	}
}
