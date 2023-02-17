//
// file:            system_specific_windows.cpp
// created on:      2019 Oct 14
// created by:      Davit Kalantaryan (davit.kalantaryan@desy.de)
//
// This header is for ...
//

#include <system/system_specific_definations.h>
#include <stdarg.h>
#include <stdio.h>

#define BUFFER_SIZE_MIN1_FOR_SPRINTF		4093

CPPUTILS_BEGIN_C


SYSTEM_EXPORT int sys_dprintf(HANDLE a_file,const char* a_cpcFormat,...)
{
	int nReturn;
	char vcBuffer[BUFFER_SIZE_MIN1_FOR_SPRINTF + 1];
	va_list  argList;

	va_start(argList, a_cpcFormat);
	nReturn = vsnprintf_s(vcBuffer, BUFFER_SIZE_MIN1_FOR_SPRINTF, BUFFER_SIZE_MIN1_FOR_SPRINTF, a_cpcFormat,argList);
	va_end(argList);

	return nReturn;
}


CPPUTILS_END_C
