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
subdir('Hash')
subdir('Terminal')
subdir('Log')
subdir('Test')
subdir('Thread')
subdir('Type')

public_dependencies = [
	platform_dep,
	chrono_dep,
	container_dep,
	external_dep,
	filesystem_dep,
	hash_dep,
	terminal_dep,
	log_dep,
	test_dep,
	thread_dep,
	type_dep
]
private_dependencies = []

includeDir = include_directories(['include', '.'])

sources = files([

])

headers = files([
	'include/CppUtils.hpp',
	'include/UnitTests/Container/MeshNode.hpp',
	'include/UnitTests/Container/Vector.hpp',
	'include/UnitTests/FileSystem/File/Binary.hpp',
	'include/UnitTests/FileSystem/File/String.hpp',
	'include/UnitTests/Log/Logger.hpp',
	'include/UnitTests/Test/UnitTest.hpp',
	'include/UnitTests/Type/Index.hpp'
])

cppUtils_lib = library(
	'CppUtils',
	sources + headers,
	include_directories: includeDir,
	dependencies: public_dependencies + private_dependencies
)

cppUtils_dep = declare_dependency(
	link_with: cppUtils_lib,
	include_directories: includeDir,
	dependencies: public_dependencies
)

unitTests_exe = executable(
	'UnitTests',
	sources: files(['src/main.cpp']) + headers,
	dependencies: [cppUtils_dep]
)

test('Unit tests', unitTests_exe)
