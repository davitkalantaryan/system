//
// file:            common/system/exe/parent.hpp
// created on:      2019 Aug 26
// created by:      Davit Kalantaryan (davit.kalantaryan@desy.de)
//
// This header is for functions, that is used to call system routines and binaries
//

#ifndef COMMON_SYSTEM_EXE_PARENT_HPP
#define COMMON_SYSTEM_EXE_PARENT_HPP

#include <daq/data_handling/export_symbols.h>
#include <common/system/exe/parent_child_common.hpp>

#define EXE_WAS_NOT_FOUND				1

namespace common{ namespace system { namespace exe { namespace parent{

typedef struct SHandle* THandle;

DAQ_DH_EXPORT THandle  RunNoWaitU(int8_t a_numberOfReadFromChildDataPipes, int8_t a_numberOfWriteToChildDataPipes,char* argv[], const char* a_cpcAdditionalSearchPath=CPPUTILS_NULL, uint8_t a_bUsingStdPipes=0);
DAQ_DH_EXPORT THandle  RunNoWaitW(int8_t a_numberOfReadFromChildDataPipes, int8_t a_numberOfWriteToChildDataPipes,char* commandLine,const char* a_cpcAdditionalSearchPath=CPPUTILS_NULL, uint8_t a_bUsingStdPipes=0);
DAQ_DH_EXPORT pindex_t ReadFromAllPipes(THandle handle,void** buffers,const size_t* buffersSizes, sssize_t* pReadSize,int timeoutMs);
DAQ_DH_EXPORT sssize_t ReadDataFromPipe(THandle handle, pindex_t index, void* buffer, size_t bufferSize, int timeoutMs);
DAQ_DH_EXPORT sssize_t WriteToStdIn(THandle handle,const void* buffer, size_t bufferSize);
DAQ_DH_EXPORT sssize_t WriteToDataPipe(THandle handle, pindex_t pipeIndex, const void* buffer, size_t bufferSize);
DAQ_DH_EXPORT bool     WaitAndClear(THandle handle,int timeoutMs, int* exeReturnCodePtr);
//uint64_t IsExeFinished(THandle handle);

}}}} // namespace common{ namespace system { namespace exe { namespace parent{

#ifndef CPPUTILS_CPP_11_DEFINED
namespace std{

::std::string to_string(int a_nValue);

}
#endif


#endif  // #ifndef COMMON_SYSTEM_EXE_PARENT_HPP
