//
// file:            common/system/handlelib.hpp
// created on:      2019 Sep 1
// created by:      Davit Kalantaryan (davit.kalantaryan@desy.de)
//
// This header is for functions, that is used to call system routines and binaries
//

#ifndef COMMON_SYSTEM_EXE_CHILD_HPP
#define COMMON_SYSTEM_EXE_CHILD_HPP

#include <system/exe/parent_child_common.hpp>


namespace systemN { namespace exe { namespace child {

bool IsCommunicationWithParentInited(void);

pindex_t ReadDataFromAllParentPipes(void** buffers, const size_t* buffersSizes, sssize_t* pReadSize, int timeoutMs);
sssize_t ReadDataFromParent(pindex_t index, void* buff, size_t count, int timeoutMs);
sssize_t WriteDataToParent(pindex_t index, const void* buff, size_t count);


}}} // namespace systemN { namespace exe { namespace child {



#endif  // #ifndef COMMON_SYSTEM_EXE_CHILD_HPP
