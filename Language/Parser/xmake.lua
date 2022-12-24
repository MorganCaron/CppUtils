target("CppUtils-Language-Parser")
	set_kind("headeronly")
	add_deps(
		"CppUtils-Hash",
		"CppUtils-Type",
		"CppUtils-Graph",
		"CppUtils-String",
		"CppUtils-Terminal"
	)
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")
