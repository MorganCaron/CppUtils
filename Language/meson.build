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
	'include/CppUtils/Language/Reader.hpp',
	'include/CppUtils/Language/Lexeme.hpp',
	'include/CppUtils/Language/Parser.hpp',
	'include/CppUtils/Language/Lexer.hpp',
	'include/CppUtils/Language/GrammarLexer.hpp',
	'include/CppUtils/Language/ParametersLexer.hpp'
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
