set_project("CppUtils")
set_version("0.0.0", {build = "%Y%m%d%H%M"})
set_license("LGPL3")
set_languages("clatest", "cxxlatest")
set_warnings("allextra", "error")
add_cxflags("-Wpedantic -Wconversion -Wno-error=deprecated-declarations -Wfatal-errors")
-- add_cxflags("-Wno-gnu-statement-expression-from-macro-expansion -Wno-gnu-statement-expression")
set_optimize("fastest")
set_symbols("hidden")

add_rules(
	"mode.debug",
	"mode.release",
	"mode.releasedbg",
	"mode.minsizerel",
	"mode.check",
	"mode.profile",
	"mode.coverage",
	"mode.valgrind",
	"mode.asan",
	"mode.tsan",
	"mode.lsan",
	"mode.ubsan")

option("libc++_static")
	set_default(false)
	set_showmenu(true)
	set_category("libc++")
	set_description("Static link libc++ (llvm stl)")
	add_cxxflags("-stdlib=libc++", "-static", "-fexperimental-library")
	add_ldflags("-stdlib=libc++", "-static")
	add_syslinks("c++", "c++abi")
option_end()

option("libc++_shared")
	set_default(false)
	set_showmenu(true)
	set_category("libc++")
	set_description("Dynamic link libc++ (llvm stl)")
	add_cxxflags("-stdlib=libc++", "-fexperimental-library")
	add_ldflags("-stdlib=libc++")
	add_syslinks("c++")
option_end()

add_defines(is_kind("static") and "STATIC_LIB" or "SHARED_LIB")

includes(
	"Stl", 
	"Platform",
	"Random",
	"Memory",
	"System",
	"Chrono",
	"Container",
	"Symbol",
	"Error",
	"External",
	"Thread",
	"FileSystem",
	"Functional",
	"Hash",
	"Terminal",
	"String",
	"Type",
	"Log",
	"Graph",
	"Language",
	"Parameters",
	"UnitTest")

target("CppUtils")
	set_kind("$(kind)")
	if is_plat("windows") and is_config("cxx", "cl") and not is_plat("mingw") then
		set_runtimes(is_mode("debug") and "MDd" or "MD")
		add_defines("NOMINMAX", "VC_EXTRALEAN", "WIN32_LEAN_AND_MEAN", { public = true })
		add_cxflags("/wd4251", {force = true}) -- ‘identifier’ : class ‘type’ needs to have dll-interface to be used by clients of class ‘type2’
	end
	add_options("libc++_static", "libc++_shared")
	add_syslinks("pthread", "dl")
	add_deps(
		"CppUtils-Stl",
		"CppUtils-Platform",
		"CppUtils-Random",
		"CppUtils-Memory",
		"CppUtils-System",
		"CppUtils-Chrono",
		"CppUtils-Container",
		"CppUtils-Symbol",
		"CppUtils-Error",
		"CppUtils-External",
		"CppUtils-Thread",
		"CppUtils-FileSystem",
		"CppUtils-Functional",
		"CppUtils-Hash",
		"CppUtils-Terminal",
		"CppUtils-String",
		"CppUtils-Type",
		"CppUtils-Log",
		"CppUtils-Graph",
		"CppUtils-Language",
		"CppUtils-Parameters",
		"CppUtils-UnitTest"
	)
	set_policy("build.merge_archive", true)
	set_policy("build.c++.modules", true)
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")

includes("UnitTests")
