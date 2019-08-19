#include "signindialog.h"
#include "ui_signindialog.h"
#include "restclient.h"
#include "distortexception.h"

#include <curlpp/cURLpp.hpp>
#include <cryptopp/pwdbased.h>
#include <cryptopp/sha.h>
#include <cryptopp/base64.h>

SignInDialog::SignInDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::SignInDialog), acc(nullptr), authParams(nullptr)

{
    ui->setupUi(this);
    setWindowTitle("Sign In");
}

SignInDialog::~SignInDialog()
{
    delete ui;
}

std::shared_ptr<AuthParams> SignInDialog::getAuthParams() const
{
    return authParams;
}

void SignInDialog::accept()
{
    std::string homeserver = ui->homeserverEdit->text().toStdString();
    /*if(homeserver.empty())
    {
        return ui->errorLabel->setText("Must specify a homeserver network address");
    }*/
    homeserver = homeserver.empty() ? "http://localhost:6945" : homeserver;
    std::string account = ui->accountEdit->text().toStdString();
    account = account.empty() ? "root" : account;
    std::string password = ui->passwordEdit->text().toStdString();
    if(password.empty())
    {
        return ui->errorLabel->setText("Password field cannot be empty");
    }

    try
    {
        // Ensure homeserver URL's end with '/' for consistency in request-path appending
        if(homeserver.back() != '/') {
            homeserver += '/';
        }

        std::string peerId = "QmemJHsMDuBjUCAyiWeVdct2LGHqtZhu8QQCt8ZQVbY1qz";

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
        authParams = std::make_shared<AuthParams>(AuthParams(homeserver, peerId, account, authToken));
        RestClient::sendDistortUnauthGet(homeserver + "ipfs", RestClient::emptyParams);

        RestClient::sendDistortAuthGet("account", *authParams, RestClient::emptyParams);


        // Update password
        /*std::map<std::string, std::string> newSettings;
        newSettings.insert(std::pair<std::string, std::string>("authToken", "+I1CLkFxN/UhArBYMN8iVG4NdHMLt/oqig5Rzx+kiVM="));
        RestClient::sendDistortAuthRequest("PUT", "account", *authParams, RestClient::emptyParams, newSettings);*/
    }
    catch(DistortException e)
    {
        std::cerr << "ERROR: " << e.what() << std::endl;

        // TODO: Handle response codes with specific error messages
        switch(e.getResponseCode())
        {
        case 500:
            return ui->errorLabel->setText("An internal server error occurred");
        default:
            return ui->errorLabel->setText(e.what());
        }
    }
    catch(curlpp::RuntimeError& e)
    {
        return ui->errorLabel->setText(e.what());
    }

    done(Accepted);
}
