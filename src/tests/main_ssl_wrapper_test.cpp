//
// file:			main_any_quick_test.c
// path:			src/tests/main_any_quick_test.c
// created on:		2023 May 08
// created by:		Davit Kalantaryan (davit.kalantaryan@gmail.com)
//


#include <system/ssl_wrapper.h>
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
#include <QByteArray>
#include <QFile>
#include <QEventLoop>
#include <cinternal/undisable_compiler_warnings.h>


#define SYSTEM_TEST_CAPACITY_FILE   "../../../../.tests/capacity.json"
#define SYSTEM_TEST_SIGNATURE_FILE  "../../../../.tests/capacity_signature.bin"

static bool VerifySignature(const QString& a_dataFile, const QString& a_signatureFile);
static int ForceLoadSslLibraries(void);


int main(int a_argc, char* a_argv[])
{
    QCoreApplication aApp(a_argc, a_argv);

    const char* cpcDataFile = SYSTEM_TEST_CAPACITY_FILE;
    const char* cpcSignatureFile = SYSTEM_TEST_SIGNATURE_FILE;

    if(a_argc>1){
        cpcDataFile = a_argv[1];
    }

    if(a_argc>2){
        cpcSignatureFile = a_argv[2];
    }

    qDebug()<<"cpcDataFile:"<<cpcDataFile<<", cpcSignatureFile:"<<cpcSignatureFile;

    if(ForceLoadSslLibraries()){
        return 1;
    }

    const bool bRet = VerifySignature(cpcDataFile,cpcSignatureFile);
    if(bRet){
        qDebug()<<"signature verified";
    }

    //QNetworkAccessManager manager;

    //// URL to fetch data from
    //QUrl url("https://jsonplaceholder.typicode.com/posts/1"); // Example URL
    //QNetworkRequest request(url);

    //// Send GET request
    //QNetworkReply *reply = manager.get(request);

    //// Connect the reply finished signal to handleNetworkReply
    //QObject::connect(reply, &QNetworkReply::finished, &aApp, [&reply]() {
    //    if (reply->error() == QNetworkReply::NoError) {
    //        // Print the data received from the URL
    //        //qDebug() << "Response:" << reply->readAll();
    //    } else {
    //        // Print the error if any
    //        qDebug() << "Error:" << reply->errorString();
    //    }
    //    reply->deleteLater(); // Clean up

    //    //void* pFuncAddress = SystemFindSymbolAddress(SYMBOL_NAME_TO_FIND);
    //    //qDebug()<<"4. pFuncAddress: "<<pFuncAddress;
    //    CallSslFunctions();
    //});

    //aApp.exec();


	return 0;
}


static bool VerifySignature(const QString& a_dataFile, const QString& a_signatureFile)
{
    static const char* sccpFocustCapPublicKey =
        "-----BEGIN PUBLIC KEY-----\r\n"
        "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA7kPhpKxqKYtlYCbJJ+QU\r\n"
        "+qtLeFmTHAkLhb5s3yZ1cHzy5viK7GcJ+y5Grbu6CW2VI7EvrUzrnEzlyAs5GRrE\r\n"
        "qSMS72V6zH871l6lUkWdKi8E1rACfEJN6k4pV6pzZQ8oAcRvNYEM43lmwQen6ySo\r\n"
        "EQotmLaKaQJWY46g6ktOPMZjzAF7q5UPWk/6pv1B3Xy+MtQ2bDjiLXvqbZrO+GdE\r\n"
        "upZE8PU1+H9/Q52jeHtmJWDGf/xGmVfykoVwACnj7v5rgJR6K6CRFshba12Yuw2a\r\n"
        "2CnRzCJO3t3kauwLEaCIw6aFwj3ilQ4e7919xFhmo2lib1Wbhu+ej6ZSKRT8ibHO\r\n"
        "rwIDAQAB\r\n"
        "-----END PUBLIC KEY-----";

    bool bRet = false;
    int nCallRet;
    const QByteArray publicKeyContent = QByteArray(sccpFocustCapPublicKey);

    QFile data(a_dataFile);
    if (!data.open(QIODevice::ReadOnly)) {
        qCritical()<<"Unable to open data file.";
        return false;
    }
    const QByteArray dataContent = data.readAll();
    data.close();

    // Load the signature
    QFile signature(a_signatureFile);
    if (!signature.open(QIODevice::ReadOnly)) {
        qCritical("Unable to open signature file.");
        return false;
    }
    const QByteArray signatureContent = signature.readAll();
    signature.close();

    nCallRet = System_sslwrap_InitializeFunctions();
    if(nCallRet){
        qCritical()<<"System_sslwrap_InitializeFunctions failed";
        return false;
    }

    nCallRet = System_sslwrap_OPENSSL_init_ssl(0,CPPUTILS_NULL);
    if(!nCallRet){
        qCritical()<<"System_sslwrap_OPENSSL_init_ssl failed";
        return false;
    }

    BIO* const bio = System_sslwrap_BIO_new_mem_buf(publicKeyContent.data(), (int)publicKeyContent.size());
    if (!bio) {
        System_sslwrap_OPENSSL_cleanup();
        qCritical("Failed to create BIO for public key.");
        return false;
    }

    RSA* const rsa = System_sslwrap_PEM_read_bio_RSA_PUBKEY(bio, nullptr, nullptr, nullptr);
    if (!rsa) {
        qCritical("Failed to load RSA public key.");
        System_sslwrap_BIO_free(bio);
        System_sslwrap_OPENSSL_cleanup();
        return false;
    }

    EVP_PKEY* const evp_key = System_sslwrap_EVP_PKEY_new();
    if (!evp_key) {
        qCritical("System_sslwrap_EVP_PKEY_new failed");
        System_sslwrap_BIO_free(bio);
        System_sslwrap_OPENSSL_cleanup();
        return false;
    }

    System_sslwrap_EVP_PKEY_assign_RSA(evp_key, rsa);

    EVP_MD_CTX* const mdctx = System_sslwrap_EVP_MD_CTX_new();
    if (!mdctx) {
        qCritical("System_sslwrap_EVP_MD_CTX_new failed");
        System_sslwrap_EVP_PKEY_free(evp_key);
        System_sslwrap_BIO_free(bio);
        System_sslwrap_OPENSSL_cleanup();
        return false;
    }

    if (System_sslwrap_EVP_DigestVerifyInit(mdctx, nullptr, System_sslwrap_EVP_sha256(), nullptr, evp_key) <= 0) {
        qCritical("EVP_DigestVerifyInit failed.");
    } else if (System_sslwrap_EVP_DigestVerifyUpdate(mdctx, dataContent.data(), dataContent.size()) <= 0) {
        qCritical("EVP_DigestVerifyUpdate failed.");
    } else {
        int rc = System_sslwrap_EVP_DigestVerifyFinal(mdctx, reinterpret_cast<const unsigned char*>(signatureContent.data()), signatureContent.size());
        if (rc == 1) {
            //returnJsonObj["has_error"] = 0; // Signature is valid
            bRet = true;
        } else if (rc == 0) {
        } else {
            qCritical("Error during EVP_DigestVerifyFinal.");
        }
    }

    // cleaning up
    System_sslwrap_EVP_MD_CTX_free(mdctx);
    System_sslwrap_EVP_PKEY_free(evp_key);
    System_sslwrap_BIO_free(bio);
    System_sslwrap_OPENSSL_cleanup();

    return bRet;
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
