//
// repo:            system
// file:            system_ssl_wrapper.cpp
// created on:      2025 Jan 21
// created by:      Davit Kalantaryan (davit.kalantaryan@desy.de)
//


#include <system/ssl_wrapper.h>


#ifdef SYSTEM_SSL_WRAPPER_EXISTS

#include <system/find_symbol_address.h>
#include <cinternal/disable_compiler_warnings.h>
#ifdef __APPLE__
#include <stdlib.h>
#include <Security/Security.h>
#include <CoreFoundation/CoreFoundation.h>
#include <openssl/evp.h>
#endif
#include <cinternal/undisable_compiler_warnings.h>


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
typedef int             (*Type_System_sslwrap_BIO_read)(BIO*, void*, int);
#ifdef __APPLE__
typedef SecKeyRef       (*Type_System_sslwrap_SecCertificateCopyKey)(SecCertificateRef);
typedef SecCertificateRef   (*Type_System_sslwrap_SecCertificateCreateWithData)(CFAllocatorRef __nullable , CFDataRef );
#endif


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
static Type_System_sslwrap_BIO_read                 s_System_sslwrap_BIO_read = CPPUTILS_NULL;
#ifdef __APPLE__
static Type_System_sslwrap_SecCertificateCopyKey    s_System_sslwrap_SecCertificateCopyKey = CPPUTILS_NULL;
static Type_System_sslwrap_SecCertificateCreateWithData            s_System_sslwrap_SecCertificateCreateWithData = CPPUTILS_NULL;
// SecCertificateRef SecCertificateCreateWithData(CFAllocatorRef __nullable allocator, CFDataRef data);

#endif


SYSTEM_EXPORT int System_sslwrap_InitializeFunctions(void) CPPUTILS_NOEXCEPT
{
    s_System_sslwrap_OPENSSL_init_ssl = (Type_System_sslwrap_OPENSSL_init_ssl)SystemFindSymbolAddress("OPENSSL_init_ssl");
    //if(!s_System_sslwrap_OPENSSL_init_ssl){
    //    return 1;
    //}

    s_System_sslwrap_OPENSSL_cleanup = (Type_System_sslwrap_OPENSSL_cleanup)SystemFindSymbolAddress("OPENSSL_cleanup");
    //if(!s_System_sslwrap_OPENSSL_cleanup){
    //    return 1;
    //}

    s_System_sslwrap_BIO_new_mem_buf = (Type_System_sslwrap_BIO_new_mem_buf)SystemFindSymbolAddress("BIO_new_mem_buf");
    if(!s_System_sslwrap_BIO_new_mem_buf){
        return 1;
    }

    s_System_sslwrap_PEM_read_bio_RSA_PUBKEY = (Type_System_sslwrap_PEM_read_bio_RSA_PUBKEY)SystemFindSymbolAddress("PEM_read_bio_RSA_PUBKEY");
#ifndef __APPLE__
    if(!s_System_sslwrap_PEM_read_bio_RSA_PUBKEY){
        return 1;
    }
#endif

    s_System_sslwrap_BIO_free = (Type_System_sslwrap_BIO_free)SystemFindSymbolAddress("BIO_free");
    if(!s_System_sslwrap_BIO_free){
        return 1;
    }

    s_System_sslwrap_EVP_PKEY_new = (Type_System_sslwrap_EVP_PKEY_new)SystemFindSymbolAddress("EVP_PKEY_new");
    if(!s_System_sslwrap_EVP_PKEY_new){
        return 1;
    }

    s_System_sslwrap_EVP_PKEY_assign = (Type_System_sslwrap_EVP_PKEY_assign)SystemFindSymbolAddress("EVP_PKEY_assign");
#ifndef __APPLE__
    if(!s_System_sslwrap_EVP_PKEY_assign){
        return 1;
    }
#endif

    s_System_sslwrap_EVP_MD_CTX_new = (Type_System_sslwrap_EVP_MD_CTX_new)SystemFindSymbolAddress("EVP_MD_CTX_new");    
#ifndef __APPLE__
    if(!s_System_sslwrap_EVP_MD_CTX_new){
        return 1;
    }
#endif

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
#ifndef __APPLE__
    if(!s_System_sslwrap_EVP_MD_CTX_free){
        return 1;
    }
#endif

    s_System_sslwrap_BIO_read = (Type_System_sslwrap_BIO_read)SystemFindSymbolAddress("BIO_read");
    if(!s_System_sslwrap_BIO_read){
        return 1;
    }

#ifdef __APPLE__

    s_System_sslwrap_SecCertificateCopyKey = (Type_System_sslwrap_SecCertificateCopyKey)SystemFindSymbolAddress("SecCertificateCopyKey");
    if(!s_System_sslwrap_SecCertificateCopyKey){
        return 1;
    }

    s_System_sslwrap_SecCertificateCreateWithData = (Type_System_sslwrap_SecCertificateCreateWithData)SystemFindSymbolAddress("SecCertificateCreateWithData");
    if(!s_System_sslwrap_SecCertificateCreateWithData){
        return 1;
    }

#endif

    return 0;
}


#ifdef __APPLE__

static SecKeyRef System_sslwrap_SecCertificateCopyKey(SecCertificateRef a_certRef) CPPUTILS_NOEXCEPT
{
    return (*s_System_sslwrap_SecCertificateCopyKey)(a_certRef);
}


static SecCertificateRef System_sslwrap_SecCertificateCreateWithData(CFAllocatorRef __nullable a_allocator, CFDataRef a_data) CPPUTILS_NOEXCEPT
{
    return (*s_System_sslwrap_SecCertificateCreateWithData)(a_allocator,a_data);
}


static long System_sslwrap_BIO_get_mem_data(BIO* a_bp, char** a_pemData) CPPUTILS_NOEXCEPT
{
    if (!a_bp || !a_pemData) {
        return -1; // Invalid input
    }

    // Estimate the size of the BIO buffer (using a sufficiently large buffer)
    constexpr size_t bufferSize = 8192; // Adjust based on expected input size
    char* buffer = static_cast<char*>(malloc(bufferSize));
    if (!buffer) {
        return -1; // Memory allocation failure
    }

    // Read data from BIO into the buffer
    int bytesRead = System_sslwrap_BIO_read(a_bp, buffer, bufferSize);
    if (bytesRead <= 0) {
        free(buffer); // Cleanup in case of failure
        return -1;    // Read failure or no data available
    }

    // Assign the buffer to the output pointer and return the number of bytes read
    *a_pemData = buffer;
    return bytesRead;
}


static inline int System_sslwrap_EVP_PKEY_assignInline(EVP_PKEY* a_pkey, int a_type, void* a_key) CPPUTILS_NOEXCEPT {
    if (!a_pkey || !a_key) {
        return 0; // Failure due to invalid input
    }

    // Set the type and key
    a_pkey->type = a_type;
    a_pkey->pkey.ptr = a_key;

    return 1; // Success
}


static inline int System_sslwrap_EVP_MD_CTX_resetInline(EVP_MD_CTX* a_ctx) CPPUTILS_NOEXCEPT {
    if(a_ctx){
        free(a_ctx->md_data);
        memset(a_ctx,0,sizeof(EVP_MD_CTX));
        return 1;
    }

    return 0;
}


#endif  //  #ifdef __APPLE__


SYSTEM_EXPORT int System_sslwrap_OPENSSL_init_ssl(uint64_t a_opts, const OPENSSL_INIT_SETTINGS* a_settings) CPPUTILS_NOEXCEPT
{
    if(s_System_sslwrap_OPENSSL_init_ssl){
        return (*s_System_sslwrap_OPENSSL_init_ssl)(a_opts,a_settings);
    }
    return 1;
}


SYSTEM_EXPORT void System_sslwrap_OPENSSL_cleanup(void) CPPUTILS_NOEXCEPT
{
    if(s_System_sslwrap_OPENSSL_cleanup){
        (*s_System_sslwrap_OPENSSL_cleanup)();
    }
}


SYSTEM_EXPORT BIO* System_sslwrap_BIO_new_mem_buf(const void* a_buf, int a_len) CPPUTILS_NOEXCEPT
{
    return (*s_System_sslwrap_BIO_new_mem_buf)(a_buf, a_len);
}


SYSTEM_EXPORT RSA* System_sslwrap_PEM_read_bio_RSA_PUBKEY(BIO* a_bp, RSA** a_x, pem_password_cb* a_cb, void* a_u) CPPUTILS_NOEXCEPT
{
#ifdef __APPLE__

    if(s_System_sslwrap_PEM_read_bio_RSA_PUBKEY){
        return (*s_System_sslwrap_PEM_read_bio_RSA_PUBKEY)(a_bp,a_x,a_cb,a_u);
    }

    // Use the custom BIO_get_mem_data equivalent
    char* pemData = nullptr;
    long pemLength = System_sslwrap_BIO_get_mem_data(a_bp, &pemData);
    if (pemLength <= 0 || !pemData) {
        return nullptr; // Failed to read from BIO
    }

    // Create CFDataRef from PEM data
    CFDataRef publicKeyData = CFDataCreate(nullptr, reinterpret_cast<const UInt8*>(pemData), pemLength);
    free(pemData); // Free the buffer after creating CFDataRef
    if (!publicKeyData) {
        return nullptr; // Failed to create CFData
    }

    // Create SecCertificateRef from CFDataRef
    SecCertificateRef certificate = System_sslwrap_SecCertificateCreateWithData(nullptr, publicKeyData);
    CFRelease(publicKeyData);

    if (!certificate) {
        return nullptr; // Failed to parse PEM as certificate
    }

    // Extract SecKeyRef from the certificate
    SecKeyRef publicKey = System_sslwrap_SecCertificateCopyKey(certificate);
    CFRelease(certificate);

    if (!publicKey) {
        return nullptr; // Failed to extract SecKey
    }

    // Placeholder: macOS doesn't support direct RSA conversions
    CFRelease(publicKey);
    return nullptr;

#else
    return (*s_System_sslwrap_PEM_read_bio_RSA_PUBKEY)(a_bp,a_x,a_cb,a_u);
#endif
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
#ifdef __APPLE__

    if(s_System_sslwrap_EVP_PKEY_assign){
        return (*s_System_sslwrap_EVP_PKEY_assign)(a_pkey,a_type,a_key);
    }

    return System_sslwrap_EVP_PKEY_assignInline(a_pkey,a_type,a_key);

#else
    return (*s_System_sslwrap_EVP_PKEY_assign)(a_pkey,a_type,a_key);
#endif
}


SYSTEM_EXPORT EVP_MD_CTX* System_sslwrap_EVP_MD_CTX_new(void) CPPUTILS_NOEXCEPT
{
#ifdef __APPLE__

    if(s_System_sslwrap_EVP_MD_CTX_new){
        return (*s_System_sslwrap_EVP_MD_CTX_new)();
    }

    EVP_MD_CTX* const pRet = (EVP_MD_CTX*)calloc(1,sizeof(EVP_MD_CTX));
    return pRet;

#else
    return (*s_System_sslwrap_EVP_MD_CTX_new)();
#endif
}


SYSTEM_EXPORT void System_sslwrap_EVP_PKEY_free(EVP_PKEY* a_pkey) CPPUTILS_NOEXCEPT
{
    // System_sslwrap_EVP_MD_CTX_resetInline
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
#ifdef __APPLE__

    if(s_System_sslwrap_EVP_MD_CTX_free){
        (*s_System_sslwrap_EVP_MD_CTX_free)(a_ctx);
        return;
    }

    if(a_ctx){
        System_sslwrap_EVP_MD_CTX_resetInline(a_ctx);
        free(a_ctx);
    }

#else
    (*s_System_sslwrap_EVP_MD_CTX_free)(a_ctx);
#endif
}


SYSTEM_EXPORT int System_sslwrap_BIO_read(BIO* a_bio, void* a_data, int a_len) CPPUTILS_NOEXCEPT
{
    return (*s_System_sslwrap_BIO_read)(a_bio,a_data,a_len);
}


CPPUTILS_END_C


#endif  //  #ifdef SYSTEM_SSL_WRAPPER_EXISTS
