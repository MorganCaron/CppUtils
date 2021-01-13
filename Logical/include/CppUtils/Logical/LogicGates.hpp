#pragma once

#include <CppUtils/Type/Typed.hpp>
#include <CppUtils/Type/Token.hpp>
#include <CppUtils/Graph/TreeNode.hpp>

namespace CppUtils::Logical::LogicGates
{
	using namespace Type::Literals;

	using ILogicGate = Type::ITyped;

	template<const Type::Token& StorageToken, typename StorageType>
	using LogicGate = Type::Typed<StorageToken, StorageType>;

	static constexpr auto OneInputLogicFunctionType = "oneInputLogicFunction"_token;
	using OneInputLogicFunction = LogicGate<OneInputLogicFunctionType, std::function<bool(bool)>>;

	static constexpr auto TwoInputLogicFunctionType = "twoInputLogicFunction"_token;
	using TwoInputLogicFunction = LogicGate<TwoInputLogicFunctionType, std::function<bool(bool, bool)>>;

	using Not = OneInputLogicFunction{[](bool value) -> bool {
		return !value;
	}};

	using LogicNode = Graph::TreeNode<ILogicGate>;

	template<typename T>
	struct OneInputLogicGate
	{
		T value;
	};
	
	template<typename T>
	struct TwoInputLogicGate
	{
		T lhs;
		T rhs;
	};

	static constexpr auto NotLogicGateType = "not"_token;
	static constexpr auto AndLogicGateType = "and"_token;
	static constexpr auto OrLogicGateType = "or"_token;
	static constexpr auto XorLogicGateType = "xor"_token;

	template<typename T>
	using Not = LogicGate<NotLogicGateType, ILogicGate>;

	template<typename T>
	using And = LogicGate<AndLogicGateType, ILogicGate>;

	template<typename T>
	using Or = LogicGate<OrLogicGateType, ILogicGate>;

	template<typename T>
	using Xor = LogicGate<XorLogicGateType, ILogicGate>;
}
