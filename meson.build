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
	'src/DynamicLibrary.cpp'
]

headers = [
	'include/CppUtils.hpp',
	'include/Test.hpp',
	'include/Type.hpp',
	'include/Class/Cloneable.hpp',
	'include/Class/NonCopyable.hpp',
	'include/Class/NonInstanciable.hpp',
	'include/Class/NonMovable.hpp',
	'include/MeshNode.hpp',
	'include/DynamicLibrary.hpp',
	'include/LoopThread.hpp'
]

cppUtils_lib = static_library('CppUtils',
	sources: srcs + headers,
	include_directories: includeDir
)

cppUtils_dep = declare_dependency(
	include_directories: includeDir,
	link_with: cppUtils_lib
)

unitTests_exe = executable('UnitTests',
	sources: ['src/main.cpp'] + headers,
	include_directories: includeDir,
	link_with: cppUtils_lib
)

test('Unit tests', unitTests_exe)
