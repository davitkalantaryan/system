//
// file:            common/system/rw.hpp
// created on:      2019 Sep 18
// created by:      Davit Kalantaryan (davit.kalantaryan@desy.de)
//
// This header is for functions, that is used to call system routines and binaries
//

#pragma once

#ifndef SYSTEM_INCLUDE_SYSTEM_RW_HPP
#define SYSTEM_INCLUDE_SYSTEM_RW_HPP

#include <system/export_symbols.h>
#include <stddef.h>
#include <string>
#include <stdint.h>
#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#define UNINITED_HANDLE	CPPUTILS_NULL
typedef DWORD		sssize_t;
typedef HANDLE		handle_t;
#else
#include <unistd.h>
#define UNINITED_HANDLE		(-1)
typedef ssize_t		sssize_t;
typedef int			handle_t;
#endif
typedef int16_t		pindex_t;
typedef handle_t* (*HandleGetterType)(void*, pindex_t);
typedef sssize_t (*WaitFunctionType)(void*, int timeoutMs);

// some return codes
#define NO_HANDLE_EXIST2				-1980
#define OUT_OF_INDEX					-1981
#define UNABLE_TO_READ					-1982
#define COMMON_SYSTEM_TIMEOUT			-1983
#define COMMON_SYSTEM_UNKNOWN			-1984
#define COMMON_SYSTEM_PIPE_CLOSED		-1985
#define COMMON_SYSTEM_RW_NO_MEM			-1986
#define COMMON_SYSTEM_RW_INTERRUPTED	-1987

//#define NUMBER_OF_EXE_READ_PIPES2		int8_t(3) // stdoutRead, stderrRead, dataFromChild, controlPipe[0]
#define CONTROL_RD_EXE_PIPE				0
#define STDOUT_EXE_PIPE					1
#define STDERR_EXE_PIPE					2
#define DATA_0_FROM_CHILD_EXE_PIPE		3

//#define ME_READ_PIPE					0
//#define ME_WRITE_PIPE					1

#define USE_STANDARD_STDIN				1
#define USE_STANDARD_STDOUT				2
#define USE_STANDARD_STDERR				4

#define INIT_C_ARRAY01(_value)			{(_value)}
#define INIT_C_ARRAY02(_value)			{(_value),(_value)}
#define INIT_C_ARRAY03(_value)			{(_value),(_value),(_value)}
#define INIT_C_ARRAY04(_value)			{(_value),(_value),(_value),(_value)}
#define INIT_C_ARRAY05(_value)			{(_value),(_value),(_value),(_value),(_value)}

#define INIT_C_ARRAYRD					INIT_C_ARRAY04

#ifdef __cplusplus

namespace systemN {

SYSTEM_EXPORT sssize_t WriteToHandle(handle_t handle,const void* buffer, size_t bufferSize);
SYSTEM_EXPORT pindex_t ReadFromManyPipes(
	void* handlesParent, pindex_t handlesCount, HandleGetterType fpHandleGetter, 
	void** buffers, const size_t* buffersSizes, sssize_t* pReadSize, int timeoutMs, WaitFunctionType a_fpWait);

}  //  namespace systemN {


#endif  // #ifdef __cplusplus

#endif  // #ifndef SYSTEM_INCLUDE_SYSTEM_RW_HPP
