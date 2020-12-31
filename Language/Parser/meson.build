public_dependencies = [
	cppUtils_type_dep,
	cppUtils_graph_dep
]
private_dependencies = []

includeDir = include_directories(['include/', '.'])

sources = files([
	'src/Expression.cpp'
])

headers = files([
	'include/CppUtils/Language/Parser/Cursor.hpp',
	'include/CppUtils/Language/Parser/Expression.hpp',
	'include/CppUtils/Language/Parser/Parsers.hpp',
])

cppUtils_language_parser_lib = library(
	'CppUtils-Language-Parser',
	sources + headers,
	include_directories: includeDir,
	dependencies: public_dependencies + private_dependencies
)

cppUtils_language_parser_dep = declare_dependency(
	link_with: cppUtils_language_parser_lib,
	include_directories: includeDir,
	dependencies: public_dependencies
)
