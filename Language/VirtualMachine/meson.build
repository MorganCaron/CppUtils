public_dependencies = []
private_dependencies = []

includeDir = include_directories(['include/', '.'])

sources = files([

])

headers = files([
	'include/CppUtils/Language/VirtualMachine/Stack.hpp',
	'include/CppUtils/Language/VirtualMachine/VirtualMachine.hpp'
])

cppUtils_language_virtualMachine_dep = declare_dependency(
	include_directories: includeDir,
	dependencies: public_dependencies
)
