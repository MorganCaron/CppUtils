includes("Lexer")
includes("Compiler")
includes("VirtualMachine")

target("CppUtils-Language-Asm")
	set_kind("static")
	set_policy("build.merge_archive", true)
	add_deps(
		"CppUtils-Language-Lexer",
		"CppUtils-Language-Compiler",
		"CppUtils-Language-VirtualMachine"
	)
