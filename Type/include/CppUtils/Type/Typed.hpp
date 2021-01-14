#pragma once

#include <memory>
#include <vector>
#include <stdexcept>
#include <functional>

#include <CppUtils/External/DllExport.hpp>
#include <CppUtils/Type/Token.hpp>

namespace CppUtils::Type
{
	struct DLL_PUBLIC ITyped
	{
		virtual ~ITyped() = default;
		virtual const Token& getType() const noexcept = 0;
	};

	template<const Token& storageToken, typename StorageType>
	struct DLL_PUBLIC Typed final: public ITyped
	{
		static constexpr auto Type = storageToken;
		using Storage = StorageType;

		explicit constexpr Typed(Storage c_value):
			value{std::move(c_value)}
		{}

		const Token& getType() const noexcept override final
		{
			return Type;
		}

		Storage value;
	};

	template <typename TargetType>
	[[nodiscard]] TargetType& ensureType(const std::unique_ptr<ITyped>& typed)
	{
		if (typed->getType() != TargetType::Type)
			throw std::runtime_error{"Incorrect type. Expected type: " + std::string{TargetType::Type.name}};
		return dynamic_cast<TargetType&>(*typed);
	}
}
