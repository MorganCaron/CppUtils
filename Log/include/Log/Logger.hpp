#pragma once

#include <ostream>
#include <iostream>
#include <functional>
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
			Cout
		};

		enum class MessageType
		{
			Information = 0,
			Debug,
			Warning,
			Error,
		};

		static inline void addLogger(OutputType loggerOutput, std::ostream& os)
		{
			m_outputs[loggerOutput] = os;
		}

		static void log(OutputType loggerOutput, MessageType logType, std::string_view message);

	private:
		static std::unordered_map<OutputType, std::reference_wrapper<std::ostream>> m_outputs;
	};
}
