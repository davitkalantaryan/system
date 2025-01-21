//
// file:			export_symbols.h
// path:			include/system/export_symbols.h
// created on:		2023 Feb 16
// created by:		Davit Kalantaryan (davit.kalantaryan@desy.de)
//

#ifndef SYSTEM_INCLUDE_SYSTEM_EXPORT_SYMBOLS_H
#define SYSTEM_INCLUDE_SYSTEM_EXPORT_SYMBOLS_H

#include <cinternal/internal_header.h>

#ifndef SYSTEM_EXPORT
#if defined(SYSTEM_COMPILING_SHARED_LIB)
#define SYSTEM_EXPORT CPPUTILS_DLL_PUBLIC
#elif defined(SYSTEM_USING_STATIC_LIB_OR_OBJECTS)
#define SYSTEM_EXPORT
#elif defined(SYSTEM_LOAD_FROM_DLL)
#define SYSTEM_EXPORT CPPUTILS_IMPORT_FROM_DLL
#else
#define SYSTEM_EXPORT CPPUTILS_DLL_PRIVATE
#endif
#endif

#ifdef _MSC_VER
#pragma warning (disable:4820) //  'systemN::exe::parent::SHandle': '4' bytes padding added
#endif


#endif  // #ifndef SYSTEM_INCLUDE_SYSTEM_EXPORT_SYMBOLS_H
