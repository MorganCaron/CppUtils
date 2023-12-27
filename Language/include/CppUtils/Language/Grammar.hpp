#pragma once

#include <CppUtils/Language/VirtualMachine.hpp>

namespace CppUtils::Language
{
	namespace LowLevelLabels
	{
		constexpr auto compile(const auto source, auto& output) -> std::size_t
		{
			using namespace std::literals;
			constexpr auto lowLevelLabelsCompiler = u8R"(
				0, 1;, 2;, 
			)"sv;
			return VirtualMachine::execute<std::size_t, bool, const decltype(source)*, decltype(output)*>(
				lowLevelLabelsCompiler, &source, &output,
				&decltype(source)::at);
		}
	}

	/*
	inline constexpr auto lowLevelLabelsCompiler = u8R"(
		_3<_L <_D <_3SP <_4!	_<E
		_3<_R <_167D <_21!	_4<_L <_1SDA _<87J
		_3<_R <_42D <_51!	_4<_60A_95A_49A_83A_66A_95A_82A_60A_95A_83A_66A
		_3<_R <_4SA
		_3<_1J _<1B
	)"sv;
	*/

	/*
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
	*/

	/*
	constexpr auto CLanguage = std::array{

	};
	*/
}
