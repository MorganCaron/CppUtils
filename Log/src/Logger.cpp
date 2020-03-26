#include "Log/Logger.hpp"

namespace CppUtils
{
	std::unordered_map<Logger::OutputType, std::reference_wrapper<std::ostream>> Logger::m_outputs{
		{ Logger::OutputType::Cerr, std::cerr },
		{ Logger::OutputType::Cout, std::cout }
	};

	void Logger::log(OutputType loggerOutput, MessageType logType, std::string_view message)
	{
		auto textColor = Terminal::TextModifier::SGRCode::TextColor_Default;

		switch (logType)
		{
			case MessageType::Information: 
				textColor = Terminal::TextModifier::SGRCode::TextColor_Cyan;
				break;
			case MessageType::Debug: 
				textColor = Terminal::TextModifier::SGRCode::TextColor_Magenta;
				break;
			case MessageType::Warning: 
				textColor = Terminal::TextModifier::SGRCode::TextColor_Yellow;
				break;
			case MessageType::Error: 
				textColor = Terminal::TextModifier::SGRCode::TextColor_Red;
				break;
		}
		m_outputs[loggerOutput]
			<< Terminal::TextModifier({ textColor })
			<< message
			<< Terminal::TextModifier::Reset;
	}
}
