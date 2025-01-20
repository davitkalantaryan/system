//
// file:			main_any_quick_test.c
// path:			src/tests/main_any_quick_test.c
// created on:		2023 May 08
// created by:		Davit Kalantaryan (davit.kalantaryan@gmail.com)
//


#include <system/find_symbol_address.h>
#include <cinternal/disable_compiler_warnings.h>
#include <stdio.h>
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDebug>
#include <QTimer>
#include <QSslSocket>
#include <cinternal/undisable_compiler_warnings.h>


// #define SYMBOL_NAME_TO_FIND     "OPENSSL_init_ssl" // works (for OpenSSL_add_all_algorithms) if version is >= 0x10100000L
// #define SYMBOL_NAME_TO_FIND     "SSL_library_init"
#define SYMBOL_NAME_TO_FIND     "OPENSSL_init_crypto" // works (for ERR_load_crypto_strings)
// #define SYMBOL_NAME_TO_FIND     "BIO_new_mem_buf" // works
// #define SYMBOL_NAME_TO_FIND     "PEM_read_bio_PrivateKey" // works
// #define SYMBOL_NAME_TO_FIND     "EVP_PKEY_new"  // works
// #define SYMBOL_NAME_TO_FIND     "EVP_PKEY_assign" // works (for EVP_PKEY_assign_RSA)
// #define SYMBOL_NAME_TO_FIND     "EVP_MD_CTX_new"  // works
// #define SYMBOL_NAME_TO_FIND     "EVP_PKEY_free"  // works
// #define SYMBOL_NAME_TO_FIND     "BIO_free" // works
// #define SYMBOL_NAME_TO_FIND     "EVP_DigestVerifyInit" // works
// #define SYMBOL_NAME_TO_FIND     "EVP_DigestVerifyUpdate" // works
// #define SYMBOL_NAME_TO_FIND     "EVP_DigestVerifyFinal" // works
// #define SYMBOL_NAME_TO_FIND     "EVP_MD_CTX_free" // works
// #define SYMBOL_NAME_TO_FIND     "EVP_PKEY_free" // works
// #define SYMBOL_NAME_TO_FIND     "BIO_free" // works
// #define SYMBOL_NAME_TO_FIND     "ERR_free_strings" // this is just a macro

int main(int a_argc, char* a_argv[])
{
    void* pFuncAddress;

    pFuncAddress = SystemFindSymbolAddress(SYMBOL_NAME_TO_FIND);
    qDebug()<<"1. pFuncAddress: "<<pFuncAddress;

    QCoreApplication aApp(a_argc, a_argv);

    if (!QSslSocket::supportsSsl()) {
        qDebug() << "OpenSSL is not available!";
    } else {
        qDebug() << "OpenSSL is available.";
        // Optionally print available ciphers to verify initialization
        qDebug() << "Supported ciphers:" ;
    }

    pFuncAddress = SystemFindSymbolAddress(SYMBOL_NAME_TO_FIND);
    qDebug()<<"2. pFuncAddress: "<<pFuncAddress;

    QNetworkAccessManager manager;

    pFuncAddress = SystemFindSymbolAddress(SYMBOL_NAME_TO_FIND);
    qDebug()<<"3. pFuncAddress: "<<pFuncAddress;

    // URL to fetch data from
    QUrl url("https://jsonplaceholder.typicode.com/posts/1"); // Example URL
    QNetworkRequest request(url);

    // Send GET request
    QNetworkReply *reply = manager.get(request);

    // Connect the reply finished signal to handleNetworkReply
    QObject::connect(reply, &QNetworkReply::finished, &aApp, [&reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            // Print the data received from the URL
            //qDebug() << "Response:" << reply->readAll();
        } else {
            // Print the error if any
            qDebug() << "Error:" << reply->errorString();
        }
        reply->deleteLater(); // Clean up

        void* pFuncAddress = SystemFindSymbolAddress(SYMBOL_NAME_TO_FIND);
        qDebug()<<"4. pFuncAddress: "<<pFuncAddress;
    });

    qDebug()<<"5. pFuncAddress: "<<pFuncAddress;


    aApp.exec();


	return 0;
}
