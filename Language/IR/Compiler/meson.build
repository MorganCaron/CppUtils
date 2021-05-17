public_dependencies = [
	cppUtils_log_dep,
	cppUtils_language_parser_dep,
	cppUtils_language_lexer_dep,
	cppUtils_language_compiler_dep,
	cppUtils_language_ir_lexer_dep
]
private_dependencies = []

includeDir = include_directories(['include/', '.'])

sources = files([

])

headers = files([
	'include/CppUtils/Language/IR/Compiler/Bytecode.hpp',
	'include/CppUtils/Language/IR/Compiler/CompilationFunctions.hpp',
	'include/CppUtils/Language/IR/Compiler/Compiler.hpp',
	'include/CppUtils/Language/IR/Compiler/Context.hpp',
	'include/CppUtils/Language/IR/Compiler/Output.hpp'
])

cppUtils_language_ir_compiler_dep = declare_dependency(
	include_directories: includeDir,
	dependencies: public_dependencies
)
