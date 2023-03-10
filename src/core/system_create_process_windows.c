//
// file:            system_create_process_windows.c
// path:            src/core/system_create_process_windows.c
// created on:      2023 Feb 18
// created by:      Davit Kalantaryan (davit.kalantaryan@desy.de)
//

#include <system/export_symbols.h>

#ifdef _WIN32

#include <system/create_process.h>
#include <stdlib.h>
#include <string.h>
#include <cinternal/disable_compiler_warnings.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>

#define SYSTEM_ALLOC_STEP	4096

CPPUTILS_BEGIN_C

struct CPPUTILS_DLL_PRIVATE SSystemProcessHandle{
	STARTUPINFOA			startupInfo;
	PROCESS_INFORMATION		procInfo;
};




SYSTEM_EXPORT TSystemProcessHandlePtr SystemCreateProcessU(char* a_argv[])
{
	TSystemProcessHandlePtr pReturn;
	size_t unAllocTotalSize = 0;
	size_t unTotalSize2 = 0;
	size_t unNewTotalSize = 1;
	size_t i;
	size_t nextStrLen;
	char* pcTmpAlloc;
	char* commandLine = CPPUTILS_NULL;

	for (i = 1; a_argv[i]; ++i) {
		nextStrLen = strlen(a_argv[i]);
		unNewTotalSize += (nextStrLen + 1);
		while (unNewTotalSize > unAllocTotalSize) {
			unAllocTotalSize += SYSTEM_ALLOC_STEP;
		}  //  while (unTotalSize > unAllocTotalSize) {
		pcTmpAlloc = CPPUTILS_STATIC_CAST(char*, realloc(commandLine, unAllocTotalSize));
		if (!pcTmpAlloc) {
			free(commandLine);
			return CPPUTILS_NULL;
		}  //  if (!pcTmpAlloc) {
		commandLine = pcTmpAlloc;
		memcpy(commandLine + unTotalSize2, a_argv[i], nextStrLen);
		*(commandLine + unTotalSize2 + nextStrLen) = ' ';
		unTotalSize2 += (nextStrLen + 1);
	}  //  for (i = 1; a_argv[i]; ++i) {

	if (commandLine) {
		*(commandLine + unTotalSize2) = 0;
	}

	pReturn = SystemCreateProcessW(a_argv[0], commandLine);
	free(commandLine);
	return pReturn;
}


SYSTEM_EXPORT TSystemProcessHandlePtr SystemCreateProcessW(const char* a_binaryName, char* a_commandLine)
{
	BOOL bCrtPrcRet;
	TSystemProcessHandlePtr pReturn = CPPUTILS_STATIC_CAST(TSystemProcessHandlePtr,(malloc(sizeof(struct SSystemProcessHandle))));
	if (!pReturn) {
		return CPPUTILS_NULL;
	}

	ZeroMemory(&(pReturn->startupInfo), sizeof(STARTUPINFOA));
	pReturn->startupInfo.wShowWindow = SW_SHOWNOACTIVATE;

	bCrtPrcRet = CreateProcessA(
		a_binaryName,
		a_commandLine,
		CPPUTILS_NULL,
		CPPUTILS_NULL,
		FALSE,
		0,
		CPPUTILS_NULL,
		CPPUTILS_NULL,
		&(pReturn->startupInfo),
		&(pReturn->procInfo)
	);

	if (!bCrtPrcRet) {
		free(pReturn);
		return CPPUTILS_NULL;
	}

	return pReturn;
}


SYSTEM_EXPORT void SystemWaitAndClearProcess(TSystemProcessHandlePtr a_procHandle, int* a_pProcessExitCode)
{
	WaitForSingleObject(a_procHandle->procInfo.hProcess,INFINITE);
	if (a_pProcessExitCode) {
		DWORD dwExitCode;
		GetExitCodeProcess(a_procHandle->procInfo.hProcess, &dwExitCode);
		*a_pProcessExitCode = CPPUTILS_STATIC_CAST(int, dwExitCode);
	}
	CloseHandle(a_procHandle->procInfo.hThread);
	CloseHandle(a_procHandle->procInfo.hProcess);
	free(a_procHandle);
}


CPPUTILS_END_C


#endif  //  #ifdef _WIN32
