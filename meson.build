project('CppUtils',
	'cpp',
	license: 'LGPL3',
	default_options: [
		'cpp_std=c++17',
		'warning_level=3',
		'optimization=3',
		'werror=true'
	]
)

subdir('Container')
subdir('External')
subdir('Test')
subdir('Thread')
subdir('Type')

public_dependencies = [
	container_dep,
	external_dep,
	test_dep,
	thread_dep,
	type_dep
]
private_dependencies = []

includeDir = include_directories(['include', '.'])

sources = files([

])

headers = files([
	'include/CppUtils.hpp'
])

cppUtils_lib = library(
	'CppUtils',
	sources + headers,
	include_directories: includeDir,
	dependencies: public_dependencies + private_dependencies,
	cpp_args : '-DBUILDING_DLL'
)

cppUtils_dep = declare_dependency(
	link_with: cppUtils_lib,
	include_directories: includeDir,
	dependencies: public_dependencies
)

unitTests_exe = executable('UnitTests',
	sources: files(['src/main.cpp']) + headers,
	dependencies: [cppUtils_dep]
)

test('Unit tests', unitTests_exe)
