#pragma once

#include <iostream>
#include <exception>

#include <CppUtils/String/Concept.hpp>
#include <CppUtils/Terminal/Terminal.hpp>
#include <CppUtils/Terminal/TextModifier.hpp>

namespace CppUtils::Log
{
	class Logger final
	{
	public:
		Logger(std::ostream& outputStream):
			m_outputStream{outputStream},
			m_textColor{Terminal::TextColor::TextColorEnum::Default},
			m_backgroundColor{Terminal::BackgroundColor::BackgroundColorEnum::Default}
		{
			Terminal::TextModifier::reset(m_outputStream.get());
		}

		~Logger()
		{
			Terminal::TextModifier::reset(m_outputStream.get());
		}

		auto setOutputStream(std::ostream& outputStream) -> void
		{
			m_outputStream = outputStream;
		}

		auto operator<<(const Type::Concept::Printable auto& value) -> Logger&
		{
			m_outputStream.get() << value;
			return *this;
		}
		
		auto operator<<(Terminal::TextColor::TextColorEnum textColor) -> Logger&
		{
			m_textColor = textColor;
			Terminal::TextModifier::colorize(m_outputStream.get(), m_textColor, m_backgroundColor);
			return *this;
		}

		auto operator<<(Terminal::BackgroundColor::BackgroundColorEnum backgroundColor) -> Logger&
		{
			m_backgroundColor = backgroundColor;
			Terminal::TextModifier::colorize(m_outputStream.get(), m_textColor, m_backgroundColor);
			return *this;
		}

	private:
		std::reference_wrapper<std::ostream> m_outputStream;
		Terminal::TextColor::TextColorEnum m_textColor;
		Terminal::BackgroundColor::BackgroundColorEnum m_backgroundColor;
	};

	template<String::Concept::Char CharT = char>
	constexpr auto getSeparatorLine() -> std::basic_string<CharT>
	{
		auto line = std::basic_string<CharT>{};
		auto terminalWidth = Terminal::getTerminalSize().width;
		for (auto i = std::size_t{0}; i < terminalWidth; ++i)
			line += "─";
		return line;
	}

	auto logException(const std::exception& exception, std::ostream& outputStream = std::ref(std::cerr), std::size_t depth = 0) -> void
	{
		auto logger = Logger{outputStream};
		if (depth == 0)
			logger
				<< Terminal::TextColor::TextColorEnum::Red << "[Error]"
				<< Terminal::TextColor::TextColorEnum::Default << ": ";
		else
			logger << std::string(depth * 2, ' ');
		logger << exception.what() << '\n';
		try
		{
			std::rethrow_if_nested(exception);
		}
		catch (const std::exception& nestedException)
		{
			logException(nestedException, outputStream, depth + 1);
		}
		catch (...)
		{}
	}
}
