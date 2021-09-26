//
// file:            common/system/exe/parent.hpp
// created on:      2019 Aug 26
// created by:      Davit Kalantaryan (davit.kalantaryan@desy.de)
//
// This header is for functions, that is used to call system routines and binaries
//

#ifndef COMMON_SYSTEM_EXE_PARENT_HPP
#define COMMON_SYSTEM_EXE_PARENT_HPP

#include <system_internal_header.h>
#include <system/exe/parent_child_common.hpp>

#define EXE_WAS_NOT_FOUND				1

namespace systemN { namespace exe { namespace parent{

typedef struct SHandle* THandle;

THandle  RunNoWaitU(int8_t a_numberOfReadFromChildDataPipes, int8_t a_numberOfWriteToChildDataPipes,char* argv[], const char* a_cpcAdditionalSearchPath=SYSTEM_NULL, uint8_t a_bUsingStdPipes=0);
THandle  RunNoWaitW(int8_t a_numberOfReadFromChildDataPipes, int8_t a_numberOfWriteToChildDataPipes,char* commandLine,const char* a_cpcAdditionalSearchPath=SYSTEM_NULL, uint8_t a_bUsingStdPipes=0);
pindex_t ReadFromAllPipes(THandle handle,void** buffers,const size_t* buffersSizes, sssize_t* pReadSize,int timeoutMs);
sssize_t ReadFromDataPipe(THandle handle, pindex_t index, void* buffer, size_t bufferSize, int timeoutMs);
sssize_t ReadFromAnyPipe(THandle handle, pindex_t index, void* buffer, size_t bufferSize, int timeoutMs);
sssize_t WriteToStdIn(THandle handle,const void* buffer, size_t bufferSize);
sssize_t WriteToDataPipe(THandle handle, pindex_t pipeIndex, const void* buffer, size_t bufferSize);
bool     WaitAndClear(THandle handle,int timeoutMs, int* exeReturnCodePtr);
bool     Clear(THandle a_handle,int* a_exeReturnCodePtr);
//uint64_t IsExeFinished(THandle handle);

}}} // namespace systemN { namespace exe { namespace parent{

#ifndef SYSTEM_CPP_11_DEFINED
namespace std{

::std::string to_string(int a_nValue);

}
#endif


#endif  // #ifndef COMMON_SYSTEM_EXE_PARENT_HPP
