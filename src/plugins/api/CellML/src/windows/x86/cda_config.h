/* Is the CellML Context enabled? */
/* #undef ENABLE_CONTEXT */

/* Are the GSL integrators enabled? */
/* #undef ENABLE_GSL_INTEGRATORS */

/* Is Java enabled? */
#define ENABLE_JAVA_BUILD
#define ENABLE_JAVA ENABLE_JAVA_BUILD

/* Is Python enabled? */
#define ENABLE_PYTHON

/* Is RDF enabled? */
#define ENABLE_RDF

/* Is SProS enabled? */
#define ENABLE_SPROS

/* Is SRuS enabled? */
#undef ENABLE_SRUS

/* Is TeLiCeMs enabled? */
#define ENABLE_TELICEMS

/* Is XPCOM enabled? */
/* #undef ENABLE_XPCOM */

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_HEADER_INTTYPES_H

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H

/* Define to 1 if you have the <JavaVM/jni.h> header file. */
/* #undef HAVE_JAVAVM_JNI_H */

/* Define to 1 if you have the <jni.h> header file. */
#define HAVE_JNI_H

/* Define to 1 if you have the `gsl' library (-lgsl). */
/* #undef HAVE_LIBGSL */

/* Define to 1 if you have the `gslcblas' library (-lgslcblas). */
/* #undef HAVE_LIBGSLCBLAS */

/* Define if gcc visibility attributes supported */
/* #undef HAVE_VISIBILITY_ATTRIBUTE */

/* The size of `wchar_t*', as computed by sizeof. */
#define SIZEOF_WCHAR_TP 2

/* Define TESTDIR to path of test sources. */
#define TESTDIR L"C:/build/mingw32-buildslave/mingw32-a-api/build/tests"

/* Define TESTDIR to path of test sources. */
#define TESTDIR8 "C:/build/mingw32-buildslave/mingw32-a-api/build/tests"

#define SIZE_INT64_T 8
#if ! (SIZE_INT64_T + 0)
#undef int64_t
#ifdef WIN32
typedef signed __int64 int64_t;
#else
typedef signed long long int64_t;
#endif
#endif
#define SIZE_UINT64_T 8
#if ! (SIZE_UINT64_T + 0)
#undef uint64_t
#ifdef WIN32
typedef unsigned __int64 uint64_t;
#else
typedef unsigned long long uint64_t;
#endif
#endif
#define SIZE_INT32_T 4
#if ! (SIZE_INT32_T + 0)
typedef signed long int32_t;
#endif
#define SIZE_UINT32_T 4
#if ! (SIZE_INT32_T + 0)
typedef unsigned long uint32_t;
#endif
#define SIZE_INT16_T 2
#if ! (SIZE_INT16_T + 0)
typedef short int16_t;
#endif
#define SIZE_UINT16_T 2
#if ! (SIZE_UINT16_T + 0)
typedef unsigned short uint16_t;
#endif
#define SIZE_INT8_T 
#if ! (SIZE_INT8_T + 0)
typedef signed char int8_t;
#endif
#define SIZE_UINT8_T 
#if ! (SIZE_UINT8_T + 0)
typedef unsigned char uint8_t;
#endif
