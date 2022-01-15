set_project("CppUtils")
set_version("0.0.0")
set_license("LGPL3")
set_languages("clatest", "cxxlatest")
set_warnings("allextra", "error")
set_optimize("fastest")
set_symbols("hidden")

add_defines(is_kind("static") and "STATIC_LIB" or "SHARED_LIB")

if is_plat("windows") and is_config("cxx", "cl") then
	set_runtimes(is_mode("debug") and "MDd" or "MD")
	add_cxflags("/wd4251", {force = true}) -- ‘identifier’ : class ‘type’ needs to have dll-interface to be used by clients of class ‘type2’
	add_syslinks("pthread", "dl")
elseif is_plat("linux") then
	add_syslinks("pthread", "dl")
end

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

includes("Platform")
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
includes("Type")
includes("Log")
includes("Graph")
includes("String")
includes("Language")
includes("Parameters")
includes("UnitTest")

target("CppUtils")
	set_kind("$(kind)")
	set_policy("build.merge_archive", true)
	add_deps(
		"CppUtils-Platform",
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
		"CppUtils-Type",
		"CppUtils-Log",
		"CppUtils-Graph",
		"CppUtils-String",
		"CppUtils-Language",
		"CppUtils-Parameters",
		"CppUtils-UnitTest"
	)
	add_defines("DLL_EXPORT")
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")

includes("UnitTests")
