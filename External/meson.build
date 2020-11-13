includeDir = include_directories(['include/', '.'])

sources = files([
	'src/DynamicLibrary.cpp'
])

headers = files([
	'include/CppUtils/External/DllExport.hpp',
	'include/CppUtils/External/DynamicLibrary.hpp'
])

external_lib = library(
	'CppUtils-External',
	sources + headers,
	include_directories: includeDir,
	cpp_args : (get_option('default_library') == 'static') ? '-DSTATIC_LIB' : '-DBUILDING_DLL'
)

external_dep = declare_dependency(
	link_with: external_lib,
	include_directories: includeDir
)
