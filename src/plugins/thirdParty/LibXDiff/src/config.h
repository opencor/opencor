#pragma once

#define PACKAGE "LibXDiff"
#define VERSION "0.23"
#define PACKAGE_VERSION "0.23"
#define PACKAGE_STRING "LibXDiff 0.23"

#define HAVE_LIMITS_H 1
#define HAVE_STDIO_H 1
#define HAVE_STDLIB_H 1
#define HAVE_STRING_H 1

#define HAVE_MEMCHR 1
#define HAVE_MEMCMP 1
#define HAVE_MEMCPY 1
#define HAVE_MEMSET 1
#define HAVE_STRLEN 1

#ifdef _WIN32
    #ifdef LibXDiff_PLUGIN
        #define LIBXDIFF_EXPORT __declspec(dllexport)
    #else
        #define LIBXDIFF_EXPORT __declspec(dllimport)
    #endif
#else
    #define LIBXDIFF_EXPORT
#endif
