set_project("CppUtils")
set_version("0.0.0", {build = "%Y%m%d%H%M"})
set_license("LGPL3")
set_languages("clatest", "cxxlatest")
set_warnings("allextra", "error")
add_cxflags("-Wpedantic -Wconversion -Wno-error=deprecated-declarations -Wfatal-errors")
-- add_cxflags("-Wno-gnu-statement-expression-from-macro-expansion -Wno-gnu-statement-expression")
set_optimize("fastest")

set_policy("check.auto_ignore_flags", false)
add_cxxflags("-fexperimental-library") -- Pour avoir std::jthread

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

add_defines(is_kind("static") and "STATIC_LIB" or "SHARED_LIB")

includes("Sources")

includes("UnitTests")
