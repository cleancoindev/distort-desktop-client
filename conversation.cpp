#include "conversation.h"

#include <sstream>

Conversation::Conversation(const QString& g, const Peer& p, uint64_t s) :
    height(s), peer(p), groupName(g)
{}

Conversation::Conversation(const Conversation& c) :
    height(c.height), peer(c.peer), groupName(c.groupName)
{}

uint64_t Conversation::getHeight() const
{
    return height;
}

const Peer& Conversation::getPeer() const
{
    return peer;
}

QString Conversation::getGroupName() const
{
    return groupName;
}

QString Conversation::uniqueLabel() const
{
    return groupName + ":" + peer.fullAddress();
}

void Conversation::setName(QString name)
{
    peer.setNickname(name);
}
