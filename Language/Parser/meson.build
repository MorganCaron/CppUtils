public_dependencies = []
private_dependencies = []

includeDir = include_directories(['include/', '.'])

sources = files([

])

headers = files([
	'include/CppUtils/Language/Parser/Cursor.hpp'
])

language_parser_lib = library(
	'CppUtils-Language-Parser',
	sources + headers,
	include_directories: includeDir,
	dependencies: public_dependencies + private_dependencies,
)

language_parser_dep = declare_dependency(
	link_with: language_parser_lib,
	include_directories: includeDir,
	dependencies: public_dependencies
)
