#pragma once 

#define PLATFORM_WIN32 1
#define PLATFORM_LINUX 2
#define PLATFORM_APPLE 3

#define COMPILER_MSVC 1
#define COMPILER_GNUC 2
#define COMPILER_INTEL 3
#define COMPILER_CLANG 4

#define ARCHITECTURE_x86_32 1
#define ARCHITECTURE_x86_64 2


//Find the compliller that is being used and the version
#if defined( _MSC_VER )
#   define COMPILER COMPILER_MSVC
#   define COMP_VER _MSC_VER
#	define THREADLOCAL __declspec(thread)
#elif defined( __GNUC__ )
#   define COMPILER COMPILER_GNUC
#   define COMP_VER (((__GNUC__)*100) + \
        (__GNUC_MINOR__*10) + \
        __GNUC_PATCHLEVEL__)
#   define THREADLOCAL __thread
#elif defined ( __INTEL_COMPILER )
#   define COMPILER COMPILER_INTEL
#	define COMP_VER __INTEL_COMPILER
// BS_THREADLOCAL define is down below because Intel compiler defines it differently based on platform
#elif defined ( __clang__ )
#   define COMPILER COMPILER_CLANG
#	define COMP_VER __clang_major__
#   define THREADLOCAL __thread
#else
#   pragma error "No known compiler. "

#endif

// Find the platofrm of the user
#if defined( __WIN32__ ) || defined( _WIN32 )
#   define PLATFORM PLATFORM_WIN32
#elif defined( __APPLE_CC__)
#   define PLATFORM PLATFORM_APPLE
#else
#   define PLATFORM PLATFORM_LINUX
#endif

// Find the architecture type
#if defined(__x86_64__) || defined(_M_X64)
#   define ARCH_TYPE ARCHITECTURE_x86_64
#else
#   define ARCH_TYPE ARCHITECTURE_x86_32
#endif