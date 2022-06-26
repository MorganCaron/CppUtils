set_project("CppUtils")
set_version("0.0.0")
set_license("LGPL3")
set_languages("clatest", "cxxlatest")
set_warnings("allextra", "error")
add_cxflags("-Wno-error=deprecated-declarations")
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
	"mode.ubsan"
)

add_defines(is_kind("static") and "STATIC_LIB" or "SHARED_LIB")

package("CppUtils")
	set_homepage("https://github.com/MorganCaron/CppUtils")
	set_description("C++ Utilities Library for compilers or various projects")
	set_license("LGPL3")

	if is_plat("windows") and is_config("cxx", "cl") then
		set_runtimes(is_mode("debug") and "MDd" or "MD")
		add_cxflags("/wd4251", {force = true}) -- ‘identifier’ : class ‘type’ needs to have dll-interface to be used by clients of class ‘type2’
		add_syslinks("pthread", "dl")
	elseif is_plat("linux") then
		add_syslinks("pthread", "dl")
	end

	includes("Platform")
	includes("Random")
	includes("Memory")
	includes("System")
	includes("Chrono")
	includes("Container")
	includes("External")
	includes("Thread")
	includes("FileSystem")
	includes("Functional")
	includes("Hash")
	includes("Terminal")
	includes("String")
	includes("Type")
	includes("Log")
	includes("Graph")
	includes("Language")
	includes("Parameters")
	includes("UnitTest")

	target("CppUtils")
		set_kind("$(kind)")
		add_deps(
			"CppUtils-Platform",
			"CppUtils-Random",
			"CppUtils-Memory",
			"CppUtils-System",
			"CppUtils-Chrono",
			"CppUtils-Container",
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
		add_includedirs("include", { public = true })
		add_headerfiles("include/**.hpp")

includes("UnitTests")
