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
			Cout
		};

		enum class MessageType
		{
			Information = 0,
			Success,
			Debug,
			Warning,
			Error,
		};

		static inline void addLogger(OutputType loggerOutput, std::experimental::observer_ptr<std::ostream> os)
		{
			m_outputs[loggerOutput] = os;
		}

		static void logWithoutNewLine(OutputType loggerOutput, MessageType logType, std::string message);
		static void log(OutputType loggerOutput, MessageType logType, std::string message);

	private:
		static std::unordered_map<OutputType, std::experimental::observer_ptr<std::ostream>> m_outputs;
	};
}
