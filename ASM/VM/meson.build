public_dependencies = [
	cppUtils_type_dep,
	cppUtils_asm_compiler_dep
]
private_dependencies = []

includeDir = include_directories(['include/', '.'])

sources = files([

])

headers = files([
	'include/CppUtils/ASM/VM/Context.hpp',
	'include/CppUtils/ASM/VM/Operations.hpp',
	'include/CppUtils/ASM/VM/VirtualMachine.hpp'
])

cppUtils_asm_vm_dep = declare_dependency(
	include_directories: includeDir,
	dependencies: public_dependencies
)
