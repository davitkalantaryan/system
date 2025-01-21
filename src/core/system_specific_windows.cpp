//
// file:            system_specific_windows.cpp
// created on:      2019 Oct 14
// created by:      Davit Kalantaryan (davit.kalantaryan@desy.de)
//
// This header is for ...
//


#include <cinternal/internal_header.h>


#ifdef _WIN32


#include <system/system_specific_definations.h>
#include <cinternal/disable_compiler_warnings.h>
#include <stdarg.h>
#include <stdio.h>
#include <cinternal/undisable_compiler_warnings.h>

#define BUFFER_SIZE_MIN1_FOR_SPRINTF		4093

CPPUTILS_BEGIN_C


SYSTEM_EXPORT int sys_dprintf(HANDLE a_file,const char* a_cpcFormat,...)
{
	BOOL bWrRet;
	DWORD dwReturn;
	int nReturn;
	char vcBuffer[BUFFER_SIZE_MIN1_FOR_SPRINTF + 1];
	va_list  argList;

	va_start(argList, a_cpcFormat);
	nReturn = vsnprintf_s(vcBuffer, BUFFER_SIZE_MIN1_FOR_SPRINTF, BUFFER_SIZE_MIN1_FOR_SPRINTF, a_cpcFormat,argList);
	va_end(argList);

	bWrRet = WriteFile(a_file, vcBuffer, (DWORD)nReturn, &dwReturn, CPPUTILS_NULL);
	if (bWrRet) {
		return (int)dwReturn;
	}

	return -1;
}


CPPUTILS_END_C


#endif  //  #ifdef _WIN32
