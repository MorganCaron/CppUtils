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
subdir('Switch')
subdir('Log')
subdir('Graph')
subdir('String')
subdir('Test')
subdir('Thread')
subdir('Type')
subdir('Language')

public_dependencies = [
	platform_dep,
	chrono_dep,
	container_dep,
	external_dep,
	filesystem_dep,
	functional_dep,
	hash_dep,
	terminal_dep,
	log_dep,
	graph_dep,
	string_dep,
	test_dep,
	thread_dep,
	type_dep,
	language_dep
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
