subdir('IR')

public_dependencies = [
	cppUtils_language_compiler_ir_dep
]
private_dependencies = []

includeDir = include_directories(['include/', '.'])

sources = files([

])

headers = files([
	'include/CppUtils/Language/Compiler/Context.hpp',
	'include/CppUtils/Language/Compiler/Compiler.hpp'
])

cppUtils_language_compiler_dep = declare_dependency(
	include_directories: includeDir,
	dependencies: public_dependencies
)
