target("CppUtils-Language-Compiler")
	set_kind("headeronly")
	add_deps(
		"CppUtils-Type",
		"CppUtils-Language-Parser"
	)
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")
