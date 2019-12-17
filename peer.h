#ifndef PEER_H
#define PEER_H

#include <QString>

class Peer
{
public:
    Peer(QString peerIpfs, QString account = "root", QString nickname = "");
    Peer(const Peer&);

    QString getPeerIpfs() const;
    QString getAccount() const;
    QString getNickname() const;
    QString getFriendlyName() const;
    QString fullAddress() const;

    void setNickname(QString nickname);

private:
    QString peerIpfs;
    QString account;
    QString nickname;
};

#endif // PEER_H
