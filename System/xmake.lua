target("CppUtils-System")
	set_kind("headeronly")
	add_deps("CppUtils-Platform")
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")
