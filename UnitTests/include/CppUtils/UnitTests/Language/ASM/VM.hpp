#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::ASM
{
	using namespace CppUtils::Language::ASM;

	const auto tests = std::vector<Test::UnitTest>{

		Test::UnitTest{"Language/ASM/VM/HALT", [] {
			auto vm = VM{};
			vm.setInstructions({
				Instruction{"HALT"_typeId}
			});
			auto context = Context{};
			vm.run(context);
		}},

		Test::UnitTest{"Language/ASM/VM/NOP", [] {
			auto vm = VM{};
			vm.setInstructions({
				Instruction{"NOP"_typeId},
				Instruction{"HALT"_typeId}
			});
			auto context = Context{};
			vm.run(context);
		}},

		/*
		Test::UnitTest{"Language/ASM/VM/MOVE", [] {
			auto vm = VM{};
			vm.setInstructions({
				Instruction{"MOVE"_typeId, { "R0"_typeId, "42"_typeId }},
				Instruction{"HALT"_typeId}
			});
			auto context = Context{};
			vm.run(context);
		}},

		Test::UnitTest{"Language/ASM/VM/PUSH and POP", [] {
			auto vm = VM{};
			vm.setInstructions({
				Instruction{"MOVE"_typeId, { "R0"_typeId, "42"_typeId }},
				Instruction{"PUSH"_typeId, { "R0"_typeId }},
				Instruction{"POP"_typeId, { "R1"_typeId }},
				Instruction{"HALT"_typeId}
			});
			auto context = Context{};
			vm.run(context);
		}}
		*/

	};
}
