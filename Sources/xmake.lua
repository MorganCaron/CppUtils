target("CppUtils")
	set_kind("$(kind)")
	if is_plat("windows") and is_config("cxx", "cl") and not is_plat("mingw") then
		set_runtimes(is_mode("debug") and "MDd" or "MD")
		add_defines("NOMINMAX", "VC_EXTRALEAN", "WIN32_LEAN_AND_MEAN", { public = true })
		add_cxflags("/wd4251", {force = true}) -- ‘identifier’ : class ‘type’ needs to have dll-interface to be used by clients of class ‘type2’
	end
	add_syslinks("pthread", "dl")
	set_policy("build.merge_archive", true)
	set_policy("build.c++.modules", true)
	add_defines("DLL_EXPORT")
	add_includedirs("Error/include", { public = true })
	add_includedirs("System/include", { public = true })
	add_files("**.mpp", { public = true })
	add_headerfiles("**.hpp")
