public_dependencies = [
	cppUtils_type_dep,
	cppUtils_graph_dep,
	cppUtils_language_parser_dep,
]
private_dependencies = []

includeDir = include_directories(['include/', '.'])

sources = files([
	'src/Lexeme.cpp'
])

headers = files([
	'include/CppUtils/Language/Lexer/Lexeme.hpp',
	'include/CppUtils/Language/Lexer/Lexer.hpp',
	'include/CppUtils/Language/Lexer/GrammarLexer.hpp',
	'include/CppUtils/Language/Lexer/ParametersLexer.hpp',
	'include/CppUtils/Language/Lexer/Parsers.hpp',
	'include/CppUtils/Language/Lexer/StringTreeLexer.hpp'
])

cppUtils_language_lexer_lib = library(
	'CppUtils-Language-Lexer',
	sources + headers,
	include_directories: includeDir,
	dependencies: public_dependencies + private_dependencies,
)

cppUtils_language_lexer_dep = declare_dependency(
	link_with: cppUtils_language_lexer_lib,
	include_directories: includeDir,
	dependencies: public_dependencies
)
