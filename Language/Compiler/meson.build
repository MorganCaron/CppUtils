public_dependencies = []
private_dependencies = []

includeDir = include_directories(['include/', '.'])

sources = files([

])

headers = files([

])

language_compiler_lib = library(
	'CppUtils-Language-Compiler',
	sources + headers,
	include_directories: includeDir,
	dependencies: public_dependencies + private_dependencies,
)

language_compiler_dep = declare_dependency(
	link_with: language_compiler_lib,
	include_directories: includeDir,
	dependencies: public_dependencies
)
