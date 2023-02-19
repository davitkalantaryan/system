//
// file:            system_specific_definations.h
// created on:      2019 Oct 14
// created by:      Davit Kalantaryan (davit.kalantaryan@desy.de)
//
// This header is for functions, that is used to call system routines and binaries
//

#ifndef SYSTEM_SPECIFIC_DEFINATIONS_H
#define SYSTEM_SPECIFIC_DEFINATIONS_H

#ifdef _MSC_VER
#define strdup	_strdup
#ifndef NOMINMAX
#define NOMINMAX
#endif
//#pragma warning(disable:4244)//TLeaf.h(121,72): warning C4244: 'return': conversion from 'Double_t' to 'Long64_t', possible loss of data
#endif

#include <system/export_symbols.h>
#include <stddef.h>

CPPUTILS_BEGIN_C

#ifdef _WIN32

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <io.h>

#define dlclose			FreeLibrary
#define dlsym			GetProcAddress
#define STDOUT_FILENO	CPPUTILS_REINTERPRET_CAST(HANDLE,STD_OUTPUT_HANDLE)

typedef DWORD ssize_t;
typedef HMODULE LibHandleType;

SYSTEM_EXPORT int sys_dprintf(HANDLE a_file, const char* a_cpcFormat, ...);

#else   // #ifdef _WIN32

#include <unistd.h>
#include <dlfcn.h>

#define sys_dprintf				dprintf		
#define LoadLibraryA(_libPath)	dlopen((_libPath),RTLD_LAZY)

typedef void* LibHandleType;

#endif  // #ifdef _WIN32


CPPUTILS_END_C



#endif  // #ifndef SYSTEM_SPECIFIC_DEFINATIONS_H
