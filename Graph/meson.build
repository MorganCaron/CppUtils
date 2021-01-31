public_dependencies = [
	cppUtils_type_dep,
	cppUtils_log_dep
]
private_dependencies = []

includeDir = include_directories(['include/', '.'])

sources = files([

])

headers = files([
	'include/CppUtils/Graph/MeshNode.hpp',
	'include/CppUtils/Graph/TreeNode.hpp',
	'include/CppUtils/Graph/VariantTreeNode.hpp'
])

cppUtils_graph_dep = declare_dependency(
	include_directories: includeDir,
	dependencies: public_dependencies
)
