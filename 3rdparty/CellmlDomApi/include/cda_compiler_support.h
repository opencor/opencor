#ifndef compiler_support_h
#define compiler_support_h

#include "cda_config.h"

#ifdef HAVE_INTTYPES_H
#include <inttypes.h>
#else
#ifdef _MSC_VER
#undef int64_t
#undef uint64_t
#define int64_t signed __int64
#define uint64_t unsigned __int64
#endif
#endif

#ifdef _MSC_VER
#define WIN32
#undef __STDC__
#endif

#ifdef __BORLANDC__
#define WIN32
#define HEADER_INLINE
#else
#define HEADER_INLINE inline
#endif

#ifdef HAVE_VISIBILITY_ATTRIBUTE
#define CDA_EXPORT_PRE
#define CDA_EXPORT_POST __attribute__((visibility("default")))
#define WARN_IF_RETURN_UNUSED __attribute__((warn_unused_result))
#define CDA_IMPORT_PRE
#define CDA_IMPORT_POST __attribute__((visibility("default")))
#elif defined(WIN32)
#if defined(__BORLANDC__) || defined(_MSC_VER)
#define CDA_EXPORT_PRE __declspec(dllexport)
#define CDA_EXPORT_POST
#define CDA_IMPORT_PRE __declspec(dllimport)
#define CDA_IMPORT_POST
#else
#define CDA_EXPORT_PRE
#define CDA_EXPORT_POST __declspec(dllexport)
#define CDA_IMPORT_PRE
#define CDA_IMPORT_POST __declspec(dllimport)
#endif
#define WARN_IF_RETURN_UNUSED
#else
#define CDA_EXPORT_PRE
#define CDA_EXPORT_POST
#define CDA_IMPORT_PRE
#define CDA_IMPORT_POST
#define WARN_IF_RETURN_UNUSED
#endif

#ifdef _WIN32
#define swprintf _snwprintf
#endif

#endif // compiler_support_h
