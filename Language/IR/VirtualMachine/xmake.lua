target("CppUtils-Language-IR-VirtualMachine")
	set_kind("headeronly")
	add_deps(
		"CppUtils-String",
		"CppUtils-Language-IR-Compiler"
	)
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")
