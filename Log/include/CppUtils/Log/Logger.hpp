#pragma once

#include <exception>
#include <ostream>
#include <print>

#include <CppUtils/Hash.hpp>
#include <CppUtils/String/Concept.hpp>
#include <CppUtils/Terminal/Terminal.hpp>
#include <CppUtils/Terminal/TextModifier.hpp>

// Todo: log le datetime
// Todo: log le stacktrace ( https://en.cppreference.com/w/cpp/utility/stacktrace_entry )
// Todo: rotation de logs

namespace CppUtils
{
	template<Hasher loggerName = Hash{}>
	struct Logger final
	{
		struct Formatter final
		{
			Terminal::TextModifier textModifier;
			std::string message;
		};

		template<Hasher logType = Hash{}>
		static inline auto format(std::string_view message) -> Formatter
		{
			return {
				Terminal::TextModifier{},
				std::string{message}};
		}

		template<Hasher logType = Hash{}, class... Args>
		static inline auto print(std::ostream& ostream, std::format_string<Args...> fmt, Args&&... args) -> void
		{
			auto [textModifier, message] = format<logType>(std::format(fmt, std::forward<Args>(args)...));
			// Todo C++23: std::print(ostream, "{}", std::move(message));
			ostream << message;
		}

		template<Hasher logType = Hash{}, class... Args>
		static inline auto print(std::format_string<Args...> fmt, Args&&... args) -> void
		{
			using namespace Hashing::Literals;
			auto [textModifier, message] = format<logType>(std::format(fmt, std::forward<Args>(args)...));
			if constexpr (logType == "error"_hash)
				std::print(stderr, "{}", std::move(message));
			else
				std::print("{}", std::move(message));
		}

		template<Hasher logType = Hash{}>
		static inline auto printSeparator(std::ostream& ostream) -> void
		{
			using namespace std::literals;
			auto line = ""s;
			auto terminalWidth = Terminal::getTerminalSize().width;
			for (auto i = 0uz; i < terminalWidth; ++i)
				line += "─";
			auto [textModifier, message] = format<logType>("");
			// Todo C++23: std::print(ostream, "{}", std::move(line));
			ostream << line;
		}

		template<Hasher logType = Hash{}>
		static inline auto printSeparator() -> void
		{
			using namespace std::literals;
			auto line = ""s;
			auto terminalWidth = Terminal::getTerminalSize().width;
			for (auto i = 0uz; i < terminalWidth; ++i)
				line += "─";
			auto [textModifier, message] = format<logType>("");
			std::print("{}", std::move(line));
		}
	};

	template<>
	template<>
	auto Logger<"CppUtils">::format<"info">(std::string_view message) -> Formatter
	{
		return {
			Terminal::TextModifier{stdout, Terminal::TextColor::TextColorEnum::Default},
			std::format("INFO: {}\n", message)};
	}

	template<>
	template<>
	auto Logger<"CppUtils">::format<"important">(std::string_view message) -> Formatter
	{
		return {
			Terminal::TextModifier{stdout, Terminal::TextColor::TextColorEnum::Cyan},
			std::format("{}\n", message)};
	}

	template<>
	template<>
	auto Logger<"CppUtils">::format<"success">(std::string_view message) -> Formatter
	{
		return {
			Terminal::TextModifier{stdout, Terminal::TextColor::TextColorEnum::Green},
			std::format("{}\n", message)};
	}

	template<>
	template<>
	auto Logger<"CppUtils">::format<"debug">(std::string_view message) -> Formatter
	{
		return {
			Terminal::TextModifier{stdout, Terminal::TextColor::TextColorEnum::Magenta},
			std::format("DEBUG: {}\n", message)};
	}

	template<>
	template<>
	auto Logger<"CppUtils">::format<"detail">(std::string_view message) -> Formatter
	{
		return {
			Terminal::TextModifier{stdout, Terminal::TextColor::TextColorEnum::Blue},
			std::format("{}\n", message)};
	}

	template<>
	template<>
	auto Logger<"CppUtils">::format<"warning">(std::string_view message) -> Formatter
	{
		return {
			Terminal::TextModifier{stdout, Terminal::TextColor::TextColorEnum::Yellow},
			std::format("WARNING: {}\n", message)};
	}

	template<>
	template<>
	auto Logger<"CppUtils">::format<"error">(std::string_view message) -> Formatter
	{
		return {
			Terminal::TextModifier{stderr, Terminal::TextColor::TextColorEnum::Red},
			std::format("ERROR: {}\n", message)};
	}

	auto logException(const std::exception& exception, std::size_t depth = 0) -> void
	{
		using Logger = Logger<"CppUtils">;
		if (depth == 0)
			Logger::print<"error">("{}", exception.what());
		[[maybe_unused]] auto textModifier = (depth == 0) ? Terminal::TextModifier{stdout, Terminal::TextColor::TextColorEnum::Red} : Terminal::TextModifier{};
		if (depth > 0)
			Logger::print("{}{}\n", std::string(depth * 2, ' '), exception.what());
		try
		{
			std::rethrow_if_nested(exception);
		}
		catch (const std::exception& nestedException)
		{
			logException(nestedException, depth + 1);
		}
		catch (...)
		{}
	}
}
