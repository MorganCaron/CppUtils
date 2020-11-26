#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::VM
{
	using namespace CppUtils::Language::VM;

	const auto tests = std::vector<CppUtils::Test::UnitTest>{

		CppUtils::Test::UnitTest("Language/VM/HLT", [] {
			auto instructions = std::vector<Bytecode::Instruction>{
				Bytecode::Instruction{Bytecode::InstructionType::HLT}
			};
			const auto vm = VM::VM{std::move(instructions)};
			vm.run();
		}),

		CppUtils::Test::UnitTest("Language/VM/NOP", [] {
			auto instructions = std::vector<Bytecode::Instruction>{
				Bytecode::Instruction{Bytecode::InstructionType::NOP},
				Bytecode::Instruction{Bytecode::InstructionType::HLT}
			};
			const auto vm = VM::VM{std::move(instructions)};
			vm.run();
		})

	};
}
