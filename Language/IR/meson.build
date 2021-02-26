subdir('Compiler')
subdir('Lexer')

public_dependencies = [
	cppUtils_type_dep,
	cppUtils_graph_dep,
	cppUtils_language_lexer_dep,
	cppUtils_language_ir_compiler_dep,
	cppUtils_language_ir_lexer_dep
]
private_dependencies = []

includeDir = include_directories(['include/', '.'])

sources = files([

])

headers = files([
	'include/CppUtils/Language/IR/RegisterIRLexer.hpp',
	'include/CppUtils/Language/IR/StackIRLexer.hpp'
])

cppUtils_language_ir_dep = declare_dependency(
	include_directories: includeDir,
	dependencies: public_dependencies
)
