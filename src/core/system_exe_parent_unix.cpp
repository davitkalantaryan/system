//
// file:            common_system.cpp
// created on:      2019 Aug 26
// created by:      Davit Kalantaryan (davit.kalantaryan@desy.de)
//
// Here is functions, that is used to call system routines and binaries
//

#include <system/export_symbols.h>

#ifndef _WIN32

#include <system/exe/parent_child_common.hpp>
#include <private/system/exe_parent_child_common.h>
#include "system_include_private.hpp"
#include <string.h> // we hope that strcspn is rejecting case
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>
#include <alloca.h>
#include <string>

#ifndef SleepEx
#define SleepEx(_x,_alrt)   usleep(1000*(_x))
#endif

#ifdef SOCKET_ERROR
#undef SOCKET_ERROR
#endif
#define SOCKET_ERROR -1

#define ALLOCATION_COUNT_STEP   8


namespace systemN { namespace exe { namespace parent{

// TExecHandle RunExecutableNoWaitLine(char* argumentsLine)

#define MAX_ARGS_NUMBER         500
#define MAX_ARGS_NUMBER_ROUNDED 500

static const char* s_sccpAcceptReject = " \t\n\r";

static void Clear(struct SHandle* a_handle);


SYSTEM_EXPORT THandle RunNoWaitW(int8_t a_numberOfReadFromChildDataPipes, int8_t a_numberOfWriteToChildDataPipes,char* a_commandLine,const char* a_cpcAdditionalSearchPath, uint8_t a_bUsingStdPipes)
{
    THandle hReturn;
    size_t unStrStrResult;
    size_t argc, unAllocatedCountTmp, unAllocatedCount = 0;
    char* pcParsePointer = a_commandLine;
    char **ppcArgsTmp, **ppcArgs = static_cast<char**>(malloc(ALLOCATION_COUNT_STEP*sizeof(char*)));

    if(!ppcArgs){return CPPUTILS_NULL;}
    unAllocatedCount = ALLOCATION_COUNT_STEP;

    for(argc=0;*pcParsePointer;){
        unStrStrResult = strspn(pcParsePointer,s_sccpAcceptReject);
        pcParsePointer += unStrStrResult;
        if(argc>=unAllocatedCount){
            unAllocatedCountTmp = unAllocatedCount + ALLOCATION_COUNT_STEP;
            ppcArgsTmp = static_cast<char**>(realloc(ppcArgs,unAllocatedCountTmp*sizeof(char*)));
            if(!ppcArgsTmp){
                free(ppcArgs);
                return CPPUTILS_NULL;
            }
            ppcArgs=ppcArgsTmp;
        }
        ppcArgs[argc++] = pcParsePointer;
        unStrStrResult = strcspn(pcParsePointer,s_sccpAcceptReject);
        pcParsePointer += unStrStrResult;
        *pcParsePointer = 0;
        ++pcParsePointer;

    }

    hReturn = RunNoWaitU(a_numberOfReadFromChildDataPipes,a_numberOfWriteToChildDataPipes,ppcArgs,a_cpcAdditionalSearchPath,a_bUsingStdPipes);
    free(ppcArgs);
    return hReturn;
}


SYSTEM_EXPORT THandle RunNoWaitU(int8_t a_numberOfReadFromChildDataPipes, int8_t a_numberOfWriteToChildDataPipes, char* a_argv[], const char* a_cpcAdditionalSearchPath, uint8_t a_bUsingStdPipes)
{
    int8_t pipeIndex;
    int8_t numberOfReadFromChildPipes;
    int stdinCopy(-1),stdoutCopy(-1),stderrCopy(-1);
    ::std::string strNewPath;
    ::std::string dataPipeStr;
    const char* cpcArg0initial = a_argv[0];
    THandle pHandle = static_cast<THandle>(calloc(1,sizeof(struct SHandle)));

    if(!pHandle){return CPPUTILS_NULL;}

    // SHandle() { this->pid = -1; this->waited = 0; this->shouldWait = this->cleared = 1; }
    pHandle->pid = -1;pHandle->reserved64bit = 1;pHandle->shouldWait2 = 0;
    pHandle->stdinToWriite.pipes[0]=pHandle->stdinToWriite.pipes[1]=-1;

    if(a_numberOfWriteToChildDataPipes>0){
        pHandle->pDataToChildPipes = static_cast<struct SPipeStruct*>(calloc(static_cast<size_t>(a_numberOfWriteToChildDataPipes),sizeof(struct SPipeStruct)));
        if (!pHandle->pDataToChildPipes) {
            Clear(pHandle);
            return CPPUTILS_NULL;
        }
        pHandle->numberOfWriteToChildDataPipes = static_cast<uint64_t>(a_numberOfWriteToChildDataPipes);
    }

    numberOfReadFromChildPipes = DATA_0_FROM_CHILD_EXE_PIPE + ((a_numberOfReadFromChildDataPipes >0)?a_numberOfReadFromChildDataPipes:0);
    pHandle->pReadPipes = static_cast<struct SPipeStruct*>(calloc(CPPUTILS_STATIC_CAST(size_t,numberOfReadFromChildPipes),sizeof(struct SPipeStruct)));
    if (!pHandle->pReadPipes) {
        Clear(pHandle);
        return CPPUTILS_NULL;
    }
    pHandle->numberOfReadFromChildPipes = static_cast<uint64_t>(numberOfReadFromChildPipes);

    for(pipeIndex=DATA_0_FROM_CHILD_EXE_PIPE;pipeIndex<numberOfReadFromChildPipes;++pipeIndex){
        if(pipe(pHandle->pReadPipes[pipeIndex].pipes)){goto errorReturnPoint;}
    }

    for(pipeIndex=0;pipeIndex<a_numberOfWriteToChildDataPipes;++pipeIndex){
        if(pipe(pHandle->pDataToChildPipes[pipeIndex].pipes)){goto errorReturnPoint;}
    }

    if(pipe(pHandle->pReadPipes[CONTROL_RD_EXE_PIPE].pipes)){goto errorReturnPoint;}
    if(pipe(pHandle->remoteControlPipeWrite.pipes)){goto errorReturnPoint;}

    if (!(a_bUsingStdPipes&USE_STANDARD_STDIN)) {
        if(pipe(pHandle->stdinToWriite.pipes)){goto errorReturnPoint;}
        stdinCopy = dup(STDIN_FILENO);
        dup2(pHandle->stdinToWriite.pipes[0],STDIN_FILENO);
    }
    if (!(a_bUsingStdPipes&USE_STANDARD_STDOUT)) {
        if(pipe(pHandle->pReadPipes[STDOUT_EXE_PIPE].pipes)){goto errorReturnPoint;}
        stdoutCopy = dup(STDOUT_FILENO);
        dup2(pHandle->pReadPipes[STDOUT_EXE_PIPE].pipes[1],STDOUT_FILENO);
    }
    if (!(a_bUsingStdPipes&USE_STANDARD_STDERR)) {
        if(pipe(pHandle->pReadPipes[STDERR_EXE_PIPE].pipes)){goto errorReturnPoint;}
        stderrCopy = dup(STDERR_FILENO);
        dup2(pHandle->pReadPipes[STDERR_EXE_PIPE].pipes[1],STDERR_FILENO);
    }

#ifdef TRY_TO_SKIP_COPY_ON_CHANGE

    setenv(COMMON_SYSTEM_EXE_DATAPIPE_TO_PARENT,::std::to_string(vnDataPipeToParent[1]).c_str(),1);
    setenv(COMMON_SYSTEM_EXE_DATAPIPE_FROM_PARENT,::std::to_string(vnDataPipeFromParent[0]).c_str(),1);
    setenv(COMMON_SYSTEM_EXE_CONTROLPIPE_TO_PARENT,::std::to_string(vnControlPipeToParent[1]).c_str(),1);
    setenv(COMMON_SYSTEM_EXE_CONTROLPIPE_FROM_PARENT,::std::to_string(vnControlPipeFromParent[0]).c_str(),1);

#endif

    pHandle->pid = fork();

    if(!pHandle->pid){

#ifndef TRY_TO_SKIP_COPY_ON_CHANGE

        printf("!!!!!!!!!!!! pid=%d,line=%d\n",static_cast<int>(getpid()),__LINE__);

        //printf("press any key, then press enter to contiue!\n");fflush(stdout);getchar();
        sleep(16);

        //if(pHandle->stdinToWriite.pipes[1]>0){
        //    close(pHandle->stdinToWriite.pipes[1]);
        //    pHandle->stdinToWriite.pipes[1]=-1;
        //}
        pHandle->stdinToWriite.pipes[0]=-1;
        //if(pHandle->pReadPipes[STDOUT_EXE_PIPE].pipes[0]>0){
        //    close(pHandle->pReadPipes[STDOUT_EXE_PIPE].pipes[0]);
        //    pHandle->pReadPipes[STDOUT_EXE_PIPE].pipes[0]=-1;
        //}
        pHandle->pReadPipes[STDOUT_EXE_PIPE].pipes[1]=-1;
        //if(pHandle->pReadPipes[STDERR_EXE_PIPE].pipes[0]>0){
        //    close(pHandle->pReadPipes[STDERR_EXE_PIPE].pipes[0]);
        //    pHandle->pReadPipes[STDERR_EXE_PIPE].pipes[0]=-1;
        //}

        setenv(COMMON_SYSTEM_EXE_DATAPIPE_TO_PARENT_NUMBER,::std::to_string(a_numberOfReadFromChildDataPipes).c_str(),1);
        setenv(COMMON_SYSTEM_EXE_DATAPIPE_FROM_PARENT_NUMBER,::std::to_string(a_numberOfWriteToChildDataPipes).c_str(),1);

        pHandle->pReadPipes[STDERR_EXE_PIPE].pipes[1]=-1;
        setenv(COMMON_SYSTEM_EXE_CONTROLPIPE_TO_PARENT,::std::to_string(pHandle->pReadPipes[CONTROL_RD_EXE_PIPE].pipes[1]).c_str(),1);
        pHandle->pReadPipes[CONTROL_RD_EXE_PIPE].pipes[1] = -1;
        setenv(COMMON_SYSTEM_EXE_CONTROLPIPE_FROM_PARENT,::std::to_string(pHandle->remoteControlPipeWrite.pipes[0]).c_str(),1);
        pHandle->remoteControlPipeWrite.pipes[0]=-1;

        for(pipeIndex=0;pipeIndex<a_numberOfReadFromChildDataPipes;++pipeIndex){
            dataPipeStr = COMMON_SYSTEM_EXE_DATAPIPE_TO_PARENT_B + ::std::to_string(pipeIndex);
            setenv(dataPipeStr.c_str(),::std::to_string(pHandle->pReadPipes[DATA_0_FROM_CHILD_EXE_PIPE + pipeIndex].pipes[1]).c_str(),1);
            pHandle->pReadPipes[DATA_0_FROM_CHILD_EXE_PIPE + pipeIndex].pipes[1]=-1;
        }
        for(pipeIndex=0;pipeIndex< a_numberOfWriteToChildDataPipes;++pipeIndex){
            dataPipeStr = COMMON_SYSTEM_EXE_DATAPIPE_FROM_PARENT_B + ::std::to_string(pipeIndex);
            setenv(dataPipeStr.c_str(),::std::to_string(pHandle->pDataToChildPipes[pipeIndex].pipes[0]).c_str(),1);
            pHandle->pDataToChildPipes[pipeIndex].pipes[0]=-1;
        }

        Clear(pHandle);pHandle=CPPUTILS_NULL;

#endif
        //execvpe(a_argv[0],a_argv,a_envp); // ???????????????????????????????????
        // For time being instead of execvpe I use code below
        execvp(a_argv[0],a_argv);

        if(a_cpcAdditionalSearchPath){
            char *pcAdditionalSearchPathInitial = strdup(a_cpcAdditionalSearchPath);
            if(pcAdditionalSearchPathInitial){
                char *pcAdditionalSearchPathTmp, *pcAdditionalSearchPath(pcAdditionalSearchPathInitial);
                while(pcAdditionalSearchPath){
                    pcAdditionalSearchPathTmp = strchr(pcAdditionalSearchPath,':');
                    if(pcAdditionalSearchPathTmp){*pcAdditionalSearchPathTmp=0;}
                    strNewPath = ::std::string(pcAdditionalSearchPath) + "/" + cpcArg0initial;
                    a_argv[0] = const_cast<char*>(strNewPath.c_str());
                    execvp(a_argv[0],a_argv);
                    pcAdditionalSearchPath = pcAdditionalSearchPathTmp?(pcAdditionalSearchPathTmp+1):CPPUTILS_NULL;
                }
                free(pcAdditionalSearchPathInitial);
            }  // if(pcAdditionalSearchPathInitial){
        }  // if(a_cpcAdditionalSearchPath){

        // if we are here,
        // this means we have
        // a problem to run execv

        //dup2(stdinCopy,STDIN_FILENO);
        //dup2(stdoutCopy,STDOUT_FILENO);
        //dup2(stderrCopy,STDERR_FILENO);

        fprintf(stderr,"Unable to run executable: %s\n",a_argv[0]);  // this will go to our error pipe
        perror("\n");

        // int vnStdin[2]={-1,-1}, vnStdout[2]={-1,-1}, vnStderr[2]={-1,-1};
        // int vnControlPipeToParent[2]={-1,-1}, vnControlPipeFromParent[2]={-1,-1}, vnDataPipeToParent[2]={-1,-1}, vnDataPipeFromParent[2]={-1,-1};
#ifdef TRY_TO_SKIP_COPY_ON_CHANGE
#endif

        _Exit(1);
    }
    else{

        if(pHandle->stdinToWriite.pipes[0]>0){
            close(pHandle->stdinToWriite.pipes[0]);
            pHandle->stdinToWriite.pipes[0]=-1;
        }

        if(pHandle->remoteControlPipeWrite.pipes[0]>0){
            close(pHandle->remoteControlPipeWrite.pipes[0]);
            pHandle->remoteControlPipeWrite.pipes[0]=-1;
        }

        for(pipeIndex=0;pipeIndex<a_numberOfWriteToChildDataPipes;++pipeIndex){
            if(pHandle->pDataToChildPipes[pipeIndex].pipes[0]>0){
                close(pHandle->pDataToChildPipes[pipeIndex].pipes[0]);
                pHandle->pDataToChildPipes[pipeIndex].pipes[0]=-1;
            }
        }

        for(pipeIndex=0;pipeIndex<numberOfReadFromChildPipes;++pipeIndex){
            if(pHandle->pReadPipes[pipeIndex].pipes[1]>0){
                close(pHandle->pReadPipes[pipeIndex].pipes[1]);
                pHandle->pReadPipes[pipeIndex].pipes[1]=-1;
            }
        }

        // these 3 lines done bu compiler
        //a_pHandle->controlPipe[0].pipe = -1;
        //a_pHandle->controlPipe[1].pipe = -1;
        //a_pHandle->waited = 0;
    }

returnPoint:

    if(stdinCopy>=0){
        dup2(stdinCopy,STDIN_FILENO);
        close(stdinCopy);
    }

    if(stdoutCopy>=0){
        dup2(stdoutCopy,STDOUT_FILENO);
        close(stdoutCopy);
    }

    if(stderrCopy>=0){
        dup2(stderrCopy,STDERR_FILENO);
        close(stderrCopy);
    }

    return pHandle;

errorReturnPoint:

    Clear(pHandle);
    pHandle = CPPUTILS_NULL;
    goto returnPoint;

    return CPPUTILS_NULL;
}


static inline handle_t* HandleGetterFunction1(void* a_handleInner, pindex_t a_pipeIndex){return &(static_cast<THandle>(a_handleInner)->pReadPipes[a_pipeIndex].pipes[0]);}
pindex_t ReadFromAllPipes(THandle a_handle,void** a_buffers,const size_t* a_buffersSizes, sssize_t* a_pReadSize,int a_timeoutMs)
{
    return ReadFromManyPipes(
        a_handle,a_handle->numberOfReadFromChildPipes,
        //[](void* a_handleInner, pindex_t a_pipeIndex) {return &(static_cast<THandle>(a_handleInner)->pReadPipes[a_pipeIndex].pr);},
        &HandleGetterFunction1,
        a_buffers,a_buffersSizes,a_pReadSize,a_timeoutMs,
        CPPUTILS_NULL);
}


static inline handle_t* HandleGetterFunction2(void* a_handleInner, pindex_t){return static_cast<handle_t*>(a_handleInner);}
sssize_t ReadDataFromPipe(THandle a_handle, pindex_t a_pipeIndex, void* a_buffer, size_t a_bufferSize, int a_timeoutMs)
{
    void* vBuffers[] = {a_buffer};
    size_t vBuffersSizes[] = {a_bufferSize};
    sssize_t readReturn(-1);

    ReadFromManyPipes(
        &(a_handle->pReadPipes[DATA_0_FROM_CHILD_EXE_PIPE+a_pipeIndex].pipes[0]),1,
        //[](void* a_handleInner, pindex_t a_pipeIndex) {return &(static_cast<THandle>(a_handleInner)->pReadPipes[a_pipeIndex].pr);},
        &HandleGetterFunction2,
        vBuffers,vBuffersSizes,&readReturn,a_timeoutMs,
        CPPUTILS_NULL);

    return readReturn;
}


sssize_t WriteToStdIn(THandle a_handle,const void* a_buffer, size_t a_bufferSize)
{
    return WriteToHandle(a_handle->stdinToWriite.pipes[1], a_buffer, a_bufferSize);
}


sssize_t WriteToDataPipe(THandle a_handle, pindex_t a_pipeIndex, const void* a_buffer, size_t a_bufferSize)
{
    if(a_pipeIndex>=a_handle->numberOfWriteToChildDataPipes){return OUT_OF_INDEX;}
    if(!a_handle->pDataToChildPipes[a_pipeIndex].pipes[1]){return NO_HANDLE_EXIST2;}

    return WriteToHandle(a_handle->pDataToChildPipes[a_pipeIndex].pipes[1], a_buffer, a_bufferSize);
}


bool WaitAndClear(THandle a_handle,int a_timeoutMs, int* a_exeReturnCodePtr)
{
    int nWaited = 0;
    pid_t w;
    int status;
    fd_set rfds, efds;
    int nTry,maxsd(a_handle->pReadPipes[CONTROL_RD_EXE_PIPE].pipes[0]+1);
    struct timeval		aTimeout;
    struct timeval*		pTimeout;

    FD_ZERO( &rfds );
    FD_ZERO( &efds );

    FD_SET( a_handle->pReadPipes[CONTROL_RD_EXE_PIPE].pipes[0], &rfds );
    FD_SET( a_handle->pReadPipes[CONTROL_RD_EXE_PIPE].pipes[0], &efds );

    if( a_timeoutMs >= 0 ){
        aTimeout.tv_sec = a_timeoutMs / 1000;
        aTimeout.tv_usec = (a_timeoutMs%1000)*1000 ;
        pTimeout = &aTimeout;
    }
    else{pTimeout = CPPUTILS_NULL;}

    nTry = select(++maxsd, &rfds, CPPUTILS_NULL, &efds, pTimeout );

    switch(nTry)
    {
    case 0:	/* time out */
        return false;
    case -1:
        if( errno == EINTR ){
            /* interrupted by signal */
            // todo: kill child and return true;
            return false;
            //return COMMON_SYSTEM_RW_INTERRUPTED;
        }

        //return COMMON_SYSTEM_UNKNOWN;
        // todo: kill child and return true;
        return false;
    default:
        // we can read
        break;
    }

    a_handle->shouldWait2=1;

    do {
        w = waitpid(a_handle->pid, &status, WUNTRACED | WCONTINUED);
        //w = waitpid(a_handle->pid, &status, WUNTRACED | WCONTINUED|WNOHANG);
        if (w == -1) {
            if(errno == EINTR){
                printf("waitpid: interrupted! handle->shouldWait = %d\n",static_cast<int>(a_handle->shouldWait2));
                return EINTR;
            }
            else{
                perror("waitpid:");
                return -1;
            }
        } else if(w != a_handle->pid){
            //goto sleepPoint;
        } else if (WIFEXITED(status)) {
            printf("exited, status=%d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("killed by signal %d\n", WTERMSIG(status));
        } else if (WIFSTOPPED(status)) {
            printf("stopped by signal %d\n", WSTOPSIG(status));
        } else if (WIFCONTINUED(status)) {
            printf("continued\n");
        }

        if(WIFEXITED(status)){
            nWaited = 1;
            if(a_exeReturnCodePtr){*a_exeReturnCodePtr=WIFEXITED(status);}
            //return WEXITSTATUS(status);
            break;
        }
        //SleepEx(1,TRUE);
    }
    //while (!WIFEXITED(status) && !WIFSIGNALED(status) && a_handle->shouldWait);
    while (a_handle->shouldWait2);

    if(nWaited){
        Clear(a_handle);
        return true;
    }

    return false;
}


//uint64_t IsExeFinished(THandle a_handle)
//{
//    return a_handle->waited;
//}


/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/



static void ClearAllHandlesStatic(struct SHandle* a_handle)
{
    uint64_t pipeIndex;

    //if(!a_handle){return;}

    for(pipeIndex=0;pipeIndex<a_handle->numberOfReadFromChildPipes;++pipeIndex){
        if(a_handle->pReadPipes[pipeIndex].pipes[0]>0){
            close(a_handle->pReadPipes[pipeIndex].pipes[0]);
            a_handle->pReadPipes[pipeIndex].pipes[0]=-1;
        }
        if(a_handle->pReadPipes[pipeIndex].pipes[1]>0){
            close(a_handle->pReadPipes[pipeIndex].pipes[1]);
            a_handle->pReadPipes[pipeIndex].pipes[1]=-1;
        }
    }

    for(pipeIndex=0;pipeIndex<a_handle->numberOfWriteToChildDataPipes;++pipeIndex){
        if(a_handle->pDataToChildPipes[pipeIndex].pipes[0]>0){
            close(a_handle->pDataToChildPipes[pipeIndex].pipes[0]);
            a_handle->pDataToChildPipes[pipeIndex].pipes[0]=-1;
        }
        if(a_handle->pDataToChildPipes[pipeIndex].pipes[1]>0){
            close(a_handle->pDataToChildPipes[pipeIndex].pipes[1]);
            a_handle->pDataToChildPipes[pipeIndex].pipes[1]=-1;
        }
    }

    // remoteControlPipeWrite
    if(a_handle->remoteControlPipeWrite.pipes[0]>0){
        close(a_handle->remoteControlPipeWrite.pipes[0]);
        a_handle->remoteControlPipeWrite.pipes[0]=-1;
    }
    if(a_handle->remoteControlPipeWrite.pipes[1]>0){
        close(a_handle->remoteControlPipeWrite.pipes[1]);
        a_handle->remoteControlPipeWrite.pipes[1]=-1;
    }

    // stdinToWriite
    if(a_handle->stdinToWriite.pipes[0]>0){
        close(a_handle->stdinToWriite.pipes[0]);
        a_handle->stdinToWriite.pipes[0]=-1;
    }
    if(a_handle->stdinToWriite.pipes[1]>0){
        close(a_handle->stdinToWriite.pipes[1]);
        a_handle->stdinToWriite.pipes[1]=-1;
    }
}


static void Clear(struct SHandle* a_handle)
{
    if(a_handle){
        ClearAllHandlesStatic(a_handle);
        free(a_handle->pDataToChildPipes);
        free(a_handle->pReadPipes);
        free(a_handle);
    }
}



}}}  //  namespace systemN { namespace exe { namespace parent{


#endif  //  #ifndef _WIN32
