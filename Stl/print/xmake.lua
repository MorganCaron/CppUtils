target("CppUtils-Stl-print")
	set_kind("headeronly")
	add_deps(
		"CppUtils-Stl-format"
	)
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")
