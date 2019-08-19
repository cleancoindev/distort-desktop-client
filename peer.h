#ifndef PEER_H
#define PEER_H

#include <QString>

class Peer
{
public:
    Peer(QString peerIpfs, QString account = "root", QString friendlyName = "");

    QString getPeerIpfs() const;
    QString getAccount() const;
    QString getFriendlyName() const;

private:
    QString peerIpfs;
    QString account;
    QString friendlyName;
};

#endif // PEER_H
