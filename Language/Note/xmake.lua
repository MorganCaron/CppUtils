target("CppUtils-Language-Note")
	set_kind("headeronly")
	add_deps(
		"CppUtils-Language-Lexer"
	)
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")
