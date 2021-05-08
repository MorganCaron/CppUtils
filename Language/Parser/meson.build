public_dependencies = [
	cppUtils_type_dep,
	cppUtils_graph_dep,
	cppUtils_string_dep
]
private_dependencies = []

includeDir = include_directories(['include/', '.'])

sources = files([])

headers = files([
	'include/CppUtils/Language/Parser/Context.hpp',
	'include/CppUtils/Language/Parser/Cursor.hpp',
	'include/CppUtils/Language/Parser/Expression.hpp',
	'include/CppUtils/Language/Parser/Lexemes.hpp',
	'include/CppUtils/Language/Parser/Modifiers.hpp',
	'include/CppUtils/Language/Parser/Parsers.hpp'
])

cppUtils_language_parser_dep = declare_dependency(
	include_directories: includeDir,
	dependencies: public_dependencies
)
