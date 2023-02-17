//
// file:            common_system.cpp
// created on:      2019 Aug 26
// created by:      Davit Kalantaryan (davit.kalantaryan@desy.de)
//
// Here is functions, that is used to call system routines and binaries
//

#include <system/exe/parent.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "system_include_private.hpp"
#include <string>
#include <private/system/exe_parent_child_common.h>
#include <mutex>
#include <iostream>

#define PIPES_SIZE		4096


namespace systemN { namespace exe { namespace parent{

static void Clear(struct SHandle* a_handle);
static void ClearAllHandlesStatic(struct SHandle* a_handle);
static DWORD WINAPI  StdOutputsRedirectorThread( ::LPVOID lpThreadParameter);
static DWORD WINAPI  StdInputRedirectorThread(::LPVOID lpThreadParameter);
static void AfterProcessTermination(struct SHandle* a_handle);

THandle RunNoWaitU(int8_t a_numberOfReadFromChildDataPipes, int8_t a_numberOfWriteToChildDataPipes, char* a_argv[], const char* a_cpcAdditionalSearchPath, uint8_t a_bUsingStdPipes)
{
	::std::string strCommandLine;

	if(!a_argv[0]){return CPPUTILS_NULL;}
	strCommandLine = a_argv[0];
	for(int argc(1);a_argv[argc];++argc){
		strCommandLine += " ";
		strCommandLine += a_argv[argc];
	}

	return RunNoWaitW(a_numberOfReadFromChildDataPipes,a_numberOfWriteToChildDataPipes,const_cast<char*>(strCommandLine.c_str()),a_cpcAdditionalSearchPath,a_bUsingStdPipes);
}


THandle RunNoWaitW(int8_t a_numberOfReadFromChildDataPipes, int8_t a_numberOfWriteToChildDataPipes, char* a_argumentsLine, const char* a_cpcAdditionalSearchPath, uint8_t a_bUsingStdPipes)
{
	static ::std::mutex   smutexFornvironment;
	::std::string dataPipeStr;
	DWORD dwErrCode;
	STARTUPINFOA aStartInfo;
	int8_t i;
	char vcPathCurrent[4096];
	//HANDLE 
	//	hStdin(reinterpret_cast<HANDLE>(static_cast<size_t>(STD_INPUT_HANDLE))), 
	//	hStdout(reinterpret_cast<HANDLE>(static_cast<size_t>(STD_OUTPUT_HANDLE))),
	//	hStderr(reinterpret_cast<HANDLE>(static_cast<size_t>(STD_ERROR_HANDLE)));
	//HANDLE hStdin(GetStdHandle(STD_INPUT_HANDLE)), hStdout(GetStdHandle(STD_OUTPUT_HANDLE)), hStderr(GetStdHandle(STD_ERROR_HANDLE));
	THandle pHandle = static_cast<THandle>(calloc(1,sizeof(struct SHandle)));

	if(!pHandle){return CPPUTILS_NULL;}

	pHandle->shouldRun = 1;

    if(a_numberOfWriteToChildDataPipes>0){
		pHandle->pDataToChildPipes = static_cast<struct SPipeStruct*>(calloc(a_numberOfWriteToChildDataPipes, sizeof(struct SPipeStruct)));
		if (!pHandle->pDataToChildPipes) {
			Clear(pHandle);
			return CPPUTILS_NULL;
		}
		pHandle->numberOfWriteToChildDataPipes = a_numberOfWriteToChildDataPipes;
	}

	pHandle->numberOfReadFromChildPipes = DATA_0_FROM_CHILD_EXE_PIPE + ((a_numberOfReadFromChildDataPipes >0)?a_numberOfReadFromChildDataPipes:0);
	pHandle->pReadPipes = static_cast<struct SPipeStruct*>(calloc(pHandle->numberOfReadFromChildPipes,sizeof(struct SPipeStruct)));
	if (!pHandle->pReadPipes) {
		Clear(pHandle);
		return CPPUTILS_NULL;
	}

	// standard pipes
	if (pHandle->stdinToWriite.InitPipeStruct()) {
		Clear(pHandle);
		return CPPUTILS_NULL;
	}
	if (pHandle->pReadPipes[STDOUT_EXE_PIPE].InitPipeStruct()) {
		Clear(pHandle);
		return CPPUTILS_NULL;
	}
	if (a_bUsingStdPipes&USE_STANDARD_STDOUT) {
		pHandle->stdoutToRead = pHandle->pReadPipes[STDOUT_EXE_PIPE].pr;
		pHandle->pReadPipes[STDOUT_EXE_PIPE].pr = CPPUTILS_NULL;
	}
	if (pHandle->pReadPipes[STDERR_EXE_PIPE].InitPipeStruct()) {
		Clear(pHandle);
		return CPPUTILS_NULL;
	}
	if (a_bUsingStdPipes&USE_STANDARD_STDERR) {
		pHandle->stderrToRead = pHandle->pReadPipes[STDERR_EXE_PIPE].pr;
		pHandle->pReadPipes[STDERR_EXE_PIPE].pr = CPPUTILS_NULL;
	}

	// read pipes
	if (pHandle->pReadPipes[CONTROL_RD_EXE_PIPE].InitPipeStruct()) {
		return CPPUTILS_NULL;
	}

	for(i=0;i<a_numberOfReadFromChildDataPipes;++i){
		if (pHandle->pReadPipes[DATA_0_FROM_CHILD_EXE_PIPE+uint16_t(i)].InitPipeStruct()) {
			Clear(pHandle);
			return CPPUTILS_NULL;
		}
	}

	// write pipes
	if (pHandle->remoteControlPipeWrite.InitPipeStruct()) {
		Clear(pHandle);
		return CPPUTILS_NULL;
	}

	for(i=0;i< a_numberOfWriteToChildDataPipes;++i){
		if (pHandle->pDataToChildPipes[i].InitPipeStruct()) {
			Clear(pHandle);
			return CPPUTILS_NULL;
		}
	}

	ZeroMemory(&aStartInfo, sizeof(aStartInfo));
	aStartInfo.dwFlags = STARTF_USESTDHANDLES;
	aStartInfo.hStdInput = pHandle->stdinToWriite.pr;
	aStartInfo.hStdOutput = pHandle->pReadPipes[STDOUT_EXE_PIPE].pw;
	aStartInfo.hStdError = pHandle->pReadPipes[STDERR_EXE_PIPE].pw;
	aStartInfo.wShowWindow = SW_HIDE;
	aStartInfo.dwFlags = STARTF_USESTDHANDLES;

	// todo: delete test lines
	//DWORD dwHadleFlags;
	//if(GetHandleInformation(hStdout,&dwHadleFlags)){
	//	::std::cout << "isInh=" << ((dwHadleFlags & HANDLE_FLAG_INHERIT)?1:0) << ::std::endl;
	//}

	pHandle->stdPipesUsed = a_bUsingStdPipes;
	if((a_bUsingStdPipes&USE_STANDARD_STDOUT)||(a_bUsingStdPipes&USE_STANDARD_STDERR)){
		pHandle->stdOutputsRedirectorThread = CreateThread(CPPUTILS_NULL,0,&StdOutputsRedirectorThread,pHandle,0,CPPUTILS_NULL);
		if(!pHandle->stdOutputsRedirectorThread){
			Clear(pHandle);
			return CPPUTILS_NULL;
		}
	}
	if(a_bUsingStdPipes&USE_STANDARD_STDIN){
		pHandle->stdInputRedirectorThread = CreateThread(CPPUTILS_NULL,0,&StdInputRedirectorThread,pHandle,0,CPPUTILS_NULL);
		if(!pHandle->stdInputRedirectorThread){
			Clear(pHandle);
			return CPPUTILS_NULL;
		}
	}

	// setup environment
	smutexFornvironment.lock();

	if(a_cpcAdditionalSearchPath){
		char vcNewPath[4096];
		vcPathCurrent[0] = 0;
		GetEnvironmentVariableA("Path", vcPathCurrent, 4095);
		_snprintf_s(vcNewPath, 4095, 4095, "%s;%s\\bin;%s\\..\\bin;%s\\..\\Win32\\bin;%s\\..\\..\\..\\Win32\\Debug\\bin;%s", 
			a_cpcAdditionalSearchPath, a_cpcAdditionalSearchPath,a_cpcAdditionalSearchPath, a_cpcAdditionalSearchPath, a_cpcAdditionalSearchPath, vcPathCurrent);
		SetEnvironmentVariableA("Path", vcNewPath);
	}

	SetEnvironmentVariableA(COMMON_SYSTEM_EXE_DATAPIPE_TO_PARENT_NUMBER, ::std::to_string(a_numberOfReadFromChildDataPipes).c_str());
	SetEnvironmentVariableA(COMMON_SYSTEM_EXE_DATAPIPE_FROM_PARENT_NUMBER, ::std::to_string(a_numberOfWriteToChildDataPipes).c_str());

	SetEnvironmentVariableA(COMMON_SYSTEM_EXE_CONTROLPIPE_TO_PARENT, ::std::to_string((size_t)pHandle->pReadPipes[CONTROL_RD_EXE_PIPE].pw).c_str());
	SetEnvironmentVariableA(COMMON_SYSTEM_EXE_CONTROLPIPE_FROM_PARENT, ::std::to_string((size_t)pHandle->remoteControlPipeWrite.pr).c_str());
	for(i=0;i<a_numberOfReadFromChildDataPipes;++i){
		dataPipeStr = COMMON_SYSTEM_EXE_DATAPIPE_TO_PARENT_B + ::std::to_string(i);
		SetEnvironmentVariableA(dataPipeStr.c_str(), ::std::to_string((size_t)pHandle->pReadPipes[DATA_0_FROM_CHILD_EXE_PIPE + uint16_t(i)].pw).c_str());
	}
	for(i=0;i< a_numberOfWriteToChildDataPipes;++i){
		dataPipeStr = COMMON_SYSTEM_EXE_DATAPIPE_FROM_PARENT_B + ::std::to_string(i);
		SetEnvironmentVariableA(dataPipeStr.c_str(), ::std::to_string((size_t)pHandle->pDataToChildPipes[i].pr).c_str());
	}

	dwErrCode = CreateProcessA(
		CPPUTILS_NULL,											// Application name (NULL means take from command line)
		const_cast<char*>(a_argumentsLine),						// Command line to execute
		CPPUTILS_NULL,											// Process security attributes
		CPPUTILS_NULL,											// Thread security attributes
		TRUE,													// Inherit handles
		NORMAL_PRIORITY_CLASS|CREATE_NO_WINDOW,
		CPPUTILS_NULL,
		CPPUTILS_NULL,
		&aStartInfo,
		&(pHandle->prcInfo)
	);
	if (a_cpcAdditionalSearchPath) {
		SetEnvironmentVariableA("Path", vcPathCurrent);
	}
	smutexFornvironment.unlock();

	if(!dwErrCode){
		Clear(pHandle);
		return CPPUTILS_NULL;
	}

	pHandle->fpWaiterFunction = [](void* a_pHandleInner1,int a_timeoutMsInner1){
		THandle pHandleInner1= static_cast<THandle>(a_pHandleInner1);
		sssize_t nReturnInner1=WaitForSingleObjectEx(pHandleInner1->prcInfo.hProcess,a_timeoutMsInner1,TRUE);
		if(nReturnInner1==WAIT_OBJECT_0){
			AfterProcessTermination(pHandleInner1);
			pHandleInner1->fpWaiterFunction = [](void*,int a_timeoutMsInner2){ return SleepEx(a_timeoutMsInner2,TRUE);};
			if(SleepEx(2,TRUE)==WAIT_IO_COMPLETION){
				nReturnInner1 = WAIT_IO_COMPLETION;
			}
		}
		return nReturnInner1;
	};

    return pHandle;
}


bool WaitAndClear(THandle a_handle, int a_timeoutMs, int* a_exeReturnCodePtr)
{
	DWORD dwWaitReturn;
	bool bReturn(true);

	if(a_handle->prcInfo.hProcess){
		dwWaitReturn = WaitForSingleObject(a_handle->prcInfo.hProcess, a_timeoutMs);
		if (dwWaitReturn != WAIT_OBJECT_0) {
			AfterProcessTermination(a_handle);
			bReturn = false;
		}
	}

	if (bReturn) {
		if(a_exeReturnCodePtr){*a_exeReturnCodePtr = static_cast<int>(a_handle->exeReturnCode);}
		Clear(a_handle);
	}

	return bReturn;
}


//uint64_t IsExeFinished(THandle a_handle)
//{
//	return a_handle->isExeFinished;
//}


sssize_t WriteToStdIn(THandle a_handle, const void* a_buffer, size_t a_bufferSize)
{
	return WriteToHandle(a_handle->stdinToWriite.pw, a_buffer, a_bufferSize);
}


//struct SPipeIndexStructPrivate {
//	THandle hndl;
//	HANDLE* pPipe;
//};
//static int WaitForCmpl(void* a_handleInner, int a_timeoutMsInner)
//{
//	THandle exeHandle = static_cast<SPipeIndexStructPrivate*>(a_handleInner)->hndl;
//	DWORD dwWaitReturn = WaitForSingleObjectEx(exeHandle->prcInfo.hProcess,a_timeoutMsInner,TRUE);
//	return static_cast<int>(dwWaitReturn);
//}

sssize_t ReadDataFromPipe(THandle a_handle, pindex_t a_pipeIndex, void* a_buffer, size_t a_bufferSize, int a_timeoutMs)
{
	struct SPipeIndexStructPrivate {
		THandle hndl;
		HANDLE* pPipe;
	}aPipeIndStr{ a_handle ,CPPUTILS_NULL};
	//SPipeIndexStructPrivate aPipeIndStr{ a_handle ,CPPUTILS_NULL };
	sssize_t readReturn;
	void* vBuffers[1] = { a_buffer };
	const size_t vBufferSizes[1] = { a_bufferSize };
	a_pipeIndex += DATA_0_FROM_CHILD_EXE_PIPE;
	if (a_pipeIndex >= a_handle->numberOfReadFromChildPipes) { return OUT_OF_INDEX; }
	aPipeIndStr.pPipe = &(a_handle->pReadPipes[a_pipeIndex].pr);
	ReadFromManyPipes(
		&aPipeIndStr,1,
		[](void*a_handleInner, pindex_t){return static_cast<SPipeIndexStructPrivate*>(a_handleInner)->pPipe;},
		vBuffers, vBufferSizes, &readReturn, a_timeoutMs,
		[](void* a_handleInner,int a_timeoutMsInner){return (*static_cast<SPipeIndexStructPrivate*>(a_handleInner)->hndl->fpWaiterFunction)(static_cast<SPipeIndexStructPrivate*>(a_handleInner)->hndl, a_timeoutMsInner);}
		//&WaitForCmpl
	);
	return readReturn;
}


sssize_t WriteToDataPipe(THandle a_handle, pindex_t a_pipeIndex, const void* a_buffer, size_t a_bufferSize)
{
	if(a_pipeIndex>=a_handle->numberOfWriteToChildDataPipes){return OUT_OF_INDEX;}
	if(!a_handle->pDataToChildPipes[a_pipeIndex].pw){return NO_HANDLE_EXIST2;}

	return WriteToHandle(a_handle->pDataToChildPipes[a_pipeIndex].pw, a_buffer, a_bufferSize);
}


static handle_t* HandleGetterForAllStatic(void* a_handle, pindex_t a_pipeIndex)
{
	THandle pHandle = static_cast<THandle>(a_handle);
	return &(static_cast<THandle>(a_handle)->pReadPipes[a_pipeIndex].pr);
}


pindex_t ReadFromAllPipes(THandle a_handle,void** a_buffers,const size_t* a_buffersSizes, sssize_t* a_pReadSize,int a_timeoutMs)
{
	return ReadFromManyPipes(
		a_handle,a_handle->numberOfReadFromChildPipes,
		[](void* a_handleInner, pindex_t a_pipeIndex) {return &(static_cast<THandle>(a_handleInner)->pReadPipes[a_pipeIndex].pr);},
		a_buffers,a_buffersSizes,a_pReadSize,a_timeoutMs,
		a_handle->fpWaiterFunction);
}


/*///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

#ifndef lblcontainer_of
#define lblcontainer_of(_ptr,_type,_member) (_type*)(  ((char*)(_ptr)) + (size_t)( (char*)(&((_type *)0)->_member) )  )
#endif

struct SPipeHelperData2 {
	uint64_t	sizeReaded;
	int32_t		indexOfReader;
	int32_t		errorCode;
};

struct SOverlapped2 {
	OVERLAPPED			ovrlp;
	SPipeHelperData2*	pHelper;
	pindex_t			index;
};


static DWORD WINAPI StdInputRedirectorThread( ::LPVOID a_lpThreadParameter)
{
	struct SHandle* pHandle = static_cast<struct SHandle*>(a_lpThreadParameter);
	HANDLE  stdinToRead = GetStdHandle(STD_INPUT_HANDLE);
	char vcBuffer[PIPES_SIZE];
	DWORD dwReadedSize;

	while (pHandle->shouldRun) {
		if(ReadFile(stdinToRead,vcBuffer,PIPES_SIZE,&dwReadedSize,CPPUTILS_NULL)&&(dwReadedSize>0)){
			WriteFile(pHandle->stdinToWriite.pw, vcBuffer, dwReadedSize, CPPUTILS_NULL, CPPUTILS_NULL);
		}
	}

	return 0;
}


static VOID WINAPI OVERLAPPED_READ_COMPLETION_ROUTINE_GEN_STAT2(
	_In_    DWORD a_dwErrorCode,
	_In_    DWORD a_dwNumberOfBytesTransfered,
	_Inout_ LPOVERLAPPED a_lpOverlapped)
{
	if ((a_dwErrorCode != ERROR_OPERATION_ABORTED) && (a_dwErrorCode != ERROR_MR_MID_NOT_FOUND) && (a_dwErrorCode != ERROR_SCOPE_NOT_FOUND)) {
		struct SOverlapped2* pPipeStr = lblcontainer_of(a_lpOverlapped, struct SOverlapped2, ovrlp);
		pPipeStr->pHelper->indexOfReader = int32_t(pPipeStr->index);
		pPipeStr->pHelper->errorCode = a_dwErrorCode;
		if (!a_dwErrorCode) {
			pPipeStr->pHelper->sizeReaded = a_dwNumberOfBytesTransfered;
		}
	}
}


static DWORD WINAPI StdOutputsRedirectorThread( ::LPVOID a_lpThreadParameter)
{
	struct SHandle* pHandle = static_cast<struct SHandle*>(a_lpThreadParameter);
	SOverlapped2   vOverlapped[2];
	SPipeHelperData2 helperStruct;
	pindex_t i;
	char vcBuffer[PIPES_SIZE];
	DWORD dwWaitReturn;
	DWORD nNumberOfValidPipes(0);
	HANDLE	vectReadStdOutOrErr[2]={CPPUTILS_NULL,CPPUTILS_NULL}, vectWriteStdOutOrErr[2]={CPPUTILS_NULL,CPPUTILS_NULL};

	vOverlapped[0].pHelper = &helperStruct;
	vOverlapped[1].pHelper = &helperStruct;

	vOverlapped[0].index = 0;
	vOverlapped[1].index = 1;

	memset(&(vOverlapped[0].ovrlp),0,sizeof(vOverlapped[0].ovrlp));
	memset(&(vOverlapped[1].ovrlp),0,sizeof(vOverlapped[1].ovrlp));

	if (pHandle->stdPipesUsed&USE_STANDARD_STDOUT) {
		vectReadStdOutOrErr[0] = pHandle->stdoutToRead;
		vectWriteStdOutOrErr[0] = GetStdHandle(STD_OUTPUT_HANDLE);
	}

	if (pHandle->stdPipesUsed&USE_STANDARD_STDERR) {
		vectReadStdOutOrErr[1] = pHandle->stderrToRead;
		vectWriteStdOutOrErr[1] = GetStdHandle(STD_ERROR_HANDLE);
	}

	while (pHandle->shouldRun) {

		helperStruct.sizeReaded = 0;
		helperStruct.indexOfReader = -1;
		helperStruct.errorCode = 0;
		nNumberOfValidPipes = 0;

		for(i=0;i<2;++i){
			if(vectReadStdOutOrErr[i]){
				if(ReadFileEx(vectReadStdOutOrErr[i],vcBuffer,PIPES_SIZE,&(vOverlapped[i].ovrlp),&OVERLAPPED_READ_COMPLETION_ROUTINE_GEN_STAT2)){
					++nNumberOfValidPipes;
				}
			}
		}

		if(nNumberOfValidPipes){
			dwWaitReturn = SleepEx(INFINITE, TRUE);
			if (
				(dwWaitReturn == WAIT_IO_COMPLETION) && (helperStruct.indexOfReader >= 0) &&
				(helperStruct.indexOfReader < 2) && (helperStruct.sizeReaded > 0) &&
				vectWriteStdOutOrErr[helperStruct.indexOfReader]) {
				WriteFile(vectWriteStdOutOrErr[helperStruct.indexOfReader], vcBuffer, static_cast<DWORD>(helperStruct.sizeReaded), CPPUTILS_NULL, CPPUTILS_NULL);
			}
		}  // if(nNumberOfValidPipes){

	}


	return 0;
}


VOID NTAPI UserAPCfunction(_In_ ULONG_PTR)
{
}


static void AfterProcessTermination(struct SHandle* a_handle)
{
	a_handle->isExeFinished = 1;
	if (a_handle->prcInfo.hThread) {
		CloseHandle(a_handle->prcInfo.hThread);
		a_handle->prcInfo.hThread = CPPUTILS_NULL;
	}

	if(a_handle->prcInfo.hProcess){
		DWORD dwExeitCode;
		GetExitCodeProcess(a_handle->prcInfo.hProcess,&dwExeitCode);
		CloseHandle(a_handle->prcInfo.hProcess);
		a_handle->prcInfo.hProcess = CPPUTILS_NULL;
		a_handle->exeReturnCode = static_cast<uint64_t>(dwExeitCode);
	}
}

static void ClearAllHandlesStatic(struct SHandle* a_handle)
{
	uint64_t i;

	if(!a_handle){return;}

	a_handle->shouldRun = 0;

	if(a_handle->prcInfo.hProcess || a_handle->prcInfo.hThread){
		WaitForSingleObject(a_handle->prcInfo.hProcess,INFINITE); // ? or we should wait interruptable?
		AfterProcessTermination(a_handle);
	}


	if(a_handle->stdOutputsRedirectorThread){
		QueueUserAPC(&UserAPCfunction,a_handle->stdOutputsRedirectorThread,0);
		WaitForSingleObject(a_handle->stdOutputsRedirectorThread,INFINITE);
		CloseHandle(a_handle->stdOutputsRedirectorThread);
		a_handle->stdOutputsRedirectorThread = CPPUTILS_NULL;
	}

	if(a_handle->stdInputRedirectorThread){
		CancelSynchronousIo(a_handle->stdInputRedirectorThread);
		WaitForSingleObject(a_handle->stdInputRedirectorThread,INFINITE);
		CloseHandle(a_handle->stdInputRedirectorThread);
		a_handle->stdInputRedirectorThread = CPPUTILS_NULL;
	}

	// standard pipes
	a_handle->stdinToWriite.DestroyPipeStruct();
	
	// read pipes
	//a_handle->pReadPipes[CONTROL_RD_EXE_PIPE].DestroyPipeStruct();
	for(i=0;i<a_handle->numberOfReadFromChildPipes;++i){
		a_handle->pReadPipes[i].DestroyPipeStruct();
	}

	if(a_handle->stdoutToRead){CloseHandle(a_handle->stdoutToRead);a_handle->stdoutToRead=CPPUTILS_NULL;}
	if(a_handle->stderrToRead){CloseHandle(a_handle->stderrToRead);a_handle->stderrToRead=CPPUTILS_NULL;}

	// write pipes
	a_handle->remoteControlPipeWrite.InitPipeStruct();
	for(i=0;i<a_handle->numberOfWriteToChildDataPipes;++i){
		a_handle->pDataToChildPipes[i].DestroyPipeStruct();
	}

}


static void Clear(struct SHandle* a_handle)
{
	ClearAllHandlesStatic(a_handle);
	free(a_handle->pDataToChildPipes);
	free(a_handle->pReadPipes);
	free(a_handle);
}

/*///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

int SPipeStruct::InitPipeStruct(void)
{
	SECURITY_ATTRIBUTES secAttr;

	secAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	secAttr.lpSecurityDescriptor = CPPUTILS_NULL;
	secAttr.bInheritHandle = TRUE;

#ifdef USE_ANON_PIPE
	if (!CreatePipe(&this->pr,&this->pw,&secAttr,PIPES_SIZE)) {
		this->DestroyPipeStruct();
		return -2;
	}
#else
	char vcPipeName[512];
	int nPid = GetCurrentProcessId();
	_snprintf_s(vcPipeName,511,511,"\\\\.\\pipe\\%d_%p",nPid,this);
	this->pw = CreateNamedPipeA(
		vcPipeName,             // pipe name 
		PIPE_ACCESS_OUTBOUND,    // read/write access 
		PIPE_TYPE_MESSAGE |       // message type pipe 
		PIPE_READMODE_MESSAGE |   // message-read mode 
		PIPE_WAIT,                // blocking mode 
		PIPE_UNLIMITED_INSTANCES, // max. instances  
		PIPES_SIZE,            // output buffer size 
		0,            // input buffer size 
		10000,                    // client time-out 
		&secAttr);

	if((!this->pw)||(this->pw==INVALID_HANDLE_VALUE)){
		this->DestroyPipeStruct();
		return -2;
	}

	this->pr = CreateFileA(vcPipeName, GENERIC_READ , FILE_SHARE_READ | FILE_SHARE_WRITE, &secAttr, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, CPPUTILS_NULL);
	//this->pr = CreateFileA(vcPipeName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, &secAttr, OPEN_EXISTING, 0, CPPUTILS_NULL);
	if((!this->pr)||(this->pr==INVALID_HANDLE_VALUE)){
		this->DestroyPipeStruct();
		return -3;
	}

	if((!ConnectNamedPipe(this->pw, CPPUTILS_NULL))&&(GetLastError()!=ERROR_PIPE_CONNECTED)){
		this->DestroyPipeStruct();
		return -4;
	}

#endif
	return 0;
}


void SPipeStruct::DestroyPipeStruct(void)
{
#ifndef USE_ANON_PIPE
	if (this->pr) { DisconnectNamedPipe(this->pr); }
	if (this->pw) { DisconnectNamedPipe(this->pw); }
#endif
	if (this->pr) { CloseHandle(this->pr); this->pr = CPPUTILS_NULL; }
	if (this->pw) { CloseHandle(this->pw); this->pw = CPPUTILS_NULL; }
}


}}}  //  namespace namespace systemN { namespace exe { namespace parent{
