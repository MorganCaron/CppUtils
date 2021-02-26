public_dependencies = []
private_dependencies = [
	cppUtils_dep
]

includeDir = include_directories(['include/', '.'])

sources = files([
	'src/main.cpp'
])

headers = files([
	'include/CppUtils/UnitTests/Chrono/Chronometer.hpp',
	'include/CppUtils/UnitTests/Container/Vector.hpp',
	'include/CppUtils/UnitTests/FileSystem/File/Binary.hpp',
	'include/CppUtils/UnitTests/FileSystem/File/String.hpp',
	'include/CppUtils/UnitTests/Functional/Function.hpp',
	'include/CppUtils/UnitTests/Graph/MeshNode.hpp',
	'include/CppUtils/UnitTests/Graph/TreeNode.hpp',
	'include/CppUtils/UnitTests/Language/ASM/Lexer.hpp',
	'include/CppUtils/UnitTests/Language/ASM/VirtualMachine.hpp',
	'include/CppUtils/UnitTests/Language/Ini/IniLexer.hpp',
	'include/CppUtils/UnitTests/Language/IR/RegisterIRLexer.hpp',
	'include/CppUtils/UnitTests/Language/IR/StackIRLexer.hpp',
	'include/CppUtils/UnitTests/Language/IR/Compiler/Compiler.hpp',
	'include/CppUtils/UnitTests/Language/IR/Lexer/Lexer.hpp',
	'include/CppUtils/UnitTests/Language/Json/JsonLexer.hpp',
	'include/CppUtils/UnitTests/Language/Lexer/GrammarLexer.hpp',
	'include/CppUtils/UnitTests/Language/Lexer/Lexer.hpp',
	'include/CppUtils/UnitTests/Language/Parser/Cursor.hpp',
	'include/CppUtils/UnitTests/Language/StringTree/StringTreeLexer.hpp',
	'include/CppUtils/UnitTests/Parameters/ParametersLexer.hpp',
	'include/CppUtils/UnitTests/Log/ChronoLogger.hpp',
	'include/CppUtils/UnitTests/Log/Logger.hpp',
	'include/CppUtils/UnitTests/String/String.hpp',
	'include/CppUtils/UnitTests/Switch/Switch.hpp',
	'include/CppUtils/UnitTests/Test/Test.hpp',
	'include/CppUtils/UnitTests/Type/Typed.hpp',
	'include/CppUtils/UnitTests/Type/Token.hpp'
])

cppUtils_unitTests_exe = executable(
	'UnitTests',
	sources: sources + headers,
	include_directories: includeDir,
	dependencies: public_dependencies + private_dependencies
)

test('Unit tests', cppUtils_unitTests_exe)
