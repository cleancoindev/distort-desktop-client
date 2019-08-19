#ifndef CONVERSATION_H
#define CONVERSATION_H

#include "peer.h"

#include <memory>

class Conversation
{
public:
    Conversation(uint64_t size, std::shared_ptr<Peer> peer);

    uint64_t getSize() const;
    std::shared_ptr<Peer> getPeer() const;

private:
    uint64_t size;
    std::shared_ptr<Peer> peer;
};

#endif // CONVERSATION_H
