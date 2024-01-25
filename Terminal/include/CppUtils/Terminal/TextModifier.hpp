#pragma once

#include <iostream>
#include <print>
#include <string_view>

#include <CppUtils/Platform/DllExport.hpp>
#include <CppUtils/Terminal/BackgroundColor.hpp>
#include <CppUtils/Terminal/Terminal.hpp>
#include <CppUtils/Terminal/TextColor.hpp>
#include <CppUtils/Terminal/TextStyle.hpp>

namespace CppUtils::Terminal
{
	using namespace std::literals;

#if defined(OS_LINUX) || defined(OS_MACOS)
	namespace ANSIEscapeCode
	{
		[[maybe_unused]] inline constexpr auto Reset = "\x1B[0m"sv;
		[[maybe_unused]] inline constexpr auto EraseCurrentLine = "\x1B[2K0E"sv;
	}
#endif

	class DLL_PUBLIC TextModifier final
	{
	public:
		TextModifier() = default;

		explicit TextModifier(
			std::FILE* file,
			TextColor::TextColorEnum textColor = TextColor::TextColorEnum::Default,
			BackgroundColor::BackgroundColorEnum backgroundColor = BackgroundColor::BackgroundColorEnum::Default,
			[[maybe_unused]] std::optional<TextStyle::TextStyleEnum> textStyle = std::nullopt);

		TextModifier(TextModifier&& other) noexcept = default;
		TextModifier& operator=(TextModifier&& other) noexcept = default;

		~TextModifier();

#if defined(OS_WINDOWS)
		[[nodiscard]] static auto getTextColor(std::FILE* file) -> std::uint16_t;
		[[nodiscard]] static auto getTextColor(std::ostream& stream) -> std::uint16_t;
#endif

		static auto eraseCurrentLine([[maybe_unused]] std::ostream& stream) -> void
		{
#if defined(OS_LINUX) || defined(OS_MACOS)
			stream << ANSIEscapeCode::EraseCurrentLine;
#endif
		}

	private:
		bool m_modify = false;
		std::FILE* m_file = nullptr;
#if defined(OS_WINDOWS)
		std::uint16_t m_attributes;
#endif
	};
}
