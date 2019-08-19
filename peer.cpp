#include "peer.h"

Peer::Peer(QString p, QString a, QString f) :
    peerIpfs(p), account(a), friendlyName(f)
{
}

QString Peer::getPeerIpfs() const
{
    return peerIpfs;
}

QString Peer::getAccount() const
{
    return account;
}

QString Peer::getFriendlyName() const
{
    if(!friendlyName.isEmpty())
    {
        return friendlyName;
    }

    if(account.isEmpty() || account == "root")
    {
        return peerIpfs;
    }

    return account;
}
