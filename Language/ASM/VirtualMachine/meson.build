public_dependencies = [
	cppUtils_type_dep,
	cppUtils_language_asm_compiler_dep
]
private_dependencies = []

includeDir = include_directories(['include/', '.'])

sources = files([

])

headers = files([
	'include/CppUtils/Language/ASM/VirtualMachine/Context.hpp',
	'include/CppUtils/Language/ASM/VirtualMachine/Operations.hpp',
	'include/CppUtils/Language/ASM/VirtualMachine/VirtualMachine.hpp'
])

cppUtils_language_asm_virtualMachine_dep = declare_dependency(
	include_directories: includeDir,
	dependencies: public_dependencies
)
