#include "conversation.h"

Conversation::Conversation(uint64_t s, std::shared_ptr<Peer> p) :
    size(s), peer(p)
{
}

uint64_t Conversation::getSize() const
{
    return size;
}

std::shared_ptr<Peer> Conversation::getPeer() const
{
    return peer;
}
