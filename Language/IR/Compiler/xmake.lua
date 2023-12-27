target("CppUtils-Language-IR-Compiler")
	set_kind("headeronly")
	add_deps(
		"CppUtils-Log",
		"CppUtils-Hash",
		"CppUtils-Language-Parser",
		"CppUtils-Language-Compiler",
		"CppUtils-Language-IR-Lexer"
	)
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")
