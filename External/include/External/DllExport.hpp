#pragma once

#ifdef _WIN32
# ifdef STATIC_LIB
#  define DLL_PUBLIC
# else
#  ifdef BUILDING_DLL
#   define DLL_PUBLIC __declspec(dllexport)
#  else
#   define DLL_PUBLIC __declspec(dllimport)
#  endif
# endif
#else
# define DLL_PUBLIC
#endif
