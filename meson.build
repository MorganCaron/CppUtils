project('CppUtils',
	'cpp',
	version: '0.0.0',
	license: 'LGPL3',
	default_options: [
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
