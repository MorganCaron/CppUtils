// Ce fichier doit être importé à la place de <windows.h>
// Ne pas importer dans un header pour ne pas tirer tout windows.h
#pragma once

#include <CppUtils/System/OS.hpp>

#if defined(OS_WINDOWS)
#	ifndef NOMINMAX
#		define NOMINMAX
#	endif
#	include <windows.h>
#endif
