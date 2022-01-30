target("CppUtils-Language-Asm-VirtualMachine")
	set_kind("headeronly")
	add_deps(
		"CppUtils-Type",
		"CppUtils-Language-Asm-Compiler"
	)
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")
