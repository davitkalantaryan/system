//
// file:            create_process.h
// path:            include/system/create_process.h
// created on:      2023 Feb 18
// created by:      Davit Kalantaryan (davit.kalantaryan@desy.de)
//

#ifndef SYSTEM_INCLUDE_SYSTEM_CREATE_PROCESS_H
#define SYSTEM_INCLUDE_SYSTEM_CREATE_PROCESS_H


#include <system/export_symbols.h>


CPPUTILS_BEGIN_C



struct SSystemProcessHandle;
typedef struct SSystemProcessHandle* TSystemProcessHandlePtr;

SYSTEM_EXPORT TSystemProcessHandlePtr SystemCreateProcessU(char* a_argv[]);
SYSTEM_EXPORT TSystemProcessHandlePtr SystemCreateProcessW(const char* a_binaryName, char* a_commandLine);
SYSTEM_EXPORT void SystemWaitAndClearProcess(TSystemProcessHandlePtr a_procHandle, int* a_pProcessExitCode);



CPPUTILS_END_C


#endif  //  #ifndef SYSTEM_INCLUDE_SYSTEM_CREATE_PROCESS_H
