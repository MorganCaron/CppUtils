target("CppUtils-Language-Parser")
	set_kind("headeronly")
	add_deps(
		"CppUtils-Type",
		"CppUtils-Graph",
		"CppUtils-String"
	)
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")
