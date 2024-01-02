#pragma once

#include <CppUtils/Language/VirtualMachine.hpp>

namespace CppUtils::Language
{
	template<class CharT>
	struct CompilationResult
	{
		std::size_t result;
		std::basic_string<CharT> output;
	};

	namespace LowLevelLabels
	{
		template<class CharT>
		inline constexpr auto compile(const std::basic_string_view<CharT> source) -> CompilationResult<CharT>
		{
			using namespace std::literals;
			constexpr auto lowLevelLabelsCompiler = u8R"(
				0, 1;, 3;) source length
				(0 counter
				P position for loop
				(0, 2, 0C copy counter
				(0, 4, 0C copy source length
				=, 4? (0X quit if parsing is finished
				(2; output
				(2: char
				(1; source
				(0, 5, 0C copy counter
				(4;) get char at
				(5;) push back char in output
				(0, 2, 0C copy counter
				(1+ plus one
				(0, 2C) copy counter
				J goto position for loop
			)"sv;

			auto compilationResult = CompilationResult<CharT>{};
			compilationResult.result = VirtualMachine::execute<
				std::size_t,
				bool,
				CharT,
				const decltype(source)*,
				std::basic_string<CharT>*>(
					lowLevelLabelsCompiler,
					&source,
					&compilationResult.output,
					&decltype(source)::size,
					&decltype(source)::at,
					&std::basic_string<CharT>::push_back);
			return compilationResult;
		}
	}

	/*inline constexpr auto lowLevelLabelsCompiler = u8R"(
		_3<_L <_D <_3SP <_4!	_<E
		_3<_R <_167D <_21!	_4<_L <_1SDA _<87J
		_3<_R <_42D <_51!	_4<_60A_95A_49A_83A_66A_95A_82A_60A_95A_83A_66A
		_3<_R <_4SA
		_3<_1J _<1B
	)"sv;

	inline constexpr auto highLevelLabelsCompiler = u8R"(
		main
			goto _1* define labels §
			end _<E
		
		§ define labels
			if _3<_L <_D <_3SP <_25! then goto _5*
			if _3<_R <_167DN <_25! then goto _2*
				_4<_L <_1SDA _<87J
				_4*
			§ endif
			if _3<_R <_42DN <_25! then goto _3*
				_4<_60A_95A_49A_83A_66A_95A_82A_60A_95A_83A_66A
				_4*
			§ endif
			_3<_R <_4SA
			§ next char
			_3<_1J _1* define labels
			§ end
			_*
	)"sv;

	inline constexpr auto CLanguage = std::array{

	};*/
}
