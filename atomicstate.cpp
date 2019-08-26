#include "atomicstate.h"

std::mutex mtx;
std::shared_ptr<AtomicState> atomicState(nullptr);
AtomicState::AtomicState() {}

std::shared_ptr<AtomicState> AtomicState::getInstance()
{
    mtx.lock();
    if(atomicState != nullptr)
    {
        mtx.unlock();
        return atomicState;
    }
    else
    {
        atomicState = std::make_shared<AtomicState>(AtomicState());
        mtx.unlock();
        return atomicState;
    }
}

void AtomicState::lockState()
{
    mtx.lock();
}

void AtomicState::unlockState()
{
    mtx.unlock();
}

const std::shared_ptr<YASL_Object> AtomicState::getAccount() const
{
    return account;
}
const std::shared_ptr<YASL_Object> AtomicState::getGroups() const
{
    return groups;
}
const std::shared_ptr<YASL_Object> AtomicState::getConversations(std::string groupName) const
{
    return conversations.at(groupName);
}
const std::shared_ptr<YASL_Object> AtomicState::getMessages(std::string conversationLabel) const
{
    return messages.at(conversationLabel);
}
