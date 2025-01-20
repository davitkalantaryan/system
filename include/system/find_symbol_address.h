//
// file:            system_specific_definations.h
// created on:      2019 Oct 14
// created by:      Davit Kalantaryan (davit.kalantaryan@desy.de)
//
// This header is for functions, that is used to call system routines and binaries
//

#ifndef SYSTEM_INCLUDE_SYSTEM_FIND_SYMBOL_ADDRESS_H
#define SYSTEM_INCLUDE_SYSTEM_FIND_SYMBOL_ADDRESS_H


#include <system/export_symbols.h>

CPPUTILS_BEGIN_C


SYSTEM_EXPORT void* SystemFindSymbolAddress(const char* CPPUTILS_ARG_NN a_symbolLookupName) CPPUTILS_NOEXCEPT;


CPPUTILS_END_C



#endif  // #ifndef SYSTEM_INCLUDE_SYSTEM_FIND_SYMBOL_ADDRESS_H
