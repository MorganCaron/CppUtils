target("CppUtils-Parameters")
	set_kind("headeronly")
	add_deps(
		"CppUtils-Language-Parser",
		"CppUtils-Language-Lexer"
	)
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")
