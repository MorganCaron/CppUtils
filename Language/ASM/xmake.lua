includes("Lexer")
includes("Compiler")
includes("VirtualMachine")

target("CppUtils-Language-Asm")
	set_kind("static")
	set_policy("build.merge_archive", true)
	add_deps(
		"CppUtils-Language-Asm-Lexer",
		"CppUtils-Language-Asm-Compiler",
		"CppUtils-Language-Asm-VirtualMachine"
	)
