set_project("CppUtils")
set_version("0.0.0")
set_license("LGPL3")
set_languages("clatest", "cxxlatest")
set_warnings("everything", "error")
set_optimize("fastest")

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
	set_kind("static")
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
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")

includes("UnitTests")
