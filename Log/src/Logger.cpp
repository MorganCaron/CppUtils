#include "Log/Logger.hpp"

namespace CppUtils
{
	std::unordered_map<Logger::OutputType, std::experimental::observer_ptr<std::ostream>> Logger::m_outputs{
		{ Logger::OutputType::Cout, std::experimental::make_observer(&std::cout) },
		{ Logger::OutputType::Cerr, std::experimental::make_observer(&std::cerr) }
	};

	void Logger::log(OutputType loggerOutput, MessageType logType, std::string_view message)
	{
		auto textColor = Terminal::TextModifier::TextColor::Default;

		switch (logType)
		{
			case MessageType::Information: 
				textColor = Terminal::TextModifier::TextColor::Cyan;
				break;
			case MessageType::Debug: 
				textColor = Terminal::TextModifier::TextColor::Magenta;
				break;
			case MessageType::Warning: 
				textColor = Terminal::TextModifier::TextColor::Yellow;
				break;
			case MessageType::Error: 
				textColor = Terminal::TextModifier::TextColor::Red;
				break;
		}
		*(m_outputs[loggerOutput].get()) << textColor << message << Terminal::TextModifier::Reset << std::endl;
	}
}
