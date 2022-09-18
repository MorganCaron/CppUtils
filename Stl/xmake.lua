includes("chrono")
includes("format")

target("CppUtils-Stl")
	set_kind("headeronly")
	add_deps(
		"CppUtils-Stl-chrono",
		"CppUtils-Stl-format"
	)
