public_dependencies = []
private_dependencies = []

includeDir = include_directories(['include/', '.'])

sources = files([

])

headers = files([
	'include/CppUtils/Language/VM/Bytecode.hpp',
	'include/CppUtils/Language/VM/Cursor.hpp',
	'include/CppUtils/Language/VM/Operations.hpp',
	'include/CppUtils/Language/VM/VM.hpp'
])

language_vm_dep = declare_dependency(
	include_directories: includeDir,
	dependencies: public_dependencies
)
