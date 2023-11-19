#pragma once

#include <print>
#include <ostream>
#include <exception>

#include <CppUtils/Hash.hpp>
#include <CppUtils/String/Concept.hpp>
#include <CppUtils/Terminal/Terminal.hpp>
#include <CppUtils/Terminal/TextModifier.hpp>

namespace CppUtils
{
	namespace
	{
		inline std::mutex loggerMutex;
	}

	template<Hash::Hasher loggerName = Hash::Hash{}>
	struct Logger final
	{
		struct Formatter final
		{
			Terminal::TextModifier textModifier;
			std::string message;
		};

		template<Hash::Hasher logType = Hash::Hash{}>
		static inline auto format(std::string_view message) -> Formatter
		{
			return {
				Terminal::TextModifier{},
				std::string{message} + '\n'
			};
		}

		template<Hash::Hasher logType = Hash::Hash{}>
		static inline auto print(std::ostream& ostream, std::string_view string) -> void
		{
			auto lock = std::unique_lock{loggerMutex};
			auto [textModifier, message] = format<logType>(string);
			// Todo C++23: std::print(ostream, "{}", std::move(message));
			ostream << message;
		}
	
		template<Hash::Hasher logType = Hash::Hash{}>
		static inline auto print(std::string_view string) -> void
		{
			using namespace Hash::Literals;
			auto lock = std::unique_lock{loggerMutex};
			auto [textModifier, message] = format<logType>(string);
			if constexpr (logType == "error"_hash)
				std::print(stderr,"{}", std::move(message));
			else
				std::print("{}", std::move(message));
		}

		template<Hash::Hasher logType = Hash::Hash{}, class... Args>
		requires (sizeof...(Args) > 0)
		static inline auto print(std::ostream& ostream, std::format_string<Args...> fmt, Args&&... args) -> void
		{
			auto lock = std::unique_lock{loggerMutex};
			auto [textModifier, message] = format<logType>(std::format(fmt, std::forward<Args>(args)...));
			// Todo C++23: std::print(ostream, "{}", std::move(message));
			ostream << message;
		}
	
		template<Hash::Hasher logType = Hash::Hash{}, class... Args>
		requires (sizeof...(Args) > 0)
		static inline auto print(std::format_string<Args...> fmt, Args&&... args) -> void
		{
			using namespace Hash::Literals;
			auto lock = std::unique_lock{loggerMutex};
			auto [textModifier, message] = format<logType>(std::format(fmt, std::forward<Args>(args)...));
			if constexpr (logType == "error"_hash)
				std::print(stderr,"{}", std::move(message));
			else
				std::print("{}", std::move(message));
		}

		template<Hash::Hasher logType = Hash::Hash{}>
		static inline auto printSeparator(std::ostream& ostream) -> void
		{
			using namespace std::literals;
			auto line = ""s;
			auto terminalWidth = Terminal::getTerminalSize().width;
			for (auto i = std::size_t{0}; i < terminalWidth; ++i)
				line += "─";
			auto lock = std::unique_lock{loggerMutex};
			auto [textModifier, message] = format<logType>("");
			// Todo C++23: std::print(ostream, "{}", std::move(line));
			ostream << line;
		}

		template<Hash::Hasher logType = Hash::Hash{}>
		static inline auto printSeparator() -> void
		{
			using namespace std::literals;
			auto line = ""s;
			auto terminalWidth = Terminal::getTerminalSize().width;
			for (auto i = std::size_t{0}; i < terminalWidth; ++i)
				line += "─";
			auto lock = std::unique_lock{loggerMutex};
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
			std::format("INFO: {}\n", message)
		};
	}

	template<>
	template<>
	auto Logger<"CppUtils">::format<"important">(std::string_view message) -> Formatter
	{
		return {
			Terminal::TextModifier{stdout, Terminal::TextColor::TextColorEnum::Cyan},
			std::format("{}\n", message)
		};
	}

	template<>
	template<>
	auto Logger<"CppUtils">::format<"success">(std::string_view message) -> Formatter
	{
		return {
			Terminal::TextModifier{stdout, Terminal::TextColor::TextColorEnum::Green},
			std::format("{}\n", message)
		};
	}

	template<>
	template<>
	auto Logger<"CppUtils">::format<"debug">(std::string_view message) -> Formatter
	{
		return {
			Terminal::TextModifier{stdout, Terminal::TextColor::TextColorEnum::Magenta},
			std::format("DEBUG: {}\n", message)
		};
	}

	template<>
	template<>
	auto Logger<"CppUtils">::format<"detail">(std::string_view message) -> Formatter
	{
		return {
			Terminal::TextModifier{stdout, Terminal::TextColor::TextColorEnum::Blue},
			std::format("{}\n", message)
		};
	}

	template<>
	template<>
	auto Logger<"CppUtils">::format<"warning">(std::string_view message) -> Formatter
	{
		return {
			Terminal::TextModifier{stdout, Terminal::TextColor::TextColorEnum::Yellow},
			std::format("WARNING: {}\n", message)
		};
	}

	template<>
	template<>
	auto Logger<"CppUtils">::format<"error">(std::string_view message) -> Formatter
	{
		return {
			Terminal::TextModifier{stderr, Terminal::TextColor::TextColorEnum::Red},
			std::format("ERROR: {}\n", message)
		};
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
