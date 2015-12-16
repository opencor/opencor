#ifndef TYPEDOBJECT_EXPORT_H
#define TYPEDOBJECT_EXPORT_H

#ifdef TYPEDOBJECT_STATIC_DEFINE
#  define TYPEDOBJECT_EXPORT
#  define TYPEDOBJECT_NO_EXPORT
#else
#  ifndef TYPEDOBJECT_EXPORT
#    ifdef typedobject_EXPORTS
        /* We are building this library */
#      ifdef WIN32
#        define TYPEDOBJECT_EXPORT __declspec(dllexport)
#      else
#        define TYPEDOBJECT_EXPORT __attribute__((visibility("default")))
#      endif
#    else
        /* We are using this library */
#      ifdef WIN32
#        define TYPEDOBJECT_EXPORT __declspec(dllimport)
#      else
#        define TYPEDOBJECT_EXPORT __attribute__((visibility("default")))
#      endif
#    endif
#  endif

#  ifndef TYPEDOBJECT_NO_EXPORT
#    ifdef WIN32
#      define TYPEDOBJECT_NO_EXPORT 
#    else
#      define TYPEDOBJECT_NO_EXPORT __attribute__((visibility("hidden")))
#    endif
#  endif
#endif

#ifndef TYPEDOBJECT_DEPRECATED
#  ifdef WIN32
#    define TYPEDOBJECT_DEPRECATED __declspec(deprecated)
#  else
#    define TYPEDOBJECT_DEPRECATED __attribute__ ((__deprecated__))
#  endif
#endif

#ifndef TYPEDOBJECT_DEPRECATED_EXPORT
#  define TYPEDOBJECT_DEPRECATED_EXPORT TYPEDOBJECT_EXPORT TYPEDOBJECT_DEPRECATED
#endif

#ifndef TYPEDOBJECT_DEPRECATED_NO_EXPORT
#  define TYPEDOBJECT_DEPRECATED_NO_EXPORT TYPEDOBJECT_NO_EXPORT TYPEDOBJECT_DEPRECATED
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define TYPEDOBJECT_NO_DEPRECATED
#endif

#endif
