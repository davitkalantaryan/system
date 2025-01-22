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


typedef const char* const ConstCharPtr;

static const ConstCharPtr  s_symbolsNames[] ={
    "OPENSSL_init_ssl",
    "OPENSSL_cleanup",
    "BIO_new_mem_buf",
    "PEM_read_bio_RSA_PUBKEY",
    "BIO_free",
    "EVP_PKEY_new",
    "EVP_PKEY_assign",
    "EVP_MD_CTX_new",
    "EVP_PKEY_free",
    "EVP_DigestVerifyInit",
    "EVP_DigestUpdate",
    "EVP_DigestVerifyFinal",
    "EVP_sha256",
    "EVP_MD_CTX_free"
};


static const size_t s_numberOfSymbols = sizeof(s_symbolsNames) / sizeof(const ConstCharPtr);

static int ForceLoadSslLibraries(void);


int main(int a_argc, char* a_argv[])
{
    size_t i;
    void* pFuncAddress;

    QCoreApplication aApp(a_argc, a_argv);

    if(ForceLoadSslLibraries()){
        return 1;
    }

    for(i=0;i<s_numberOfSymbols;++i){
        pFuncAddress= SystemFindSymbolAddress(s_symbolsNames[i]);
        qDebug()<<s_symbolsNames[i] << " => "<<pFuncAddress;
    }

    //aApp.exec();

	return 0;
}


static int ForceLoadSslLibraries(void)
{
    if (QSslSocket::supportsSsl()) {
        qDebug() << "SSL Library Version:" << QSslSocket::sslLibraryVersionString();
        qDebug() << "SSL Library Build Version:" << QSslSocket::sslLibraryBuildVersionString();

        QNetworkAccessManager manager;
        const QUrl url("https://google.com");
        QNetworkRequest request(url);
        QNetworkReply* const reply = manager.get(request);
        QEventLoop loopUntilDone;
        bool resultNotReady = true;
        QObject::connect(reply, &QNetworkReply::finished, &manager, [&reply,&loopUntilDone,&resultNotReady]() {
            resultNotReady = false;
            loopUntilDone.quit();
            reply->deleteLater(); // Clean up
        });
        if(resultNotReady){
            loopUntilDone.exec();
        }

        return 0;
    }



    return 1;
}
