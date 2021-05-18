#pragma once

#include <memory>
#include <sstream>
#include <iostream>
#include <stdexcept>

#include <CppUtils/Type/Token.hpp>
#include <CppUtils/Type/Traits.hpp>
#include <CppUtils/External/DllExport.hpp>

namespace CppUtils::Type
{
	struct DLL_PUBLIC ITyped
	{
		virtual ~ITyped() = default;
		virtual const Token& getType() const noexcept = 0;
		virtual std::string getPrintable() const noexcept = 0;
	};

	std::ostream& operator<<(std::ostream& os, const ITyped& typed)
	{
		return os << typed.getPrintable();
	}

	template<const Token& storageToken, typename StorageType>
	struct DLL_PUBLIC Typed final: public ITyped
	{
		static constexpr auto Type = storageToken;
		using Storage = StorageType;

		explicit constexpr Typed(Storage c_value):
			value{std::move(c_value)}
		{}

		[[nodiscard]] const Token& getType() const noexcept override final
		{
			return Type;
		}

		[[nodiscard]] std::string getPrintable() const noexcept override final
		{
			auto ss = std::stringstream{};
			ss << storageToken.name;
			if constexpr(Traits::isDereferenceable<StorageType>)
			{
				if constexpr(Traits::isPrintable<decltype(*value)>)
					ss << ": " << *value;
			}
			else if constexpr(Traits::isPrintable<StorageType>)
				ss << ": " << value;
			return ss.str();
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
