#include "signinworker.h"
#include "restclient.h"

#include <cryptopp/pwdbased.h>
#include <cryptopp/sha.h>
#include <cryptopp/base64.h>

extern "C"
{
    #include "json.yasl/json.h"
}


namespace
{
    // TODO: DELETE THIS
    void patch_pushcstring(YASL_State* S, const char* cstr)
    {
        size_t len = strlen(cstr);
        char* c = (char*)malloc(len + 1);
        memcpy(c, cstr, len);
        c[len] = 0;

        YASL_pushszstring(S, c);
    }
}

SignInWorker::SignInWorker(QObject *parent) : QObject(parent)
{}

void SignInWorker::doWork(QString homeserver, QString account, QString password)
{
    try
    {
        // Get IPFS peer ID
        QString response = RestClient::sendDistortUnauthGet((homeserver + "ipfs").toStdString(), RestClient::emptyParams);
        QString peerId = RestClient::getMessage(response);

        // PBKDF2
        CryptoPP::PKCS5_PBKDF2_HMAC<CryptoPP::SHA256> sha256;
        std::shared_ptr<uint8_t[]> buffer(new uint8_t[32]);
        sha256.DeriveKey(buffer.get(), 32, 0, (const uint8_t*)password.toUtf8().data(), password.size(), (const uint8_t*)peerId.toUtf8().data(), 46, 1000);
        CryptoPP::Base64Encoder encoder;
        encoder.Put(buffer.get(), 32);
        encoder.MessageEnd();
        uint64_t size = encoder.MaxRetrievable();
        std::shared_ptr<char[]> encodedAuth(new char[size]);
        encoder.Get((uint8_t*)encodedAuth.get(), size);
        QString authToken = QString::fromUtf8(encodedAuth.get(), size);

        AuthParams* authParams = new AuthParams(homeserver, peerId, account, authToken);
        RestClient::sendDistortAuthGet("account", *authParams, RestClient::emptyParams);

        emit success(authParams);
    }
    catch(DistortException e)
    {
        QString error = RestClient::getError(e.what());

        // TODO: Handle response codes with specific error messages
        switch(e.getResponseCode())
        {
        case 500:
            emit failure("An internal server error occurred");
        default:
            emit failure(error);
        }
    }
    catch(cURLpp::RuntimeError e)
    {
        emit failure(e.what());
    }
}
