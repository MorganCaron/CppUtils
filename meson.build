project('CppUtils',
	'cpp',
	version: '0.0.0',
	license: 'LGPL3',
	default_options: [
		'c_std=c2x',
		'cpp_std=c++2a',
		'warning_level=3',
		'optimization=3',
		'werror=true'
	]
)

subdir('Platform')
subdir('Chrono')
subdir('Container')
subdir('External')
subdir('FileSystem')
subdir('Functional')
subdir('Hash')
subdir('Terminal')
subdir('Type')
subdir('Log')
subdir('Graph')
subdir('String')
subdir('Thread')
subdir('Language')
subdir('Parameters')
subdir('UnitTest')

public_dependencies = [
	cppUtils_platform_dep,
	cppUtils_chrono_dep,
	cppUtils_container_dep,
	cppUtils_external_dep,
	cppUtils_fileSystem_dep,
	cppUtils_functional_dep,
	cppUtils_hash_dep,
	cppUtils_terminal_dep,
	cppUtils_type_dep,
	cppUtils_log_dep,
	cppUtils_graph_dep,
	cppUtils_string_dep,
	cppUtils_thread_dep,
	cppUtils_language_dep,
	cppUtils_parameters_dep,
	cppUtils_unitTest_dep
]
private_dependencies = []

includeDir = include_directories(['include', '.'])

sources = files([

])

headers = files([
	'include/CppUtils.hpp'
])

cppUtils_dep = declare_dependency(
	include_directories: includeDir,
	dependencies: public_dependencies
)

subdir('UnitTests')

standardese = find_program('standardese', required: false)

documentation = custom_target(
	'docs',
	output: 'documentation-test',
	command: [
		standardese,
		'--config=../standardese.ini',
		'../include/',
		'../Chrono/include/',
		'../Container/include/',
		'../External/include/',
		'../FileSystem/include/',
		'../Functional/include/',
		'../Graph/include/',
		'../Hash/include/',
		'../Language/include/',
		'../Language/ASM/Compiler/include/',
		'../Language/ASM/Lexer/include/',
		'../Language/ASM/VirtualMachine/include/',
		'../Language/Compiler/include/',
		'../Language/Ini/include/',
		'../Language/IR/include/',
		'../Language/IR/Compiler/include/',
		'../Language/IR/Lexer/include/',
		'../Language/JSON/include/',
		'../Language/Lexer/include/',
		'../Language/Parser/include/',
		'../Language/StringTree/include/',
		'../Language/VirtualMachine/include/',
		'../Log/include/',
		'../Parameters/include/',
		'../Platform/include/',
		'../String/include/',
		'../Terminal/include/',
		'../Terminal/TextModifier/include/',
		'../Thread/include/',
		'../Type/include/',
		'../UnitTest/include/'
	],
	install: false,
	build_by_default: true
)
