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
    static AtomicState* getInstance();
    ~AtomicState();

    void lockState();
    void unlockState();

    YASL_State* getState();

private:
    YASL_State* state;

    AtomicState();
};

#endif // ATOMICSTATE_H
