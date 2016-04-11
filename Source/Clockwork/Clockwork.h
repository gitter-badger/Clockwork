

#ifndef CLOCKWORK_API_H
#define CLOCKWORK_API_H

#ifdef _MSC_VER

#pragma warning(disable: 4251)
#pragma warning(disable: 4275)

#ifdef CLOCKWORK_STATIC_DEFINE
#  define CLOCKWORK_API
#  define CLOCKWORK_NO_EXPORT
#else
#  ifndef CLOCKWORK_API
#    ifdef CLOCKWORK_EXPORTS
        /* We are building this library */
#      define CLOCKWORK_API __declspec(dllexport)
#    else
        /* We are using this library */
#      define CLOCKWORK_API __declspec(dllimport)
#    endif
#  endif

#  ifndef CLOCKWORK_NO_EXPORT
#    define CLOCKWORK_NO_EXPORT 
#  endif
#endif

#ifndef CLOCKWORK_DEPRECATED
#  define CLOCKWORK_DEPRECATED __declspec(deprecated)
#endif

#ifndef CLOCKWORK_DEPRECATED_EXPORT
#  define CLOCKWORK_DEPRECATED_EXPORT CLOCKWORK_API CLOCKWORK_DEPRECATED
#endif

#ifndef CLOCKWORK_DEPRECATED_NO_EXPORT
#  define CLOCKWORK_DEPRECATED_NO_EXPORT CLOCKWORK_NO_EXPORT CLOCKWORK_DEPRECATED
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define CLOCKWORK_NO_DEPRECATED
#endif

#else

#ifdef CLOCKWORK_STATIC_DEFINE
#ifndef CLOCKWORK_API
#  define CLOCKWORK_API
#endif
#  define CLOCKWORK_NO_EXPORT
#else
#  ifndef CLOCKWORK_API
#    ifdef CLOCKWORK_EXPORTS
        /* We are building this library */
#      define CLOCKWORK_API __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define CLOCKWORK_API __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef CLOCKWORK_NO_EXPORT
#    define CLOCKWORK_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef CLOCKWORK_DEPRECATED
#  define CLOCKWORK_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef CLOCKWORK_DEPRECATED_EXPORT
#  define CLOCKWORK_DEPRECATED_EXPORT CLOCKWORK_API CLOCKWORK_DEPRECATED
#endif

#ifndef CLOCKWORK_DEPRECATED_NO_EXPORT
#  define CLOCKWORK_DEPRECATED_NO_EXPORT CLOCKWORK_NO_EXPORT CLOCKWORK_DEPRECATED
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define CLOCKWORK_NO_DEPRECATED
#endif

#endif

#endif

