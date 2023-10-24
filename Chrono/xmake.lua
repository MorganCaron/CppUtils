target("CppUtils-Chrono")
	set_kind("headeronly")
	add_deps(
		"CppUtils-Stl"
	)
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")
