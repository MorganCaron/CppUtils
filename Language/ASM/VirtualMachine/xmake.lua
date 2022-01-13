target("CppUtils-Language-ASM-VirtualMachine")
	set_kind("static")
	set_policy("build.merge_archive", true)
	add_deps(
		"CppUtils-Type",
		"CppUtils-Language-ASM-Compiler"
	)
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")
