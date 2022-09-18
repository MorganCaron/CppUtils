#pragma once

#include <iterator>
#include <string_view>
#include <type_traits>

namespace Stl
{
    // https://en.cppreference.com/w/cpp/utility/format/format_error
    class format_error: public std::runtime_error
    {
    public:
        explicit format_error(const std::string& message): std::runtime_error{message}
        {}
        explicit format_error(const char* message): std::runtime_error{message}
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
    struct formatter<std::basic_string_view<CharT>, CharT>
    {
        // Todo: implémenter parse()

        template<class FormatContext>
        static auto format(std::basic_string_view<CharT> string, FormatContext& context) -> decltype(context.out())
        {
            return std::copy(std::begin(string), std::end(string), context.out());
        }
    };

    template<class CharT, std::size_t N>
    struct formatter<const CharT[N], CharT>: public formatter<std::basic_string_view<CharT>, CharT>
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
            return formatter<std::basic_string_view<CharT>, CharT>::format(std::to_string(nb), context);
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

        [[nodiscard]] auto out() -> iterator
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
        auto format_helper(OutputIt outputIt, std::basic_string_view<CharT>& inputString, const T& value) -> OutputIt
        {
            auto openBracket = inputString.find('{');
            if (openBracket == std::basic_string_view<CharT>::npos)
                return outputIt;
            auto closeBracket = inputString.find('}', openBracket + 1);
            if (closeBracket == std::basic_string_view<CharT>::npos)
                return outputIt;
            outputIt = std::copy(inputString.cbegin(), inputString.cbegin() + openBracket, outputIt);
            // Todo: gérer le contenu des {}
            auto formatContext = basic_format_context<std::back_insert_iterator<std::basic_string<CharT>>, CharT>{outputIt};
            outputIt = formatter<T, CharT>::format(value, formatContext);
            inputString = inputString.substr(closeBracket + 1);
            return outputIt;
        }
    }

    // https://en.cppreference.com/w/cpp/utility/format/format_to
    template<class OutputIt, class CharT, class... Args>
    auto format_to(OutputIt outputIt, std::basic_string_view<CharT> format, Args&&... args) -> OutputIt
    {
        ((outputIt = format_helper(outputIt, format, args)), ...);
        outputIt = std::copy(format.cbegin(), format.cend(), outputIt);
        return outputIt;
    }

    // https://en.cppreference.com/w/cpp/utility/format/format
    template<class CharT, class... Args>
    [[nodiscard]] auto format(std::basic_string_view<CharT> format, Args&&... args) -> std::basic_string<CharT>
    {
        auto outputString = std::basic_string<CharT>{};
        format_to(std::back_inserter(outputString), format, args...);
        return outputString;
    }
}
