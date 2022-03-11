target("CppUtils-Graph")
	set_kind("headeronly")
	add_deps(
		"CppUtils-Type",
		"CppUtils-Log"
	)
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")
