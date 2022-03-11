target("CppUtils-Language-Lexer")
	set_kind("headeronly")
	add_deps(
		"CppUtils-Type",
		"CppUtils-Graph",
		"CppUtils-Language-Parser"
	)
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")
