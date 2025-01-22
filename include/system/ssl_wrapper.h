//
// file:            system_specific_definations.h
// created on:      2019 Oct 14
// created by:      Davit Kalantaryan (davit.kalantaryan@desy.de)
//
// This header is for functions, that is used to call system routines and binaries
//

#ifndef SYSTEM_INCLUDE_SYSTEM_SSL_WRAPPER_H
#define SYSTEM_INCLUDE_SYSTEM_SSL_WRAPPER_H


#include <system/export_symbols.h>
#include <cinternal/disable_compiler_warnings.h>
#include <openssl/ssl.h>
#include <cinternal/undisable_compiler_warnings.h>

#ifdef SYSTEM_SSL_WRAPPER_EXISTS

CPPUTILS_BEGIN_C


SYSTEM_EXPORT int System_sslwrap_InitializeFunctions(void) CPPUTILS_NOEXCEPT;
// see: https://docs.openssl.org/3.2/man3/OPENSSL_init_ssl/#copyright
// The function returns 1 on success or 0 on error.
SYSTEM_EXPORT int System_sslwrap_OPENSSL_init_ssl(uint64_t a_opts, const OPENSSL_INIT_SETTINGS* a_settings) CPPUTILS_NOEXCEPT;
// see: https://docs.openssl.org/3.2/man3/OPENSSL_init_crypto/#synopsis
SYSTEM_EXPORT void System_sslwrap_OPENSSL_cleanup(void) CPPUTILS_NOEXCEPT;
// see: https://docs.openssl.org/3.2/man3/BIO_s_mem/
SYSTEM_EXPORT BIO* System_sslwrap_BIO_new_mem_buf(const void* a_buf, int a_len) CPPUTILS_NOEXCEPT;
// see: https://docs.openssl.org/3.2/man3/PEM_read_bio_PrivateKey/#synopsis
SYSTEM_EXPORT RSA* System_sslwrap_PEM_read_bio_RSA_PUBKEY(BIO* a_bp, RSA** a_x, pem_password_cb* a_cb, void* a_u) CPPUTILS_NOEXCEPT;
SYSTEM_EXPORT int System_sslwrap_BIO_free(BIO* a_bio) CPPUTILS_NOEXCEPT;
// see: https://docs.openssl.org/3.2/man3/EVP_PKEY_new/
SYSTEM_EXPORT EVP_PKEY* System_sslwrap_EVP_PKEY_new(void) CPPUTILS_NOEXCEPT;
// see: https://docs.openssl.org/3.2/man3/EVP_PKEY_set1_RSA/#description
SYSTEM_EXPORT int System_sslwrap_EVP_PKEY_assign(EVP_PKEY* a_pkey, int a_type, void* a_key) CPPUTILS_NOEXCEPT;
SYSTEM_EXPORT EVP_MD_CTX* System_sslwrap_EVP_MD_CTX_new(void) CPPUTILS_NOEXCEPT;
SYSTEM_EXPORT void System_sslwrap_EVP_PKEY_free(EVP_PKEY* a_pkey) CPPUTILS_NOEXCEPT;
SYSTEM_EXPORT int System_sslwrap_EVP_DigestVerifyInit(EVP_MD_CTX* a_ctx, EVP_PKEY_CTX** a_pctx,const EVP_MD* a_type, ENGINE* a_e, EVP_PKEY* a_pkey) CPPUTILS_NOEXCEPT;
SYSTEM_EXPORT int System_sslwrap_EVP_DigestUpdate(EVP_MD_CTX* a_ctx, const void* a_d,size_t a_cnt) CPPUTILS_NOEXCEPT;
SYSTEM_EXPORT int System_sslwrap_EVP_DigestVerifyFinal(EVP_MD_CTX* a_ctx, const unsigned char* a_sig, size_t a_siglen) CPPUTILS_NOEXCEPT;
SYSTEM_EXPORT const EVP_MD* System_sslwrap_EVP_sha256(void) CPPUTILS_NOEXCEPT;
SYSTEM_EXPORT void System_sslwrap_EVP_MD_CTX_free(EVP_MD_CTX* a_ctx) CPPUTILS_NOEXCEPT;


# ifndef OPENSSL_NO_RSA
#  define System_sslwrap_EVP_PKEY_assign_RSA(pkey,rsa) System_sslwrap_EVP_PKEY_assign((pkey),EVP_PKEY_RSA,(char *)(rsa))
# endif
# define System_sslwrap_EVP_DigestVerifyUpdate(a,b,c)   System_sslwrap_EVP_DigestUpdate(a,b,c)


CPPUTILS_END_C


#endif  //  #ifdef SYSTEM_SSL_WRAPPER_EXISTS

#endif  //  #ifndef SYSTEM_INCLUDE_SYSTEM_SSL_WRAPPER_H
