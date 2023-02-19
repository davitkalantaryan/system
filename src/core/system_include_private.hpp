//
// file:       common_system_include_private.hpp
//

#ifndef COMMON_SYSTEM_INCLUDE_PRIVATE_HPP
#define COMMON_SYSTEM_INCLUDE_PRIVATE_HPP


#include <system/exe/parent.hpp>
#include <stdint.h>

#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable:4365)
#endif
#include <string>
#ifdef _MSC_VER
#pragma warning (pop)
#endif

//#define MAKE_SOME_TESTS


namespace systemN { namespace exe { namespace parent{


#ifdef _WIN32

struct SPipeHelperData;

struct SPipeStruct{
	HANDLE				pr;
	HANDLE				pw;
	int  InitPipeStruct(void);
	void DestroyPipeStruct(void);
};

struct SHandle{
	PROCESS_INFORMATION		prcInfo;
	//SPipeStruct				readPipes[NUMBER_OF_EXE_READ_PIPES], stdinToWriite, dataToChild, remoteControlPipeWrite;
	SPipeStruct				stdinToWriite, remoteControlPipeWrite;
	SPipeStruct*			pReadPipes;
	SPipeStruct*			pDataToChildPipes;
	uint64_t				exeReturnCode : 32;
	uint64_t				numberOfReadFromChildPipes : 8;
	uint64_t				numberOfWriteToChildDataPipes : 8;
	uint64_t				stdPipesUsed : 8;
	uint64_t				shouldRun : 1;
	uint64_t				isExeFinished : 1;
	uint64_t				reserved64bit : 6;
	HANDLE					stdOutputsRedirectorThread;
	HANDLE					stdInputRedirectorThread;
	//HANDLE					stdinToWriteNoClose;
	HANDLE					stdoutToRead;
	HANDLE					stderrToRead;
	WaitFunctionType		fpWaiterFunction;
};

#else   // #ifdef _WIN32

struct SPipeStruct{
    int pipes[2];
    uint64_t  isClosed : 1;
    uint64_t  isInError : 1;
    uint64_t  reserved : 62;
};

struct SHandle {
    pid_t   pid, reservedPid;
    //SPipeStruct readPipes[NUMBER_OF_EXE_READ_PIPES], stdinToWriite, dataToChild, remoteControlPipeWrite;
    SPipeStruct				stdinToWriite, remoteControlPipeWrite;
    SPipeStruct*			pReadPipes;
    SPipeStruct*			pDataToChildPipes;
    //uint64_t  waited : 1;
    //uint64_t  cleared : 1;
    //uint64_t  shouldWait : 1;
    uint64_t				numberOfReadFromChildPipes : 8;
    uint64_t				numberOfWriteToChildDataPipes : 8;
    uint64_t                shouldWait2 : 1;
    uint64_t                reserved64bit : 48;
    //int32_t returnCode, reserved;
    //SHandle() { this->pid = -1; this->waited = 0; this->shouldWait = this->cleared = 1; }
};

#endif  // #ifdef _WIN32

}}}  //  namespace systemN { namespace exe { namespace parent{

#endif  // #ifndef COMMON_SYSTEM_INCLUDE_PRIVATE_HPP
