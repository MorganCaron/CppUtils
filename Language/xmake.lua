includes("Parser")
includes("Lexer")
includes("VirtualMachine")
includes("Compiler")
includes("IR")
includes("Ini")
includes("Json")
includes("Markdown")
includes("Note")
includes("ASM")
includes("Xml")

target("CppUtils-Language")
	set_kind("$(kind)")
	set_policy("build.merge_archive", true)
	add_deps(
		"CppUtils-Language-Parser",
		"CppUtils-Language-Lexer",
		"CppUtils-Language-VirtualMachine",
		"CppUtils-Language-Compiler",
		"CppUtils-Language-IR",
		"CppUtils-Language-Ini",
		"CppUtils-Language-Json",
		"CppUtils-Language-Markdown",
		"CppUtils-Language-Note",
		"CppUtils-Language-Asm",
		"CppUtils-Language-Xml"
	)
