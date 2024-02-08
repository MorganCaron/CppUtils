#pragma once

#include <iterator>
#include <locale>
#include <stdexcept>

#if __has_include(<string_view>)
#	include <string_view>

namespace Stl
{
	template<class CharT, class Traits = std::char_traits<CharT>>
	using basic_string_view = std::basic_string_view<CharT, Traits>;
}

#else
#	include <Stl/string_view.hpp>
#endif

namespace Stl
{
	// https://en.cppreference.com/w/cpp/utility/format/format_error
	class format_error: public std::runtime_error
	{
	public:
		explicit format_error(const std::string& message):
			std::runtime_error{message}
		{}
		explicit format_error(const char* message):
			std::runtime_error{message}
		{}
		format_error(const format_error&) noexcept = default;
	};

	// https://en.cppreference.com/w/cpp/utility/format/formatter
	// https://en.cppreference.com/w/cpp/named_req/Formatter
	// https://en.cppreference.com/w/cpp/named_req/BasicFormatter
	template<class T, class CharT>
	struct formatter
	{
		template<class ParseContext>
		static auto parse([[maybe_unused]] ParseContext& context) -> void
		{
			throw format_error{"unknown char"};
		}

		template<class FormatContext>
		static auto format(T value, FormatContext& context) -> decltype(context.out())
		{
			auto&& out = context.out();
			*out = static_cast<CharT>(value);
			return ++out;
		}
	};

	template<class CharT>
	struct formatter<basic_string_view<CharT>, CharT>
	{
		// Todo: implémenter parse()

		template<class FormatContext>
		static auto format(basic_string_view<CharT> string, FormatContext& context) -> decltype(context.out())
		{
			return std::copy(std::begin(string), std::end(string), context.out());
		}
	};

	template<class CharT, std::size_t N>
	struct formatter<const CharT[N], CharT>: public formatter<basic_string_view<CharT>, CharT>
	{};

	template<class CharT, std::size_t N>
	struct formatter<CharT[N], CharT>: public formatter<const CharT[N], CharT>
	{};

	template<class CharT>
	struct formatter<int, CharT>
	{
		// Todo: implémenter parse()

		template<class FormatContext>
		static auto format(int nb, FormatContext& context) -> decltype(context.out())
		{
			return formatter<basic_string_view<CharT>, CharT>::format(std::to_string(nb), context);
		}
	};

	// https://en.cppreference.com/w/cpp/utility/format/basic_format_context
	template<class OutputIt, class CharT>
	struct basic_format_context
	{
		using iterator = OutputIt;
		using char_type = CharT;

		template<class T>
		using formatter_type = formatter<T, CharT>;

		explicit basic_format_context(iterator outputIt):
			m_outputIt{outputIt}
		{}

		// Todo: implémenter arg()
		// Todo: implémenter locale()

		[[nodiscard]] inline auto out() -> iterator
		{
			return m_outputIt;
		}

		auto advance_to(iterator it) -> void
		{
			m_outputIt = it;
		}

	private:
		iterator m_outputIt;
	};

	template<class OutputIt>
	using format_context = basic_format_context<OutputIt, char>;

	template<class OutputIt>
	using wformat_context = basic_format_context<OutputIt, wchar_t>;

	namespace
	{
		template<class OutputIt, class CharT, class T>
		auto format_helper(OutputIt outputIt, basic_string_view<CharT>& inputString, const T& value) -> OutputIt
		{
			auto openBracket = inputString.find('{');
			if (openBracket == basic_string_view<CharT>::npos)
				return outputIt;
			auto closeBracket = inputString.find('}', openBracket + 1);
			if (closeBracket == basic_string_view<CharT>::npos)
				return outputIt;
			outputIt = std::copy(inputString.cbegin(), inputString.cbegin() + openBracket, outputIt);
			// Todo: gérer le contenu des {}
			auto formatContext = basic_format_context<std::back_insert_iterator<std::basic_string<CharT>>, CharT>{outputIt};
			outputIt = formatter<T, CharT>::format(value, formatContext);
			inputString = inputString.substr(closeBracket + 1);
			return outputIt;
		}
	}

	// https://en.cppreference.com/w/cpp/utility/format/basic_format_string
	template<class CharT, class... Args>
	struct basic_format_string
	{
		template<class T>
		consteval basic_format_string(const T& s):
			sv{s}
		{}

		basic_format_string(basic_string_view<CharT> s) noexcept:
			sv{s}
		{}

		constexpr basic_string_view<CharT> get() const noexcept
		{
			return sv;
		}

	private:
		basic_string_view<CharT> sv;
	};

	template<class... Args>
	using format_string = basic_format_string<char, std::type_identity_t<Args>...>;

	template<class... Args>
	using wformat_string = basic_format_string<wchar_t, std::type_identity_t<Args>...>;

	// https://en.cppreference.com/w/cpp/utility/format/format_to
	template<class OutputIt, class CharT, class... Args>
	auto format_to(OutputIt outputIt, basic_string_view<CharT> format, Args&&... args) -> OutputIt
	{
		((outputIt = format_helper(outputIt, format, args)), ...);
		outputIt = std::copy(format.cbegin(), format.cend(), outputIt);
		return outputIt;
	}

	template<class... Args>
	[[nodiscard]] inline auto format(format_string<Args...> format, Args&&... args) -> std::string
	{
		auto outputString = std::string{};
		format_to(std::back_inserter(outputString), format, args...);
		return outputString;
	}

	template<class... Args>
	[[nodiscard]] inline auto format(wformat_string<Args...> format, Args&&... args) -> std::wstring
	{
		auto outputString = std::wstring{};
		format_to(std::back_inserter(outputString), format, args...);
		return outputString;
	}

	template<class... Args>
	[[nodiscard]] inline auto format([[maybe_unused]] const std::locale& loc, format_string<Args...> format, Args&&... args) -> std::string
	{
		auto outputString = std::string{};
		format_to(std::back_inserter(outputString), format, args...);
		return outputString;
	}

	// https://en.cppreference.com/w/cpp/utility/format/format
	template<class... Args>
	[[nodiscard]] inline auto format([[maybe_unused]] const std::locale& loc, wformat_string<Args...> format, Args&&... args) -> std::wstring
	{
		auto outputString = std::wstring{};
		format_to(std::back_inserter(outputString), format, args...);
		return outputString;
	}
}
