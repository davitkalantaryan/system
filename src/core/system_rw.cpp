//
// file:            common_system.cpp
// created on:      2019 Aug 26
// created by:      Davit Kalantaryan (davit.kalantaryan@desy.de)
//
// Here is functions, that is used to call system routines and binaries
//

#include <system/rw.hpp>
#include <vector>
#include <utility>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

namespace systemN { 

#ifdef _WIN32

#ifndef lblcontainer_of
#define lblcontainer_of(_ptr,_type,_member) (_type*)(  ((char*)(_ptr)) + (size_t)( (char*)(&((_type *)0)->_member) )  )
#endif

struct SPipeHelperData{
	uint64_t	sizeReaded;
	int32_t		indexOfReader;
	int32_t		errorCode;
};

struct SOverlapped {
	OVERLAPPED			ovrlp;
	SPipeHelperData*	pHelper;
	pindex_t			index;
};

static VOID WINAPI OVERLAPPED_READ_COMPLETION_ROUTINE_GEN_STAT(
	_In_    DWORD a_dwErrorCode,
	_In_    DWORD a_dwNumberOfBytesTransfered,
	_Inout_ LPOVERLAPPED a_lpOverlapped);

static thread_local int  stl_isAsyncIoDone = 0;


sssize_t WriteToHandle(handle_t a_pipeHandle, const void* a_buffer, size_t a_bufferSize)
{
	DWORD dwSize;
	BOOL bRet(WriteFile(a_pipeHandle, a_buffer, static_cast<DWORD>(a_bufferSize), &dwSize, CPPUTILS_NULL));
	return bRet ? dwSize : -1;
}


pindex_t ReadFromManyPipes(
	void* a_handlesParent, pindex_t a_handlesCount, HandleGetterType a_fpHandleGetter, 
	void** a_buffers, const size_t* a_buffersSizes, sssize_t* a_pReadSize, int a_timeoutMs, WaitFunctionType a_fpWait)
{
#if 0

	pindex_t ssnReturn(COMMON_SYSTEM_UNKNOWN);
	pindex_t i;
	handle_t* pCurHandle;
	int nNumberOfValidPipes(0);
	pindex_t dwWaitIndex;
	HANDLE* pHandles = static_cast<HANDLE*>(calloc(a_handlesCount, sizeof(HANDLE)));

	if (!pHandles) {
		return COMMON_SYSTEM_RW_NO_MEM; // no need to jump to return for cleanup
	}

	for (i = 0; i < a_handlesCount; ++i) {
		if (a_buffersSizes[i]) {
			pCurHandle = (*a_fpHandleGetter)(a_handlesParent, i);
			if (*pCurHandle) {
				pHandles[nNumberOfValidPipes++] = *pCurHandle;
			}
		}

	}

	if (!nNumberOfValidPipes) {
		ssnReturn = NO_HANDLE_EXIST2;
		goto returnPoint;
	}

	dwWaitIndex = static_cast<pindex_t>(WaitForMultipleObjects(nNumberOfValidPipes,pHandles,TRUE,a_timeoutMs));
	dwWaitIndex -= WAIT_OBJECT_0;

	if((dwWaitIndex<0)||(dwWaitIndex>=a_handlesCount)){
		ssnReturn = OUT_OF_INDEX;
		goto returnPoint;
	}


	if(!ReadFile(pHandles[dwWaitIndex],a_buffers[dwWaitIndex],static_cast<DWORD>(a_buffersSizes[dwWaitIndex]),a_pReadSize,CPPUTILS_NULL)){
		ssnReturn = UNABLE_TO_READ;
		goto returnPoint;
	}
	
	ssnReturn = dwWaitIndex;
returnPoint:
	free(pHandles);
	return ssnReturn;

#else
	pindex_t ssnReturn(COMMON_SYSTEM_UNKNOWN);
	pindex_t i;
	int nWaitReturn;
	handle_t* pCurHandle;
	BOOL bRetByReadEx;
	DWORD nNumberOfValidPipes(0);
	SPipeHelperData aHelper{0,-1};
	SOverlapped* pOverlapped = static_cast<SOverlapped*>( calloc(a_handlesCount,sizeof(struct SOverlapped)) );

	if(!pOverlapped){
		return COMMON_SYSTEM_RW_NO_MEM; // no need to jump to return for cleanup
	}

	if(!a_fpWait){
		a_fpWait = [](void*, int a_timeoutMs) {return SleepEx(a_timeoutMs,TRUE);};
	}

	for(i=0;i< a_handlesCount;++i){
		if(a_buffersSizes[i]){
			pCurHandle = (*a_fpHandleGetter)(a_handlesParent, i);
			if(*pCurHandle){
				pOverlapped[i].index = i;
				pOverlapped[i].pHelper = &aHelper;
				bRetByReadEx = ReadFileEx(
					*pCurHandle,
					a_buffers[i],
					static_cast<DWORD>(a_buffersSizes[i]),
					&pOverlapped[i].ovrlp,
					&OVERLAPPED_READ_COMPLETION_ROUTINE_GEN_STAT);
				if(bRetByReadEx){++nNumberOfValidPipes;}
				else{CloseHandle(*pCurHandle);*pCurHandle=CPPUTILS_NULL;}
			}
		}
	
	}

	if(!nNumberOfValidPipes){
		ssnReturn = NO_HANDLE_EXIST2;
		goto returnPoint; 
	}

	while((!aHelper.errorCode)&&(!aHelper.sizeReaded)){
		if ( (nWaitReturn=(*a_fpWait)(a_handlesParent,a_timeoutMs)) != WAIT_IO_COMPLETION) {
			ssnReturn = (nWaitReturn== WAIT_TIMEOUT)?COMMON_SYSTEM_TIMEOUT:(COMMON_SYSTEM_RW_INTERRUPTED-nWaitReturn);
			goto returnPoint;
		}

		if((aHelper.sizeReaded<1)&&(!stl_isAsyncIoDone)){
			ssnReturn = COMMON_SYSTEM_RW_INTERRUPTED;
			goto returnPoint;
		}
		stl_isAsyncIoDone = 0;
	}
	
	if(aHelper.indexOfReader<0){
		ssnReturn = OUT_OF_INDEX;
		goto returnPoint;
	}

	if (aHelper.errorCode) {
		pCurHandle = (*a_fpHandleGetter)(a_handlesParent, aHelper.indexOfReader);
		CloseHandle(*pCurHandle); *pCurHandle = CPPUTILS_NULL;
		ssnReturn = COMMON_SYSTEM_PIPE_CLOSED;
		goto returnPoint;
	}

	*a_pReadSize = static_cast<sssize_t>(aHelper.sizeReaded);
	ssnReturn= aHelper.indexOfReader;
returnPoint:

	for(i=0;i< a_handlesCount;++i){
		if(a_buffersSizes[i]&&(i!= ssnReturn)){
			pCurHandle = (*a_fpHandleGetter)(a_handlesParent, i);
			if(*pCurHandle){
				CancelIo(*pCurHandle);
				SleepEx(1,TRUE);
				stl_isAsyncIoDone = 0;
			}
		}
	
	}
	//SleepEx(1, TRUE); // wait for io comletion
	free(pOverlapped);
	return ssnReturn;
#endif
}


/*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
static VOID WINAPI OVERLAPPED_READ_COMPLETION_ROUTINE_GEN_STAT(
	_In_    DWORD a_dwErrorCode,
	_In_    DWORD a_dwNumberOfBytesTransfered,
	_Inout_ LPOVERLAPPED a_lpOverlapped)
{
	stl_isAsyncIoDone = 1;
	if((a_dwErrorCode!=ERROR_OPERATION_ABORTED)&&(a_dwErrorCode!=ERROR_MR_MID_NOT_FOUND)&&(a_dwErrorCode!=ERROR_SCOPE_NOT_FOUND) ){
		struct SOverlapped* pPipeStr = lblcontainer_of(a_lpOverlapped, struct SOverlapped, ovrlp);
		pPipeStr->pHelper->indexOfReader = int32_t(pPipeStr->index);
		pPipeStr->pHelper->errorCode = a_dwErrorCode;
		if (!a_dwErrorCode) {
			pPipeStr->pHelper->sizeReaded = a_dwNumberOfBytesTransfered;
		}
	}
}


#else    // #ifdef _WIN32


SYSTEM_EXPORT sssize_t WriteToHandle(handle_t a_pipeHandle, const void* a_buffer, size_t a_bufferSize)
{
        return write(a_pipeHandle, a_buffer, a_bufferSize);
}


SYSTEM_EXPORT pindex_t ReadFromManyPipes(
    void* a_handlesParent, pindex_t a_handlesCount, HandleGetterType a_fpHandleGetter,
    void** a_buffers, const size_t* a_buffersSizes, sssize_t* a_pReadSize, int a_timeoutMs, WaitFunctionType)
{
    fd_set rfds, efds;
    int nTry, maxsd(0);
    //SPipeStruct* vcFds = a_handle->readPipes;
    //readCode::Type retCode = readCode::RCerror;
    pindex_t i;
    size_t validPipeIndex, nNumberOfPipes;
    handle_t* pCurHandle;
    struct timeval		aTimeout2;
    struct timeval*		pTimeout;
    ::std::vector< ::std::pair< handle_t,pindex_t > > vectOfPipes;

    FD_ZERO( &rfds );
    FD_ZERO( &efds );

    for(i=0,nNumberOfPipes=0;i<a_handlesCount;++i){
        pCurHandle = (*a_fpHandleGetter)(a_handlesParent, i);
        if(*pCurHandle>=0){
            //++nNumberOfPipes;
            FD_SET( *pCurHandle, &rfds );
            FD_SET( *pCurHandle, &efds );
            vectOfPipes.push_back( ::std::make_pair(*pCurHandle,i));

            if(*pCurHandle>maxsd){
                maxsd = *pCurHandle;
            }
        }
    }  // for(i=0;i<NUMBER_OF_STANDARD_READ_PIPES;++i){

    nNumberOfPipes = vectOfPipes.size();

    if(nNumberOfPipes<1){
        return NO_HANDLE_EXIST2;
    }

    if( a_timeoutMs >= 0 ){
        aTimeout2.tv_sec = a_timeoutMs / 1000;
        aTimeout2.tv_usec = (a_timeoutMs%1000)*1000 ;
        pTimeout = &aTimeout2;
    }
    else{
        pTimeout = CPPUTILS_NULL;
    }

    nTry = select(++maxsd, &rfds, CPPUTILS_NULL, &efds, pTimeout );

    switch(nTry)
    {
    case 0:	/* time out */
        return COMMON_SYSTEM_TIMEOUT;
    case -1:
        if( errno == EINTR ){
            /* interrupted by signal */
            return COMMON_SYSTEM_RW_INTERRUPTED;
        }

        return COMMON_SYSTEM_UNKNOWN;
    default:
        // we can read
        break;
    }

    for(validPipeIndex=0;validPipeIndex<nNumberOfPipes;++validPipeIndex){
        if (FD_ISSET(vectOfPipes[validPipeIndex].first, &rfds)){
            a_pReadSize[vectOfPipes[validPipeIndex].second] = read(vectOfPipes[validPipeIndex].first,a_buffers[vectOfPipes[validPipeIndex].second],a_buffersSizes[vectOfPipes[validPipeIndex].second]);

            if(a_pReadSize[vectOfPipes[validPipeIndex].second]>0){
                return vectOfPipes[validPipeIndex].second;
            }
            pCurHandle = (*a_fpHandleGetter)(a_handlesParent, vectOfPipes[validPipeIndex].second);
            close(*pCurHandle);
            *pCurHandle = -1;
        }
        else if(FD_ISSET(vectOfPipes[validPipeIndex].first, &efds)){
            pCurHandle = (*a_fpHandleGetter)(a_handlesParent, vectOfPipes[validPipeIndex].second);
            close(*pCurHandle);
            *pCurHandle = -1;
        }
    }

    return -1;
}

#endif  // #ifdef _WIN32



}  //  namespace systemN { 

#ifndef CPPUTILS_CPP_11_DEFINED
namespace std{

::std::string to_string(int a_nValue)
{
    char vcBuffer[128];
    snprintf(vcBuffer,127,"%d",a_nValue);
    return vcBuffer;
}

}
#endif
