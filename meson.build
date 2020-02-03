project('CppUtils',
	'cpp',
	license: 'LGPL3',
	default_options: [
		'cpp_std=c++2a',
		'warning_level=3',
		'optimization=3',
		'werror=true'
	]
)

includeDir = include_directories('include')

srcs = [
	'src/main.cpp'
]

headers = [
	'include/CppUtils.hpp',
	'include/Test.hpp'
]

static_library('CppUtils',
	sources: headers
)

unitTests_exe = executable('UnitTests',
	sources: srcs + headers,
	include_directories: includeDir
)

test('Unit tests', unitTests_exe)
