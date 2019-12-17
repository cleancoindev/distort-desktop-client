#include "atomicstate.h"

#include <iostream>

extern "C"
{
    #include "yasl/yasl_state.h"
    #include "json.yasl/json.h"
}

static std::mutex mtx;
static AtomicState* atomicState;

AtomicState::AtomicState()
{
    // Initialize an empty environment that supports JSON extension
    state = YASL_newstate("scripts/init.yasl");
    if(!state) {
        std::cerr << "scripts/init.yasl: can't find it" << std::endl;
        exit(-10);
    }
    YASL_load_json(state);

    if(int status = YASL_execute(state) != YASL_MODULE_SUCCESS)
    {
        std::cerr << "scripts/init.yasl: module failed" << std::endl;
        std::cerr << "YASL state is required, aborting" << std::endl;
        exit(-1);
    }

    YASL_Object* r = YASL_popobject(state);
    if(!YASL_getboolean(r))
    {
        std::cerr << "scripts/init.yasl: returned failure" << std::endl;
        std::cerr << "YASL state is required, aborting" << std::endl;
        exit(-1);
    }
}

AtomicState::~AtomicState()
{
    YASL_delstate(state);
}

AtomicState* AtomicState::getInstance()
{
    mtx.lock();
    if(!atomicState)
    {
        atomicState = new AtomicState;
    }

    mtx.unlock();
    return atomicState;
}

void AtomicState::lockState()
{
    mtx.lock();
}

void AtomicState::unlockState()
{
    mtx.unlock();
}

YASL_State* AtomicState::getState()
{
    return state;
}

