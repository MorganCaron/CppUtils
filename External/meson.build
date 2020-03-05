includeDir = include_directories(['include/', '.'])

sources = files([
	'src/DynamicLibrary.cpp'
])

headers = files([
	'include/External/DllExport.hpp',
	'include/External/DynamicLibrary.hpp'
])

external_lib = library(
	'CppUtils-External',
	sources + headers,
	include_directories: includeDir,
	cpp_args : (get_option('default_library') != 'static') ? '-DBUILDING_DLL' : '-DSTATIC_LIB'
)

external_dep = declare_dependency(
	link_with: external_lib,
	include_directories: includeDir
)
