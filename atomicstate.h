#ifndef ATOMICSTATE_H
#define ATOMICSTATE_H

#include <string>
#include <mutex>
#include <memory>
#include <map>

extern "C"
{
    #include "yasl/yasl.h"
}

class AtomicState
{
public:
    static std::shared_ptr<AtomicState> getInstance();
    static void lockState();
    static void unlockState();

    const std::shared_ptr<YASL_Object> getAccount() const;
    const std::shared_ptr<YASL_Object> getGroups() const;
    const std::shared_ptr<YASL_Object> getConversations(std::string groupName) const;
    const std::shared_ptr<YASL_Object> getMessages(std::string conversationLabel) const;

private:
    AtomicState();

    const std::shared_ptr<YASL_Object> account;
    const std::shared_ptr<YASL_Object> groups;
    const std::map<std::string, std::shared_ptr<YASL_Object>> conversations;
    const std::map<std::string, std::shared_ptr<YASL_Object>> messages;
};

#endif // ATOMICSTATE_H
