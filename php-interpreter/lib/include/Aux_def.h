#pragma once

#ifdef _WINDOWS

#ifdef PHP_lib_EXPORTS
#  define PHP_LIB_API __declspec(dllexport)
#else
#  define PHP_LIB_API __declspec(dllimport)
#endif

#else

#  define PHP_LIB_API

#endif
