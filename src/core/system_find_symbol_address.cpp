//
// file:            system_specific_windows.cpp
// created on:      2019 Oct 14
// created by:      Davit Kalantaryan (davit.kalantaryan@desy.de)
//
// This header is for ...
//


#include <system/find_symbol_address.h>


#ifdef _WIN32

#include <cinternal/logger.h>
#include <cinternal/disable_compiler_warnings.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <TlHelp32.h>
#include <cinternal/undisable_compiler_warnings.h>


CPPUTILS_BEGIN_C


SYSTEM_EXPORT void* SystemFindSymbolAddress(const char* CPPUTILS_ARG_NN a_symbolLookupName) CPPUTILS_NOEXCEPT
{
    void* pReturn = CPPUTILS_NULL;
    HMODULE module = CPPUTILS_NULL;
    MODULEENTRY32 moduleEntry;
    const HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        CInternalLogError("Failed to create snapshot.");
        return CPPUTILS_NULL;
    }

    moduleEntry.dwSize = sizeof(MODULEENTRY32);

    if (Module32First(snapshot, &moduleEntry)) {
        do {
            // Check each module for the function
            module = GetModuleHandle(moduleEntry.szModule);
            if (module) {
                pReturn = GetProcAddress(module, a_symbolLookupName);
                if (pReturn) {
                    break;
                }
            }
        } while (Module32Next(snapshot, &moduleEntry));
    }  //  if (Module32First(snapshot, &moduleEntry)) {

    CloseHandle(snapshot);
    return pReturn;
}



CPPUTILS_END_C


#elif defined(__APPLE__)

#include <cinternal/disable_compiler_warnings.h>
#include <stdint.h>
#include <dlfcn.h>
#include <mach-o/dyld.h>
#include <cinternal/undisable_compiler_warnings.h>


CPPUTILS_BEGIN_C


SYSTEM_EXPORT void* SystemFindSymbolAddress(const char* CPPUTILS_ARG_NN a_symbolLookupName) CPPUTILS_NOEXCEPT
{
    void *handle, *function;
    const char *imageName;
    uint32_t i;
    const uint32_t libsCount = _dyld_image_count();

    for (i = 0; i < libsCount; ++i) {
        imageName = _dyld_get_image_name(i);
        if (imageName) {
            handle = dlopen(imageName, RTLD_NOW);
            if (handle) {
                function = dlsym(handle, a_symbolLookupName);
                if (function) {
                    dlclose(handle);
                    return function;
                }
                dlclose(handle);
            }  //  if (handle) {
        }  //  if (imageName) {
    }  //  for (i = 0; i < libsCount; ++i) {

    return CPPUTILS_NULL;
}



CPPUTILS_END_C


#else


#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <cinternal/disable_compiler_warnings.h>
#include <link.h>
#include <cinternal/undisable_compiler_warnings.h>


CPPUTILS_BEGIN_C


#define SYSTEM_USE_ITERATE


#ifdef SYSTEM_USE_ITERATE

static int FindFunctionStatic(struct dl_phdr_info* a_info, size_t a_size, void* a_data) CPPUTILS_NOEXCEPT;

struct SystemClbStruct01{
    const char* functionName;
    void*       funcAddress;
};


#endif


SYSTEM_EXPORT void* SystemFindSymbolAddress(const char* CPPUTILS_ARG_NN a_symbolLookupName) CPPUTILS_NOEXCEPT
{
    
#ifdef SYSTEM_USE_ITERATE

    struct SystemClbStruct01 clbkData = {a_symbolLookupName,CPPUTILS_NULL};
    dl_iterate_phdr(&FindFunctionStatic, &clbkData);
    return clbkData.funcAddress;
    
#else 
    
    void* const pRet = dlsym(RTLD_DEFAULT,a_symbolLookupName);
    return pRet;
    
#endif
    
}

#ifdef SYSTEM_USE_ITERATE

static int FindFunctionStatic(struct dl_phdr_info* a_info, size_t a_size, void* a_data) CPPUTILS_NOEXCEPT
{
    struct SystemClbStruct01* const clbkData_p = (struct SystemClbStruct01*)a_data;
    void* const handle = dlopen(a_info->dlpi_name, RTLD_NOW);
    CPPUTILS_STATIC_CAST(void,a_size);
    if (handle) {
        clbkData_p->funcAddress = dlsym(handle, clbkData_p->functionName);
        if (clbkData_p->funcAddress) {
            dlclose(handle);
            return 1; // Stop iterating
        }
        dlclose(handle);
    }
    return 0; // Continue iterating
}

#endif


CPPUTILS_END_C


#endif  //  #ifdef _WIN32
