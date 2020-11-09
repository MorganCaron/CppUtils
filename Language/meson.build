public_dependencies = [
	type_dep,
	graph_dep
]
private_dependencies = []

includeDir = include_directories(['include/', '.'])

sources = files([
	'src/Lexeme.cpp'
])

headers = files([
	'include/Language/GrammarLexer.hpp',
	'include/Language/Lexeme.hpp',
	'include/Language/Lexer.hpp',
	'include/Language/ParametersLexer.hpp',
	'include/Language/Parser.hpp',
	'include/Language/Parsers.hpp'
])

language_lib = library(
	'CppUtils-Language',
	sources + headers,
	include_directories: includeDir,
	dependencies: public_dependencies + private_dependencies,
)

language_dep = declare_dependency(
	link_with: language_lib,
	include_directories: includeDir,
	dependencies: public_dependencies
)
