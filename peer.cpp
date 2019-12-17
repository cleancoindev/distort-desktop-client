#include "peer.h"

Peer::Peer(QString p, QString a, QString n) :
    peerIpfs(p), account(a), nickname(n)
{}
Peer::Peer(const Peer& p) :
    peerIpfs(p.peerIpfs), account(p.account), nickname(p.nickname)
{
    if(account.isEmpty())
    {
        account = "root";
    }
}

QString Peer::getPeerIpfs() const
{
    return peerIpfs;
}

QString Peer::getAccount() const
{
    return account;
}

QString Peer::getNickname() const
{
    return nickname;
}

QString Peer::getFriendlyName() const
{
    if(!nickname.isEmpty())
    {
        return nickname;
    }

    if(account.isEmpty() || account == "root")
    {
        return peerIpfs;
    }

    return account;
}

void Peer::setNickname(QString n)
{
    nickname = n;
}

QString Peer::fullAddress() const
{
    return peerIpfs + ":" + account;
}
