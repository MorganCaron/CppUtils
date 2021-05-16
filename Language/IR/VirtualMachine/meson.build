public_dependencies = [
	cppUtils_type_dep,
	cppUtils_language_ir_compiler_dep
]
private_dependencies = []

includeDir = include_directories(['include/', '.'])

sources = files([

])

headers = files([
	'include/CppUtils/Language/IR/VirtualMachine/Context.hpp',
	'include/CppUtils/Language/IR/VirtualMachine/Operations.hpp',
	'include/CppUtils/Language/IR/VirtualMachine/ProgramMemory.hpp',
	'include/CppUtils/Language/IR/VirtualMachine/VirtualMachine.hpp'
])

cppUtils_language_ir_virtualMachine_dep = declare_dependency(
	include_directories: includeDir,
	dependencies: public_dependencies
)
