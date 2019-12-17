#ifndef AUTHPARAMS_H
#define AUTHPARAMS_H

#include <QString>

class AuthParams
{
public:
    AuthParams(QString homeserver, QString peerId, QString account, QString authToken);

    QString getHomeserver() const;
    QString getPeerId() const;
    QString getAccount() const;
    QString getAuthToken() const;

    void setHomeserver(QString homeserver);
    void setPeerId(QString peerId);
    void setAccount(QString account);
    void setAuthToken(QString authToken);

private:
    QString homeserver;
    QString peerId;
    QString account;
    QString authToken;
};

#endif // AUTHPARAMS_H
