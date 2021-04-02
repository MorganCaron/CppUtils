#include <CppUtils/Log/Logger.hpp>

#include <array>

namespace CppUtils::Log
{
	using namespace Type::Literals;

	State Logger::state{};
	
	std::unordered_map<Logger::OutputType, std::ostream*> Logger::m_outputs{
		{ Logger::OutputType::Cout, &std::cout },
		{ Logger::OutputType::Cerr, &std::cerr },
		{ Logger::OutputType::Clog, &std::clog }
	};

	std::unordered_map<Type::Token, Terminal::TextModifier::TextColor::TextColorEnum, Type::Token::hash_fn> Logger::m_colors{
		{ "Important"_token, Terminal::TextModifier::TextColor::TextColorEnum::Cyan },
		{ "Success"_token, Terminal::TextModifier::TextColor::TextColorEnum::Green },
		{ "Debug"_token, Terminal::TextModifier::TextColor::TextColorEnum::Magenta },
		{ "Detail"_token, Terminal::TextModifier::TextColor::TextColorEnum::Blue },
		{ "Warning"_token, Terminal::TextModifier::TextColor::TextColorEnum::Yellow },
		{ "Error"_token, Terminal::TextModifier::TextColor::TextColorEnum::Red }
	};

	void Logger::log(OutputType loggerOutput, Type::Token logType, std::string_view message, bool newLine)
	{
		if (!state.get(logType))
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

		if (logType == "Information"_token)
		{
			stream << ((newLine) ? (std::string{message} + '\n') : message) << std::flush;
			return;
		}

#if defined(OS_WINDOWS)
		const auto attributes = Terminal::TextModifier::getTextColor(stream);
#endif
		if (m_colors.find(logType) != m_colors.end())
			Terminal::TextModifier::colorize(stream, m_colors.at(logType));
		else
			Terminal::TextModifier::reset(stream);
		stream << ((newLine) ? (std::string{message} + '\n') : message) << std::flush;
#if defined(OS_WINDOWS)
		SetConsoleTextAttribute(Terminal::getTerminalHandle(stream), attributes);
#elif defined(OS_LINUX) || defined(OS_MACOS)
		Terminal::TextModifier::reset(stream);
#endif
	}
}
