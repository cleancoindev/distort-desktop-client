#include "signinworker.h"
#include "restclient.h"

#include <cryptopp/pwdbased.h>
#include <cryptopp/sha.h>
#include <cryptopp/base64.h>

SignInWorker::SignInWorker(QObject *parent) : QObject(parent)
{}

void SignInWorker::doWork(std::string homeserver, std::string account, std::string password)
{
    try
    {
        // Get IPFS peer ID
        RestClient::sendDistortUnauthGet(homeserver + "ipfs", RestClient::emptyParams);
        std::string peerId = "QmPP628pbAbc77BExibxu5rFoeGJBZnX28jwxb5ruQpvNd";

        // PBKDF2
        CryptoPP::PKCS5_PBKDF2_HMAC<CryptoPP::SHA256> sha256;
        std::shared_ptr<uint8_t> buffer(new uint8_t[32]);
        sha256.DeriveKey(buffer.get(), 32, 0, (const uint8_t*)password.c_str(), password.size(), (const uint8_t*)peerId.c_str(), 46, 1000);
        CryptoPP::Base64Encoder encoder;
        encoder.Put(buffer.get(), 32);
        encoder.MessageEnd();
        uint64_t size = encoder.MaxRetrievable();
        std::shared_ptr<char> encodedAuth(new char[size]);
        encoder.Get((uint8_t*)encodedAuth.get(), size);
        std::string authToken(encodedAuth.get(), size);

        AuthParams* authParams = new AuthParams(homeserver, peerId, account, authToken);
        RestClient::sendDistortAuthGet("account", *authParams, RestClient::emptyParams);

        emit success(authParams);
    }
    catch(DistortException e)
    {
        std::cerr << "ERROR: " << e.what() << std::endl;

        // TODO: Handle response codes with specific error messages
        switch(e.getResponseCode())
        {
        case 500:
            emit failure("An internal server error occurred");
        default:
            emit failure(e.what());
        }
    }
    catch(cURLpp::RuntimeError e)
    {
        emit failure(e.what());
    }
}
