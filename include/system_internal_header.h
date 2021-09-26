//
// file:			system_internal_header.h
// path:			include/SYSTEM_internal_header.h
// created on:		2021 Sep 26
// created by:		Davit Kalantaryan (davit.kalantaryan@gmail.com)
//

#ifndef INCLUDE_SYSTEM_INTERNAL_HEADER_H
#define INCLUDE_SYSTEM_INTERNAL_HEADER_H

#include <stddef.h>

#ifdef _MSC_VER
    #define SYSTEM_UNREACHABLE_CODE(_code)
	//#if _MSC_FULL_VER
	#if (_MSC_VER>1900) // 1900 is VS2015
		#pragma warning (disable:5045)
	#endif
	// assignment within conditional expression (https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-4-c4706?view=msvc-160)
	#pragma warning (disable:4706) 
	// https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-1-c4711?view=msvc-160
	#pragma warning (disable:4711)
	// Function not inlined althou defined to be inline. 
	// this can happen also with library functions, so this should be disabled
	// https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-1-c4711?view=msvc-160
	#pragma warning (disable:4710)
    #define SYSTEM_BEFORE_CPP_17_FALL_THR
	#if defined(_MSVC_LANG) && (_MSVC_LANG>=201100L)
        #define SYSTEM_CPP_11_DEFINED		1
	#endif
	#if defined(_MSVC_LANG) && (_MSVC_LANG>=201400L)
        #define SYSTEM_CPP_14_DEFINED		1
	#endif
	#if defined(_MSVC_LANG) && (_MSVC_LANG>=201700L)
        #define SYSTEM_CPP_17_DEFINED		1
	#endif
    #define SYSTEM_DLL_PUBLIC		__declspec(dllexport)
    #define SYSTEM_DLL_PRIVATE
    #define SYSTEM_IMPORT_FROM_DLL	__declspec(dllimport)
    #define SYSTEM_THREAD_LOCAL		__declspec(thread)
#elif defined(__GNUC__) || defined(__clang__)
    #define SYSTEM_UNREACHABLE_CODE(_code)	_code ;
    #define SYSTEM_BEFORE_CPP_17_FALL_THR	__attribute__ ((fallthrough)) ;
	#if defined(__cplusplus) && (__cplusplus>=201100L)
        #define SYSTEM_CPP_11_DEFINED		1
	#endif
	#if defined(__cplusplus) && (__cplusplus>=201400L)
        #define SYSTEM_CPP_14_DEFINED		1
	#endif
	#if defined(__cplusplus) && (__cplusplus>=201700L)
        #define SYSTEM_CPP_17_DEFINED		1
	#endif
    //#define SYSTEM_DLL_PUBLIC		__attribute__((visibility("default")))
    #define SYSTEM_DLL_PUBLIC
    #define SYSTEM_DLL_PRIVATE		__attribute__((visibility("hidden")))
    #define SYSTEM_IMPORT_FROM_DLL
    #define SYSTEM_THREAD_LOCAL		__thread
#elif defined(__CYGWIN__)
    #define SYSTEM_UNREACHABLE_CODE(_code)	_code ;
    #define SYSTEM_BEFORE_CPP_17_FALL_THR	__attribute__ ((fallthrough)) ;
	#if defined(__cplusplus) && (__cplusplus>=201100L)
        #define SYSTEM_CPP_11_DEFINED		1
	#endif
	#if defined(__cplusplus) && (__cplusplus>=201400L)
        #define SYSTEM_CPP_14_DEFINED		1
	#endif
	#if defined(__cplusplus) && (__cplusplus>=201700L)
        #define SYSTEM_CPP_17_DEFINED		1
	#endif
    #define SYSTEM_DLL_PUBLIC		__attribute__((dllexport))
    #define SYSTEM_DLL_PRIVATE
    #define SYSTEM_IMPORT_FROM_DLL	__attribute__((dllimport))
#elif defined(__MINGW64__) || defined(__MINGW32__)
    #define SYSTEM_UNREACHABLE_CODE(_code)	_code ;
    #define SYSTEM_BEFORE_CPP_17_FALL_THR	__attribute__ ((fallthrough)) ;
	#if defined(__cplusplus) && (__cplusplus>=201100L)
        #define SYSTEM_CPP_11_DEFINED		1
	#endif
	#if defined(__cplusplus) && (__cplusplus>=201400L)
        #define SYSTEM_CPP_14_DEFINED		1
	#endif
	#if defined(__cplusplus) && (__cplusplus>=201700L)
        #define SYSTEM_CPP_17_DEFINED		1
	#endif
    #define SYSTEM_DLL_PUBLIC		_declspec(dllexport)
    #define SYSTEM_DLL_PRIVATE
    #define SYSTEM_IMPORT_FROM_DLL	_declspec(dllimport)
#elif defined(__SUNPRO_CC)
    #define SYSTEM_UNREACHABLE_CODE(_code)	_code ;
    // #define SYSTEM_BEFORE_CPP_17_FALL_THR	__attribute__ ((fallthrough)) ; // ???
    #define SYSTEM_BEFORE_CPP_17_FALL_THR
	#if defined(__cplusplus) && (__cplusplus>=201100L)
        #define SYSTEM_CPP_11_DEFINED		1
	#endif
	#if defined(__cplusplus) && (__cplusplus>=201400L)
        #define SYSTEM_CPP_14_DEFINED		1
	#endif
	#if defined(__cplusplus) && (__cplusplus>=201700L)
        #define SYSTEM_CPP_17_DEFINED		1
	#endif
    #define SYSTEM_DLL_PUBLIC
    #define SYSTEM_DLL_PRIVATE		__hidden
    #define SYSTEM_IMPORT_FROM_DLL
#endif  // #ifdef _MSC_VER

#if defined(SYSTEM_COMPILING_SHARED_LIB)
    #define SYSTEM_EXPORT SYSTEM_DLL_PUBLIC
#elif defined(SYSTEM_USING_STATIC_LIB_OR_OBJECTS)
    #define SYSTEM_EXPORT
#else
    #define SYSTEM_EXPORT SYSTEM_IMPORT_FROM_DLL
#endif

#ifdef __cplusplus
#define SYSTEM_STATIC_CAST(_type,_data)	static_cast<_type>(_data)
#define SYSTEM_REINTERPRET_CAST(_type,_data)	reinterpret_cast<_type>(_data)
#define SYSTEM_GLOBAL	   ::
#define SYSTEM_BEGIN_C   extern "C" {
#define SYSTEM_END_C     }
#define SYSTEM_EXTERN_C  extern "C"
#else
#define SYSTEM_STATIC_CAST(_type,_data)	((_type)(_data))
#define SYSTEM_REINTERPRET_CAST(_type,_data)	((_type)(_data))
#define SYSTEM_GLOBAL
#define SYSTEM_BEGIN_C
#define SYSTEM_END_C
#define SYSTEM_EXTERN_C
#endif


#ifdef SYSTEM_CPP_17_DEFINED
#define SYSTEM_FALLTHROUGH	[[fallthrough]] ;
#else
#define SYSTEM_FALLTHROUGH	SYSTEM_BEFORE_CPP_17_FALL_THR
//#define SYSTEM_FALLTHROUGH
#endif

#ifdef SYSTEM_CPP_11_DEFINED
#define SYSTEM_STD_MOVE_DEFINED   1
#define SYSTEM_NOEXCEPT	noexcept
#define SYSTEM_NULL		nullptr
#define SYSTEM_OVERRIDE	override
#else
#define SYSTEM_NOEXCEPT	throw()
#define SYSTEM_NULL		NULL
#define SYSTEM_OVERRIDE
#endif

#ifdef __cplusplus
#define SYSTEM_REGISTER
#else
#define SYSTEM_REGISTER	register
#endif


#ifdef _WIN32
#define SYSTEM_HANDLE_CAST	SYSTEM_REINTERPRET_CAST
#else
#define SYSTEM_HANDLE_CAST	SYSTEM_STATIC_CAST
#endif


#endif  // #ifndef INCLUDE_SYSTEM_INTERNAL_HEADER_H
