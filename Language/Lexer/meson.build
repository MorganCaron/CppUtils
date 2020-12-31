public_dependencies = [
	cppUtils_type_dep,
	cppUtils_graph_dep,
	cppUtils_language_parser_dep
]
private_dependencies = []

includeDir = include_directories(['include/', '.'])

sources = files([])

headers = files([
	'include/CppUtils/Language/Lexer/Lexer.hpp',
	'include/CppUtils/Language/Lexer/GrammarLexer.hpp',
	'include/CppUtils/Language/Lexer/ParametersLexer.hpp',
	'include/CppUtils/Language/Lexer/StringTreeLexer.hpp'
])

cppUtils_language_lexer_dep = declare_dependency(
	include_directories: includeDir,
	dependencies: public_dependencies
)
