//
// repo:            system
// file:            system_ssl_wrapper.cpp
// created on:      2025 Jan 21
// created by:      Davit Kalantaryan (davit.kalantaryan@desy.de)
//


#include <system/ssl_wrapper.h>


#ifdef SYSTEM_SSL_WRAPPER_EXISTS

#include <system/find_symbol_address.h>


CPPUTILS_BEGIN_C


typedef int             (*Type_System_sslwrap_OPENSSL_init_ssl)(uint64_t, const OPENSSL_INIT_SETTINGS*);
typedef void            (*Type_System_sslwrap_OPENSSL_cleanup)(void);
typedef BIO*            (*Type_System_sslwrap_BIO_new_mem_buf)(const void*, int);
typedef RSA*            (*Type_System_sslwrap_PEM_read_bio_RSA_PUBKEY)(BIO*, RSA**, pem_password_cb*, void*);
typedef int             (*Type_System_sslwrap_BIO_free)(BIO*);
typedef EVP_PKEY*       (*Type_System_sslwrap_EVP_PKEY_new)(void);
typedef int             (*Type_System_sslwrap_EVP_PKEY_assign)(EVP_PKEY*, int, void*);
typedef EVP_MD_CTX*     (*Type_System_sslwrap_EVP_MD_CTX_new)(void);
typedef void            (*Type_System_sslwrap_EVP_PKEY_free)(EVP_PKEY*);
typedef int             (*Type_System_sslwrap_EVP_DigestVerifyInit)(EVP_MD_CTX*, EVP_PKEY_CTX**,const EVP_MD*, ENGINE*, EVP_PKEY*);
typedef int             (*Type_System_sslwrap_EVP_DigestUpdate)(EVP_MD_CTX*, const void*,size_t);
typedef int             (*Type_System_sslwrap_EVP_DigestVerifyFinal)(EVP_MD_CTX*, const unsigned char*, size_t);
typedef const EVP_MD*   (*Type_System_sslwrap_EVP_sha256)(void);
typedef void            (*Type_System_sslwrap_EVP_MD_CTX_free)(EVP_MD_CTX*);


static Type_System_sslwrap_OPENSSL_init_ssl         s_System_sslwrap_OPENSSL_init_ssl = CPPUTILS_NULL;
static Type_System_sslwrap_OPENSSL_cleanup          s_System_sslwrap_OPENSSL_cleanup = CPPUTILS_NULL;
static Type_System_sslwrap_BIO_new_mem_buf          s_System_sslwrap_BIO_new_mem_buf = CPPUTILS_NULL;
static Type_System_sslwrap_PEM_read_bio_RSA_PUBKEY  s_System_sslwrap_PEM_read_bio_RSA_PUBKEY = CPPUTILS_NULL;
static Type_System_sslwrap_BIO_free                 s_System_sslwrap_BIO_free = CPPUTILS_NULL;
static Type_System_sslwrap_EVP_PKEY_new             s_System_sslwrap_EVP_PKEY_new = CPPUTILS_NULL;
static Type_System_sslwrap_EVP_PKEY_assign          s_System_sslwrap_EVP_PKEY_assign = CPPUTILS_NULL;
static Type_System_sslwrap_EVP_MD_CTX_new           s_System_sslwrap_EVP_MD_CTX_new = CPPUTILS_NULL;
static Type_System_sslwrap_EVP_PKEY_free            s_System_sslwrap_EVP_PKEY_free = CPPUTILS_NULL;
static Type_System_sslwrap_EVP_DigestVerifyInit     s_System_sslwrap_EVP_DigestVerifyInit = CPPUTILS_NULL;
static Type_System_sslwrap_EVP_DigestUpdate         s_System_sslwrap_EVP_DigestUpdate = CPPUTILS_NULL;
static Type_System_sslwrap_EVP_DigestVerifyFinal    s_System_sslwrap_EVP_DigestVerifyFinal = CPPUTILS_NULL;
static Type_System_sslwrap_EVP_sha256               s_System_sslwrap_EVP_sha256 = CPPUTILS_NULL;
static Type_System_sslwrap_EVP_MD_CTX_free          s_System_sslwrap_EVP_MD_CTX_free = CPPUTILS_NULL;


SYSTEM_EXPORT int System_sslwrap_InitializeFunctions(void) CPPUTILS_NOEXCEPT
{
    s_System_sslwrap_OPENSSL_init_ssl = (Type_System_sslwrap_OPENSSL_init_ssl)SystemFindSymbolAddress("OPENSSL_init_ssl");
    if(!s_System_sslwrap_OPENSSL_init_ssl){
        return 1;
    }

    s_System_sslwrap_OPENSSL_cleanup = (Type_System_sslwrap_OPENSSL_cleanup)SystemFindSymbolAddress("OPENSSL_cleanup");
    if(!s_System_sslwrap_OPENSSL_cleanup){
        return 1;
    }

    s_System_sslwrap_BIO_new_mem_buf = (Type_System_sslwrap_BIO_new_mem_buf)SystemFindSymbolAddress("BIO_new_mem_buf");
    if(!s_System_sslwrap_BIO_new_mem_buf){
        return 1;
    }

    s_System_sslwrap_PEM_read_bio_RSA_PUBKEY = (Type_System_sslwrap_PEM_read_bio_RSA_PUBKEY)SystemFindSymbolAddress("PEM_read_bio_RSA_PUBKEY");
    if(!s_System_sslwrap_PEM_read_bio_RSA_PUBKEY){
        return 1;
    }

    s_System_sslwrap_BIO_free = (Type_System_sslwrap_BIO_free)SystemFindSymbolAddress("BIO_free");
    if(!s_System_sslwrap_BIO_free){
        return 1;
    }

    s_System_sslwrap_EVP_PKEY_new = (Type_System_sslwrap_EVP_PKEY_new)SystemFindSymbolAddress("EVP_PKEY_new");
    if(!s_System_sslwrap_EVP_PKEY_new){
        return 1;
    }

    s_System_sslwrap_EVP_PKEY_assign = (Type_System_sslwrap_EVP_PKEY_assign)SystemFindSymbolAddress("EVP_PKEY_assign");
    if(!s_System_sslwrap_EVP_PKEY_assign){
        return 1;
    }

    s_System_sslwrap_EVP_MD_CTX_new = (Type_System_sslwrap_EVP_MD_CTX_new)SystemFindSymbolAddress("EVP_MD_CTX_new");
    if(!s_System_sslwrap_EVP_MD_CTX_new){
        return 1;
    }

    s_System_sslwrap_EVP_PKEY_free = (Type_System_sslwrap_EVP_PKEY_free)SystemFindSymbolAddress("EVP_PKEY_free");
    if(!s_System_sslwrap_EVP_PKEY_free){
        return 1;
    }

    s_System_sslwrap_EVP_DigestVerifyInit = (Type_System_sslwrap_EVP_DigestVerifyInit)SystemFindSymbolAddress("EVP_DigestVerifyInit");
    if(!s_System_sslwrap_EVP_DigestVerifyInit){
        return 1;
    }

    s_System_sslwrap_EVP_DigestUpdate = (Type_System_sslwrap_EVP_DigestUpdate)SystemFindSymbolAddress("EVP_DigestUpdate");
    if(!s_System_sslwrap_EVP_DigestUpdate){
        return 1;
    }

    s_System_sslwrap_EVP_DigestVerifyFinal = (Type_System_sslwrap_EVP_DigestVerifyFinal)SystemFindSymbolAddress("EVP_DigestVerifyFinal");
    if(!s_System_sslwrap_EVP_DigestVerifyFinal){
        return 1;
    }

    s_System_sslwrap_EVP_sha256 = (Type_System_sslwrap_EVP_sha256)SystemFindSymbolAddress("EVP_sha256");
    if(!s_System_sslwrap_EVP_sha256){
        return 1;
    }

    s_System_sslwrap_EVP_MD_CTX_free = (Type_System_sslwrap_EVP_MD_CTX_free)SystemFindSymbolAddress("EVP_MD_CTX_free");
    if(!s_System_sslwrap_EVP_MD_CTX_free){
        return 1;
    }

    return 0;
}


SYSTEM_EXPORT int System_sslwrap_OPENSSL_init_ssl(uint64_t a_opts, const OPENSSL_INIT_SETTINGS* a_settings) CPPUTILS_NOEXCEPT
{
    return (*s_System_sslwrap_OPENSSL_init_ssl)(a_opts,a_settings);
}


SYSTEM_EXPORT void System_sslwrap_OPENSSL_cleanup(void) CPPUTILS_NOEXCEPT
{
    (*s_System_sslwrap_OPENSSL_cleanup)();
}


SYSTEM_EXPORT BIO* System_sslwrap_BIO_new_mem_buf(const void* a_buf, int a_len) CPPUTILS_NOEXCEPT
{
    return (*s_System_sslwrap_BIO_new_mem_buf)(a_buf, a_len);
}


SYSTEM_EXPORT RSA* System_sslwrap_PEM_read_bio_RSA_PUBKEY(BIO* a_bp, RSA** a_x, pem_password_cb* a_cb, void* a_u) CPPUTILS_NOEXCEPT
{
    return (*s_System_sslwrap_PEM_read_bio_RSA_PUBKEY)(a_bp,a_x,a_cb,a_u);
}


SYSTEM_EXPORT int System_sslwrap_BIO_free(BIO* a_bio) CPPUTILS_NOEXCEPT
{
    return (*s_System_sslwrap_BIO_free)(a_bio);
}


SYSTEM_EXPORT EVP_PKEY* System_sslwrap_EVP_PKEY_new(void) CPPUTILS_NOEXCEPT
{
    return (*s_System_sslwrap_EVP_PKEY_new)();
}


SYSTEM_EXPORT int System_sslwrap_EVP_PKEY_assign(EVP_PKEY* a_pkey, int a_type, void* a_key) CPPUTILS_NOEXCEPT
{
    return (*s_System_sslwrap_EVP_PKEY_assign)(a_pkey,a_type,a_key);
}


SYSTEM_EXPORT EVP_MD_CTX* System_sslwrap_EVP_MD_CTX_new(void) CPPUTILS_NOEXCEPT
{
    return (*s_System_sslwrap_EVP_MD_CTX_new)();
}


SYSTEM_EXPORT void System_sslwrap_EVP_PKEY_free(EVP_PKEY* a_pkey) CPPUTILS_NOEXCEPT
{
    (*s_System_sslwrap_EVP_PKEY_free)(a_pkey);
}


SYSTEM_EXPORT int System_sslwrap_EVP_DigestVerifyInit(EVP_MD_CTX* a_ctx, EVP_PKEY_CTX** a_pctx,const EVP_MD* a_type, ENGINE* a_e, EVP_PKEY* a_pkey) CPPUTILS_NOEXCEPT
{
    return (*s_System_sslwrap_EVP_DigestVerifyInit)(a_ctx,a_pctx,a_type,a_e,a_pkey);
}


SYSTEM_EXPORT int System_sslwrap_EVP_DigestUpdate(EVP_MD_CTX* a_ctx, const void* a_d,size_t a_cnt) CPPUTILS_NOEXCEPT
{
    return (*s_System_sslwrap_EVP_DigestUpdate)(a_ctx,a_d,a_cnt);
}


SYSTEM_EXPORT int System_sslwrap_EVP_DigestVerifyFinal(EVP_MD_CTX* a_ctx, const unsigned char* a_sig, size_t a_siglen) CPPUTILS_NOEXCEPT
{
    return (*s_System_sslwrap_EVP_DigestVerifyFinal)(a_ctx,a_sig, a_siglen);
}


SYSTEM_EXPORT const EVP_MD* System_sslwrap_EVP_sha256(void) CPPUTILS_NOEXCEPT
{
    return (*s_System_sslwrap_EVP_sha256)();
}


SYSTEM_EXPORT void System_sslwrap_EVP_MD_CTX_free(EVP_MD_CTX* a_ctx) CPPUTILS_NOEXCEPT
{
    (*s_System_sslwrap_EVP_MD_CTX_free)(a_ctx);
}


CPPUTILS_END_C


#endif  //  #ifdef SYSTEM_SSL_WRAPPER_EXISTS
