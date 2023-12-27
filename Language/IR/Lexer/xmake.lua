target("CppUtils-Language-IR-Lexer")
	set_kind("headeronly")
	add_deps(
		"CppUtils-Language-Compiler"
	)
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")
