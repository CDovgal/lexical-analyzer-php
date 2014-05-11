#pragma once

#ifdef PHP_LIB_EXPORTS
#  define PHP_LIB_API __declspec(dllexport)
#else
#  define PHP_LIB_API __declspec(dllimport)
#endif
