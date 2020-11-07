#pragma once

#include <memory>
#include <vector>
#include <stdexcept>
#include <functional>

#include <External/DllExport.hpp>
#include <Type/TypeId.hpp>

using namespace std::literals;

namespace CppUtils::Type
{
	struct DLL_PUBLIC ITyped
	{
		virtual ~ITyped() = default;
		virtual const TypeId& getType() const noexcept = 0;
	};

	template<const TypeId& StorageTypeId, typename StorageType>
	struct DLL_PUBLIC Typed final: public ITyped
	{
	public:
		static constexpr auto Type = StorageTypeId;
		using Storage = StorageType;

		explicit constexpr Typed(Storage c_value):
			value{std::move(c_value)}
		{}

		const TypeId& getType() const noexcept override final
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
