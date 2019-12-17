#ifndef CONVERSATION_H
#define CONVERSATION_H

#include "peer.h"

#include <memory>
#include <string>

class Conversation
{
public:
    Conversation(const QString& groupName, const Peer& peer, uint64_t height);
    Conversation(const Conversation&);

    const Peer& getPeer() const;
    uint64_t getHeight() const;
    QString getGroupName() const;
    QString uniqueLabel() const;

    void setName(QString name);

private:
    uint64_t height;
    Peer peer;
    QString groupName;
};

#endif // CONVERSATION_H
