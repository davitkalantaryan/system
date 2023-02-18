//
// file:            system_create_process_unix.c
// path:            src/core/system_create_process_unix.c
// created on:      2023 Feb 18
// created by:      Davit Kalantaryan (davit.kalantaryan@desy.de)
//

#include <system/export_symbols.h>

#ifndef _WIN32

#include <system/create_process.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define SYSTEM_INIT_NUMBER_OF_CMD_LINE_ARGUMENTS     128

#pragma GCC diagnostic ignored "-Wattributes"


CPPUTILS_BEGIN_C

struct CPPUTILS_DLL_PRIVATE SSystemProcessHandle{
    char**      ppcArgv;
    char**      reserved01;
    int32_t     pid;
    int32_t     reserved02;
};


static inline void ClearArgvInline(char** a_argv)
{
    if(a_argv){
        free(a_argv[0]);
        free(a_argv);
    }
}


static inline void ClearProcessHandleata(TSystemProcessHandlePtr a_procHandle)
{
    ClearArgvInline(a_procHandle->ppcArgv);
    free(a_procHandle);
}


static inline TSystemProcessHandlePtr CreatProcessInine(TSystemProcessHandlePtr a_procHandle, char* a_argv[])
{
    char** ppcArgv = a_procHandle->ppcArgv;

    a_procHandle->pid = CPPUTILS_STATIC_CAST(int32_t,fork());
    if(a_procHandle->pid==-1){
        ClearProcessHandleata(a_procHandle);
        return CPPUTILS_NULL;
    }

    if(a_procHandle->pid){
        // this is creator process, and the pid is childs pid
        return a_procHandle;
    }

    free(a_procHandle);
    execvp(a_argv[0],a_argv);
    // we have an error
    ClearArgvInline(ppcArgv);

    exit(1);
}


SYSTEM_EXPORT TSystemProcessHandlePtr SystemCreateProcessU(char* a_argv[])
{
    TSystemProcessHandlePtr pReturn = CPPUTILS_STATIC_CAST(TSystemProcessHandlePtr,malloc(sizeof(struct SSystemProcessHandle)));
    if(!pReturn){
        return CPPUTILS_NULL;
    }
    pReturn->ppcArgv = CPPUTILS_NULL;
    return CreatProcessInine(pReturn,a_argv);
}


SYSTEM_EXPORT TSystemProcessHandlePtr SystemCreateProcessW(const char* a_binaryName, char* a_commandLine)
{
    static const char* scpcWhiteSymbols = " \t\n";
    char** pcAllocTmp;
    size_t unSizeAll;
    size_t unCountArgv;
    size_t unStrSpn;
    TSystemProcessHandlePtr pReturn = CPPUTILS_STATIC_CAST(TSystemProcessHandlePtr,malloc(sizeof(struct SSystemProcessHandle)));
    if(!pReturn){
        return CPPUTILS_NULL;
    }

    pReturn->ppcArgv = CPPUTILS_STATIC_CAST(char**,malloc(SYSTEM_INIT_NUMBER_OF_CMD_LINE_ARGUMENTS*sizeof(char*)));
    if(!(pReturn->ppcArgv)){
        free(pReturn);
        return CPPUTILS_NULL;
    }
    unSizeAll = SYSTEM_INIT_NUMBER_OF_CMD_LINE_ARGUMENTS;

    (pReturn->ppcArgv)[0] = strdup(a_binaryName);
    if(!((pReturn->ppcArgv)[0])){
        free(pReturn->ppcArgv);
        free(pReturn);
        return CPPUTILS_NULL;
    }

    unCountArgv = 1;
    unStrSpn = strspn(a_commandLine,scpcWhiteSymbols);
    while(1){
        if(!a_commandLine[unStrSpn]){break;}
        a_commandLine += (unStrSpn+1);
        unStrSpn = strcspn(a_commandLine,scpcWhiteSymbols);
        if(unCountArgv>unSizeAll){
            unSizeAll += SYSTEM_INIT_NUMBER_OF_CMD_LINE_ARGUMENTS;
            pcAllocTmp = CPPUTILS_STATIC_CAST(char**,realloc(pReturn->ppcArgv,unSizeAll*sizeof(char*)));
            if(!pcAllocTmp){
                ClearProcessHandleata(pReturn);
                return CPPUTILS_NULL;
            }  //  if(!pcAllocTmp){
            pReturn->ppcArgv = pcAllocTmp;
        }  //  if((++unCountArgv)>unSizeAll){
        (pReturn->ppcArgv)[unCountArgv++] = a_commandLine;
        if(!a_commandLine[unStrSpn]){break;}
        a_commandLine[unStrSpn] = 0;
        a_commandLine += (unStrSpn+1);
        unStrSpn = strspn(a_commandLine,scpcWhiteSymbols);
    }  //  while(1){

    return CreatProcessInine(pReturn,pReturn->ppcArgv);
}


SYSTEM_EXPORT void SystemWaitAndClearProcess(TSystemProcessHandlePtr a_procHandle, int* a_pProcessExitCode)
{
    int status;
    const pid_t chP = (pid_t)a_procHandle->pid;
    pid_t w;

    while(1){
        w = waitpid(a_procHandle->pid, &status, WUNTRACED);
        if((w==chP)&&WIFEXITED(status)){
            break;
        }
    }

    if(a_pProcessExitCode){
        *a_pProcessExitCode = WEXITSTATUS(status);
    }
}


CPPUTILS_END_C


#endif  //  #ifndef _WIN32
