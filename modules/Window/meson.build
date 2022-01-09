wayland_client_dep = dependency('wayland-client')
wayland_protocols_dep = dependency('wayland-protocols')
#wayland_cursor_dep = dependency('wayland-cursor')

private_dependencies = [
	wayland_client_dep,
	wayland_protocols_dep#,
	#wayland_cursor_dep
]

cppUtils_window_lib = library(
	'CppUtils-Window',
	dependencies: private_dependencies,
)
