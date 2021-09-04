#pragma once

#include <CppUtils/Log/Logger.hpp>
#include <CppUtils/Container/Sequence.hpp>

namespace CppUtils::Log
{
	using namespace Type::Literals;

	class TreeNodeLogger final
	{
	public:
		[[nodiscard]] static constexpr char getDifferenceChar(Container::Sequence::Difference difference) noexcept
		{
			switch (difference)
			{
				case Container::Sequence::Difference::Added:
					return '+';
				case Container::Sequence::Difference::Removed:
					return '-';
				case Container::Sequence::Difference::Edited:
					return '*';
				default:
					return ' ';
			}
		}

		[[nodiscard]] static constexpr Terminal::TextColor::TextColorEnum getDifferenceColor(Container::Sequence::Difference difference) noexcept
		{
			switch (difference)
			{
				case Container::Sequence::Difference::Added:
					return Terminal::TextColor::TextColorEnum::Green;
				case Container::Sequence::Difference::Removed:
					return Terminal::TextColor::TextColorEnum::Red;
				case Container::Sequence::Difference::Edited:
					return Terminal::TextColor::TextColorEnum::Cyan;
				default:
					return Terminal::TextColor::TextColorEnum::Default;
			}
		}

		template<typename Storage>// requires Type::Traits::isPrintable<Storage> // Fonctionne sur GCC mais pas sur Clang https://godbolt.org/z/75cja8
		static void log(const Graph::TreeNode<Storage>& treeNode, const std::string& prefix = "") noexcept
		{
			auto os = std::ostringstream{};
			os << treeNode.value;
			Log::Logger::logInformation(os.str());
			const auto nbChilds = treeNode.childs.size();
			for (auto i = 0u; i < nbChilds; ++i)
			{
				Log::Logger::log("Information"_token, prefix + ((i != nbChilds - 1) ? "├" : "└") + "─ ", Terminal::TextColor::TextColorEnum::Blue, false);
				log(treeNode.childs.at(i), prefix + ((i != nbChilds - 1) ? "│" : " ") + "  ");
			}
		}

		template<typename Storage>// requires Type::Traits::isPrintable<Storage> // Fonctionne sur GCC mais pas sur Clang https://godbolt.org/z/75cja8
		static void logDifferences(const Graph::TreeNode<Storage>& treeNode, const Graph::TreeNode<Container::Sequence::Difference>& differences, const std::string& prefix = "")
		{
			const auto& difference = differences.value;
			Log::Logger::log("Information"_token, std::string{getDifferenceChar(difference)}, getDifferenceColor(difference), false);
			auto os = std::ostringstream{};
			os << treeNode.value;
			Log::Logger::log("Information"_token, os.str(), getDifferenceColor(difference));
			const auto nbChilds = treeNode.childs.size();
			if (nbChilds != differences.childs.size())
				throw std::out_of_range{"CppUtils::Log::TreeNodeLogger::logDifferences(): the difference tree must have as many branches as the displayed tree"};
			for (auto i = 0u; i < nbChilds; ++i)
			{
				Log::Logger::log("Information"_token, prefix + ((i != nbChilds - 1) ? "├" : "└") + "─ ", Terminal::TextColor::TextColorEnum::Blue, false);
				logDifferences(treeNode.childs.at(i), prefix + ((i != nbChilds - 1) ? "│" : " ") + "  ", differences.childs.at(i));
			}
		}
	};
}
