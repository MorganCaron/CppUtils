target("CppUtils-Language-VirtualMachine")
	set_kind("static")
	set_policy("build.merge_archive", true)
	add_deps("CppUtils-Type")
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")
