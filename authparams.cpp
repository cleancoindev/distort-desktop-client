#include "authparams.h"

AuthParams::AuthParams(std::string h, std::string p, std::string acc, std::string auth) :
    homeserver(h), peerId(p), account(acc), authToken(auth)
{
    // Ensure homeserver URL's end with '/' for consistency in request-path appending
    if(homeserver.size() > 0 && homeserver.back() != '/') {
        homeserver += '/';
    }
}

std::string AuthParams::getHomeserver() const
{
    return homeserver;
}

std::string AuthParams::getPeerId() const
{
    return peerId;
}

std::string AuthParams::getAccount() const
{
    return account;
}

std::string AuthParams::getAuthToken() const
{
    return authToken;
}

void AuthParams::setHomeserver(std::string h)
{
    homeserver = h;
}

void AuthParams::setPeerId(std::string p)
{
    peerId = p;
}

void AuthParams::setAccount(std::string a)
{
    account = a;
}

void AuthParams::setAuthToken(std::string a)
{
    authToken = a;
}
