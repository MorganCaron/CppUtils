includeDir = include_directories(['include/', '.'])

sources = files([

])

headers = files([
	'include/Chrono/Chronometer.hpp'
])

chrono_lib = library(
	'CppUtils-Chrono',
	sources + headers,
	include_directories: includeDir
)

chrono_dep = declare_dependency(
	link_with: chrono_lib,
	include_directories: includeDir
)
