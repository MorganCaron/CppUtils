includes("Parser")
includes("Compiler")
includes("IR")
includes("Ini")
includes("Json")
includes("Markdown")
includes("Note")
includes("ASM")
includes("Xml")

target("CppUtils-Language")
	set_kind("headeronly")
	add_deps(
		"CppUtils-Language-Parser",
		"CppUtils-Language-Compiler",
		"CppUtils-Language-IR",
		"CppUtils-Language-Ini",
		"CppUtils-Language-Json",
		"CppUtils-Language-Markdown",
		"CppUtils-Language-Note",
		"CppUtils-Language-Asm",
		"CppUtils-Language-Xml"
	)
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")
