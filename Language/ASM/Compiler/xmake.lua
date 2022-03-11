target("CppUtils-Language-Asm-Compiler")
	set_kind("headeronly")
	add_deps(
		"CppUtils-Log",
		"CppUtils-Language-Parser",
		"CppUtils-Language-Lexer",
		"CppUtils-Language-Compiler",
		"CppUtils-Language-Asm-Lexer"
	)
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")
