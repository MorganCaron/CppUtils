public_dependencies = [
	cppUtils_type_dep,
	cppUtils_language_parser_dep
]
private_dependencies = []

includeDir = include_directories(['include/', '.'])

sources = files([

])

headers = files([
	'include/CppUtils/Language/Compiler/Compiler.hpp'
])

cppUtils_language_compiler_dep = declare_dependency(
	include_directories: includeDir,
	dependencies: public_dependencies
)
