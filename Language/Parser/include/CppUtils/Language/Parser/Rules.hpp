#pragma once

#include <CppUtils/Type/Token.hpp>
#include <CppUtils/Type/Typed.hpp>

namespace CppUtils::Language::Parser
{
	using namespace Type::Literals;

	using IRule = Type::ITyped;
	template<const Type::Token& StorageToken, typename StorageType>
	using Rule = Type::Typed<StorageToken, StorageType>;
	
	static constexpr auto ExclusionRuleType = "exclusion"_token;
	using ExclusionRule = Rule<ExclusionRuleType, Type::Token>;
}
