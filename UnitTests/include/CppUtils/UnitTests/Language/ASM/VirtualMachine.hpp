#pragma once

#include <cstdint>

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::ASM
{
	using namespace CppUtils::Language::ASM;

	const auto tests = std::vector<Test>{

		Test{"Language/ASM/VirtualMachine/HALT", [] {
			auto vm = VirtualMachine<std::uint32_t>{};
			vm.setInstructions({
				Instruction{"HALT"_typeId}
			});
			auto context = Context{};
			vm.run(context);
		}},

		Test{"Language/ASM/VirtualMachine/NOP", [] {
			auto vm = VirtualMachine<std::uint32_t>{};
			vm.setInstructions({
				Instruction{"NOP"_typeId},
				Instruction{"HALT"_typeId}
			});
			auto context = Context{};
			vm.run(context);
		}},

		/*
		Test{"Language/ASM/VirtualMachine/MOVE", [] {
			auto vm = VirtualMachine{};
			vm.setInstructions({
				Instruction{"MOVE"_typeId, { "R0"_typeId, "42"_typeId }},
				Instruction{"HALT"_typeId}
			});
			auto context = Context{};
			vm.run(context);
		}},

		Test{"Language/ASM/VirtualMachine/PUSH and POP", [] {
			auto vm = VirtualMachine{};
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
