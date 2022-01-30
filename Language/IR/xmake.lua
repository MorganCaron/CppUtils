includes("Lexer")
includes("Compiler")
includes("VirtualMachine")

target("CppUtils-Language-IR")
	set_kind("headeronly")
	add_deps(
		"CppUtils-Language-IR-Lexer",
		"CppUtils-Language-IR-Compiler",
		"CppUtils-Language-IR-VirtualMachine"
	)
