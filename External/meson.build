public_dependencies = [
	cppUtils_platform_dep
]
private_dependencies = []

includeDir = include_directories(['include/', '.'])

sources = files([
	'src/DynamicLibrary.cpp'
])

headers = files([
	'include/CppUtils/External/DllExport.hpp',
	'include/CppUtils/External/DynamicLibrary.hpp'
])

cppUtils_external_lib = library(
	'CppUtils-External',
	sources + headers,
	include_directories: includeDir,
	dependencies: public_dependencies + private_dependencies,
	cpp_args : (get_option('default_library') == 'static') ? '-DSTATIC_LIB' : '-DBUILDING_DLL'
)

cppUtils_external_dep = declare_dependency(
	link_with: cppUtils_external_lib,
	include_directories: includeDir,
	dependencies: public_dependencies
)
