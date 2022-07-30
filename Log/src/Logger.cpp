#include <CppUtils/Log/Logger.hpp>

#include <array>

namespace CppUtils::Log
{
	using namespace Hash::Literals;

	State Logger::state{};
	
	std::unordered_map<Logger::OutputType, std::ostream*> Logger::m_outputs{
		{ Logger::OutputType::Cout, &std::cout },
		{ Logger::OutputType::Cerr, &std::cerr },
		{ Logger::OutputType::Clog, &std::clog }
	};

	std::unordered_map<Hash::Token, Logger::OutputType> Logger::m_loggerOutputs{
		{ "Information"_token, Logger::OutputType::Cout },
		{ "Important"_token, Logger::OutputType::Cout },
		{ "Success"_token, Logger::OutputType::Cout },
		{ "Debug"_token, Logger::OutputType::Cout },
		{ "Detail"_token, Logger::OutputType::Cout },
		{ "Warning"_token, Logger::OutputType::Cout },
		{ "Error"_token, Logger::OutputType::Cerr }
	};

	std::unordered_map<Hash::Token, Terminal::TextColor::TextColorEnum> Logger::m_colors{
		{ "Information"_token, Terminal::TextColor::TextColorEnum::Default },
		{ "Important"_token, Terminal::TextColor::TextColorEnum::Cyan },
		{ "Success"_token, Terminal::TextColor::TextColorEnum::Green },
		{ "Debug"_token, Terminal::TextColor::TextColorEnum::Magenta },
		{ "Detail"_token, Terminal::TextColor::TextColorEnum::Blue },
		{ "Warning"_token, Terminal::TextColor::TextColorEnum::Yellow },
		{ "Error"_token, Terminal::TextColor::TextColorEnum::Red }
	};

	void Logger::log(Hash::Token logType, std::string_view message, Terminal::TextColor::TextColorEnum textColor, bool newLine)
	{
		if (!state.get(logType))
			return;
		
		auto& stream = getLoggerOutputStream(logType);
		if (message.empty())
		{
			if (newLine)
				stream << std::endl;
			return;
		}
		
		if (textColor != Terminal::TextColor::TextColorEnum::Default)
			Terminal::TextModifier::colorize(stream, textColor);

		stream << ((newLine) ? (std::string{message} + '\n') : message) << std::flush;
		
		if (textColor != Terminal::TextColor::TextColorEnum::Default)
			Terminal::TextModifier::reset(stream);
	}
}
