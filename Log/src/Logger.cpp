#include <Log/Logger.hpp>

namespace CppUtils::Log
{
	const Logger::MessageType Logger::InformationType = Switch::newId(true);
	const Logger::MessageType Logger::ImportantType = Switch::newId(true);
	const Logger::MessageType Logger::SuccessType = Switch::newId(true);
	const Logger::MessageType Logger::DebugType = Switch::newId(true);
	const Logger::MessageType Logger::DetailType = Switch::newId(true);
	const Logger::MessageType Logger::WarningType = Switch::newId(true);
	const Logger::MessageType Logger::ErrorType = Switch::newId(true);
	
	std::unordered_map<Logger::OutputType, std::ostream*> Logger::m_outputs{
		{ Logger::OutputType::Cout, &std::cout },
		{ Logger::OutputType::Cerr, &std::cerr },
		{ Logger::OutputType::Clog, &std::clog }
	};

	std::unordered_map<Logger::MessageType, Terminal::TextModifier::TextColor::TextColorEnum> Logger::m_colors{
		{ Logger::ImportantType, Terminal::TextModifier::TextColor::TextColorEnum::Cyan },
		{ Logger::SuccessType, Terminal::TextModifier::TextColor::TextColorEnum::Green },
		{ Logger::DebugType, Terminal::TextModifier::TextColor::TextColorEnum::Magenta },
		{ Logger::DetailType, Terminal::TextModifier::TextColor::TextColorEnum::Blue },
		{ Logger::WarningType, Terminal::TextModifier::TextColor::TextColorEnum::Yellow },
		{ Logger::ErrorType, Terminal::TextModifier::TextColor::TextColorEnum::Red }
	};

	void Logger::log(OutputType loggerOutput, MessageType logType, std::string_view message, bool newLine)
	{
		if (!Switch::isEnabled(logType))
			return;
		
		if (m_outputs.find(loggerOutput) == m_outputs.end())
			throw std::out_of_range{"An unknown output stream has been passed to the logger"};
		auto& stream = *(m_outputs[loggerOutput]);

		if (message.empty())
		{
			if (newLine)
				stream << std::endl;
			return;
		}

		if (logType == InformationType)
		{
			stream << ((newLine) ? (message.data() + "\n"s) : message) << std::flush;
			return;
		}

#if defined(OS_WINDOWS)
		const auto attributes = Terminal::TextModifier::getTextColor(stream);
#endif
		if (m_colors.find(logType) != m_colors.end())
			CppUtils::Terminal::TextModifier::colorize(stream, m_colors.at(logType));
		else
			CppUtils::Terminal::TextModifier::reset(stream);
		stream << ((newLine) ? (message.data() + "\n"s) : message) << std::flush;
#if defined(OS_WINDOWS)
		SetConsoleTextAttribute(Terminal::getTerminalHandle(stream), attributes);
#elif defined(OS_LINUX) || defined(OS_MACOS)
		CppUtils::Terminal::TextModifier::reset(stream);
#endif
	}
}
