#include "authparams.h"

AuthParams::AuthParams(QString h, QString p, QString acc, QString auth) :
    homeserver(h), peerId(p), account(acc), authToken(auth)
{
    // Ensure homeserver URL's end with '/' for consistency in request-path appending
    if(homeserver.size() > 0 && homeserver.back() != '/') {
        homeserver += '/';
    }
}

QString AuthParams::getHomeserver() const
{
    return homeserver;
}

QString AuthParams::getPeerId() const
{
    return peerId;
}

QString AuthParams::getAccount() const
{
    return account;
}

QString AuthParams::getAuthToken() const
{
    return authToken;
}

void AuthParams::setHomeserver(QString h)
{
    homeserver = h;
}

void AuthParams::setPeerId(QString p)
{
    peerId = p;
}

void AuthParams::setAccount(QString a)
{
    account = a;
}

void AuthParams::setAuthToken(QString a)
{
    authToken = a;
}
