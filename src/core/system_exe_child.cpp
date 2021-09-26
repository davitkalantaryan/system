//
// file:            common_system_exe_child.cpp
// created on:      2019 Aug 26
// created by:      Davit Kalantaryan (davit.kalantaryan@desy.de)
//
// Here is functions, that is used to call system routines and binaries
//

#include <system_internal_header.h>
#include <system/exe/child.hpp>
#include <system/rw.hpp>
#include <src/system_exe_parent_child_common.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#ifdef _MSC_VER
#ifdef snprintf
#undef snprintf
#endif
#define snprintf(_buf,_bufSize,...)		_snprintf_s((_buf),(_bufSize),(_bufSize),__VA_ARGS__)
#pragma warning (disable:4996)
#endif
#ifdef _WIN32
#define CLOSEDPIPE2	SYSTEM_NULL
#else
#define	CloseHandle	close
#define CLOSEDPIPE2	-1
#endif

namespace systemN { namespace exe { namespace child {

static void InitializeCommunicationWithParent (void);
static void CleanupCommunicationWithParent (void);

class InitDeinit1{
public:
    InitDeinit1(){InitializeCommunicationWithParent();}
    ~InitDeinit1(){CleanupCommunicationWithParent();}
}static s_initDeinit;

static int		 s_shouldRun = 0;
static bool		 s_bIsInited = false;
static pindex_t	 s_nNumberOfDataPipesToParent = 0;
static pindex_t	 s_nNumberOfDataPipesFromParent = 0;
static handle_t* s_pnDataPipeToParent = SYSTEM_NULL;
static handle_t* s_pnDataPipeFromParent = SYSTEM_NULL;
static handle_t  s_nControlPipeToParent = UNINITED_HANDLE;
static handle_t  s_nControlPipeFromParent = UNINITED_HANDLE;


bool IsCommunicationWithParentInited(void)
{
    return s_bIsInited;
}


pindex_t ReadDataFromAllParentPipes(void** a_buffers, const size_t* a_buffersSizes, sssize_t* a_pReadSize, int a_timeoutMs)
{

	if(!s_pnDataPipeFromParent){return NO_HANDLE_EXIST2;}
	return ReadFromManyPipes(
		s_pnDataPipeFromParent, s_nNumberOfDataPipesFromParent,
		[](void* a_handleInner, pindex_t a_pipeIndex) {return &(static_cast<handle_t*>(a_handleInner)[a_pipeIndex]); },
        a_buffers, a_buffersSizes, a_pReadSize, a_timeoutMs,SYSTEM_NULL);
}


sssize_t ReadDataFromParent(pindex_t a_pipeIndex, void* a_buffer, size_t a_bufferSize, int a_timeoutMs)
{
	sssize_t readReturn(0);
	void* vBuffers[1] = { a_buffer };
	const size_t vBufferSizes[1] = { a_bufferSize };

	if (!s_pnDataPipeFromParent) { return NO_HANDLE_EXIST2; }
	if (a_pipeIndex >= s_nNumberOfDataPipesFromParent) { return OUT_OF_INDEX; }
	ReadFromManyPipes(
		&s_pnDataPipeFromParent, 1,
		[](void* a_handleInner, pindex_t) {return static_cast<handle_t*>(a_handleInner); },
        vBuffers, vBufferSizes, &readReturn, a_timeoutMs,SYSTEM_NULL);
	return readReturn;
}


sssize_t WriteDataToParent(pindex_t a_pipeIndex, const void* a_buffer, size_t a_bufferSize)
{
	if (!s_pnDataPipeToParent) { return NO_HANDLE_EXIST2; }
	if (a_pipeIndex >= s_nNumberOfDataPipesToParent) { return OUT_OF_INDEX; }


	return WriteToHandle(s_pnDataPipeToParent[a_pipeIndex], a_buffer, a_bufferSize);
}

static void InitializeCommunicationWithParent(void)
{
    //{
    //        sigset_t sigSet;
    //        int nSigNum;
    //
    //        sigemptyset(&sigSet);
    //        sigaddset(&sigSet,SIGINT);
    //        // let us use depricated API here (instead of sigaction)
    //        signal(SIGINT, SIG_IGN);
    //        dprintf(STDOUT_FILENO,"waiting for signal %d(SIGINT) [ kill -%d %d ] \n",SIGINT,SIGINT,static_cast<int>(getpid()));
    //        sigwait(&sigSet,&nSigNum);
    //        signal(SIGINT, SIG_DFL);
    //}

	char vcBuffer[1024];
	pindex_t i;
    char* pcPipeString = getenv(COMMON_SYSTEM_EXE_DATAPIPE_TO_PARENT_NUMBER);
    if(pcPipeString){ s_nNumberOfDataPipesToParent = SYSTEM_STATIC_CAST(pindex_t,atoi(pcPipeString)); }

	pcPipeString = getenv(COMMON_SYSTEM_EXE_DATAPIPE_FROM_PARENT_NUMBER);
    if(pcPipeString){ s_nNumberOfDataPipesFromParent = SYSTEM_STATIC_CAST(pindex_t,atoi(pcPipeString)); }
    
    pcPipeString = getenv(COMMON_SYSTEM_EXE_CONTROLPIPE_TO_PARENT);
    if(pcPipeString){ s_nControlPipeToParent = SYSTEM_HANDLE_CAST(handle_t,strtoll(pcPipeString,SYSTEM_NULL,10)); }
    
	pcPipeString = getenv(COMMON_SYSTEM_EXE_CONTROLPIPE_FROM_PARENT);
    if(pcPipeString){ s_nControlPipeFromParent = SYSTEM_HANDLE_CAST(handle_t,strtoll(pcPipeString,SYSTEM_NULL,10)); }

	if(s_nNumberOfDataPipesToParent>0){
        s_pnDataPipeToParent = static_cast<handle_t*>(calloc(SYSTEM_STATIC_CAST(size_t,s_nNumberOfDataPipesToParent),sizeof(handle_t)));
		if(!s_pnDataPipeToParent){
			return;
		}

		for (i = 0; i < s_nNumberOfDataPipesToParent;++i) {
			snprintf(vcBuffer,1023, COMMON_SYSTEM_EXE_DATAPIPE_TO_PARENT_F,int(i));
			pcPipeString = getenv(vcBuffer);
            if(pcPipeString){ s_pnDataPipeToParent[i] = SYSTEM_HANDLE_CAST(handle_t,strtoll(pcPipeString, SYSTEM_NULL, 10)); }
		}
	}


	if(s_nNumberOfDataPipesFromParent>0){
        s_pnDataPipeFromParent = static_cast<handle_t*>(calloc(SYSTEM_STATIC_CAST(size_t,s_nNumberOfDataPipesFromParent),sizeof(handle_t)));
		if(!s_pnDataPipeFromParent){
			CleanupCommunicationWithParent();
			return;
		}

		for (i = 0; i < s_nNumberOfDataPipesFromParent;++i) {
			snprintf(vcBuffer,1023, COMMON_SYSTEM_EXE_DATAPIPE_FROM_PARENT_F,int(i));
			pcPipeString = getenv(vcBuffer);
            if(pcPipeString){ s_pnDataPipeFromParent[i] = SYSTEM_HANDLE_CAST(handle_t,strtoll(pcPipeString, SYSTEM_NULL, 10)); }
		}
	}

	
	s_shouldRun = 1;
    s_bIsInited =true;
}


static void CleanupCommunicationWithParent(void)
{
    //if(s_nDataPipeToParent>0){
    //    close(s_nDataPipeToParent);
    //    s_nDataPipeToParent = -1;
    //}
	//
    //if(s_nDataPipeFromParent>0){
    //    close(s_nDataPipeFromParent);
    //    s_nDataPipeFromParent = -1;
    //}
	//
    //if(s_nControlPipeToParent>0){
    //    close(s_nControlPipeToParent);
    //    s_nControlPipeToParent = -1;
    //}
	//
    //if(s_nControlPipeFromParent>0){
    //    close(s_nControlPipeFromParent);
    //    s_nControlPipeFromParent = -1;
    //}

	// todo: close the handles;
	pindex_t i;

	for (i = 0; i < s_nNumberOfDataPipesToParent; ++i) {
		if (s_pnDataPipeToParent[i]!=CLOSEDPIPE2) {
			CloseHandle(s_pnDataPipeToParent[i]);
			s_pnDataPipeToParent[i] = CLOSEDPIPE2;
		}
	}

	for (i = 0; i < s_nNumberOfDataPipesFromParent; ++i) {
		if (s_pnDataPipeFromParent[i]!=CLOSEDPIPE2) {
			CloseHandle(s_pnDataPipeFromParent[i]);
			s_pnDataPipeFromParent[i] = CLOSEDPIPE2;
		}
	}

	if(s_nControlPipeToParent!=CLOSEDPIPE2){
		CloseHandle(s_nControlPipeToParent);
		s_nControlPipeToParent = CLOSEDPIPE2;
	}
	if(s_nControlPipeFromParent!=CLOSEDPIPE2){
		CloseHandle(s_nControlPipeFromParent);
		s_nControlPipeFromParent = CLOSEDPIPE2;
	}


	free(s_pnDataPipeFromParent);
	free(s_pnDataPipeToParent);
        s_pnDataPipeFromParent = SYSTEM_NULL;
        s_pnDataPipeToParent = SYSTEM_NULL;
}

#ifndef __GNUC__
class ClassConstructDestruct {
public:
	ClassConstructDestruct(){InitializeCommunicationWithParent();}
	~ClassConstructDestruct(){ CleanupCommunicationWithParent();}
}static s_classConstructDestruct;
#endif

}}} // namespace systemN { namespace exe { namespace child {
