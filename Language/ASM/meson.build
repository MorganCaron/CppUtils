public_dependencies = []
private_dependencies = []

includeDir = include_directories(['include/', '.'])

sources = files([

])

headers = files([
	'include/CppUtils/Language/ASM/Context.hpp',
	'include/CppUtils/Language/ASM/Instruction.hpp',
	'include/CppUtils/Language/ASM/Operations.hpp',
	'include/CppUtils/Language/ASM/Value.hpp',
	'include/CppUtils/Language/ASM/VM.hpp'
])

language_asm_dep = declare_dependency(
	include_directories: includeDir,
	dependencies: public_dependencies
)
