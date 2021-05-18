public_dependencies = [
	cppUtils_thread_dep
]
private_dependencies = []

includeDir = include_directories(['include/', '.'])

sources = files([

])

headers = files([
	'include/CppUtils/FileSystem/File.hpp',
	'include/CppUtils/FileSystem/FileWatcher.hpp'
])

cppUtils_fileSystem_dep = declare_dependency(
	include_directories: includeDir,
	dependencies: public_dependencies
)
