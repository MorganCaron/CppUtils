target("CppUtils-Language-Compiler")
	set_kind("$(kind)")
	set_policy("build.merge_archive", true)
	add_deps(
		"CppUtils-Type",
		"CppUtils-Language-Parser",
		"CppUtils-Language-VirtualMachine"
	)
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")
