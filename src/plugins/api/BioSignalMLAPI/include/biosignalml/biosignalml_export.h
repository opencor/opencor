
#ifndef BIOSIGNALML_EXPORT_H
#define BIOSIGNALML_EXPORT_H

#ifdef BIOSIGNALML_STATIC_DEFINE
#  define BIOSIGNALML_EXPORT
#  define BIOSIGNALML_NO_EXPORT
#else
#  ifndef BIOSIGNALML_EXPORT
#    ifdef biosignalml_EXPORTS
        /* We are building this library */
#      define BIOSIGNALML_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define BIOSIGNALML_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef BIOSIGNALML_NO_EXPORT
#    define BIOSIGNALML_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef BIOSIGNALML_DEPRECATED
#  define BIOSIGNALML_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef BIOSIGNALML_DEPRECATED_EXPORT
#  define BIOSIGNALML_DEPRECATED_EXPORT BIOSIGNALML_EXPORT BIOSIGNALML_DEPRECATED
#endif

#ifndef BIOSIGNALML_DEPRECATED_NO_EXPORT
#  define BIOSIGNALML_DEPRECATED_NO_EXPORT BIOSIGNALML_NO_EXPORT BIOSIGNALML_DEPRECATED
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define BIOSIGNALML_NO_DEPRECATED
#endif

#endif
