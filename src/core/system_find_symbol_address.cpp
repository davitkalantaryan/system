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


#else


CPPUTILS_BEGIN_C





CPPUTILS_END_C


#endif  //  #ifdef _WIN32
