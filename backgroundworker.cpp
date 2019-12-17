#include "backgroundworker.h"
#include "authparams.h"
#include "restclient.h"
#include "distortexception.h"
#include "atomicstate.h"
#include "groupwidget.h"
#include "conversationwidget.h"
#include "inmessage.h"
#include "outmessage.h"

#include <curlpp/cURLpp.hpp>
#include <QThread>
#include <QTreeWidgetItem>

extern "C"
{
    #include "yasl/yasl.h"
    #include "yasl/yasl_state.h"
}

typedef QTreeWidgetItem GroupItem;
typedef QTreeWidgetItem ConvItem;


// TODO: DELETE THIS
namespace
{
    void patch_pushcstring(YASL_State* S, const char* cstr)
    {
        size_t len = strlen(cstr);
        char* c = (char*)malloc(len + 1);
        memcpy(c, cstr, len);
        c[len] = 0;

        YASL_pushszstring(S, c);
    }


    // WRAPPER C++ FUNCTIONS FOR YASL
    // ==============================

    int escape(YASL_State* S)
    {
        // string toEscape
        YASL_Object* str = YASL_popobject(S);
        std::string requestPath = std::string(YASL_getstring(str), YASL_getstringlen(str));

        std::string esc = cURLpp::escape(requestPath);

        patch_pushcstring(S, esc.c_str());
        return YASL_SUCCESS;
    }

    int send(YASL_State* S)
    {
        // string method, string requestPath, string bodyParams, AuthParams authParams
        YASL_Object* aP = YASL_popobject(S);
        AuthParams* authParams = reinterpret_cast<AuthParams*>(YASL_getuserpointer(aP));

        YASL_Object* bP = YASL_popobject(S);
        std::string bodyParams(YASL_getstring(bP), YASL_getstringlen(bP));

        YASL_Object* rP = YASL_popobject(S);
        std::string requestPath(YASL_getstring(rP), YASL_getstringlen(rP));

        YASL_Object* m = YASL_popobject(S);
        std::string method(YASL_getstring(m), YASL_getstringlen(m));

        // TODO: Push response code when available
        try
        {
            QString res = RestClient::send(method, requestPath, bodyParams, authParams);
            patch_pushcstring(S, res.toUtf8());
            // YASL_pushinteger(S, 200);
        }
        catch(DistortException& e)
        {
            std::cerr << "ERROR " << e.getResponseCode() << ": " << e.what() << std::endl;
            YASL_pushundef(S);
            // YASL_pushinteger(S, e.getResponseCode());
        }
        catch(cURLpp::RuntimeError& e)
        {
            std::cerr << "ERROR: " << e.what() << std::endl;
            YASL_pushundef(S);
        }
        return YASL_SUCCESS;
    }

    int setActiveGroup(YASL_State* S)
    {
        // GroupItem g, bool isActive, worker
        YASL_Object* w = YASL_popobject(S);
        BackgroundWorker* worker = reinterpret_cast<BackgroundWorker*>(YASL_getuserpointer(w));

        YASL_Object* a = YASL_popobject(S);
        bool isActive = YASL_getboolean(a);

        YASL_Object* g = YASL_popobject(S);
        GroupItem* group = reinterpret_cast<GroupItem*>(YASL_getuserpointer(g));
        emit worker->setActiveGroupUI(group, isActive);

        YASL_pushundef(S);
        return YASL_SUCCESS;
    }

    int setSubgroupIndex(YASL_State* S)
    {
        // Group g, int index, worker
        YASL_Object* w = YASL_popobject(S);
        BackgroundWorker* worker = reinterpret_cast<BackgroundWorker*>(YASL_getuserpointer(w));

        YASL_Object* i = YASL_popobject(S);
        unsigned int index = static_cast<unsigned int>(YASL_getinteger(i));

        YASL_Object* g = YASL_popobject(S);
        GroupItem* group = reinterpret_cast<GroupItem*>(YASL_getuserpointer(g));
        emit worker->setSubgroupIndexUI(group, index);

        YASL_pushundef(S);
        return YASL_SUCCESS;
    }

    int setConversationName(YASL_State* S)
    {
        // ConvItem, string nickname, worker
        YASL_Object* w = YASL_popobject(S);
        BackgroundWorker* worker = reinterpret_cast<BackgroundWorker*>(YASL_getuserpointer(w));

        YASL_Object* n = YASL_popobject(S);
        QString nickname = QString::fromUtf8(YASL_getstring(n), YASL_getstringlen(n));

        YASL_Object* c = YASL_popobject(S);
        ConvItem* conv = reinterpret_cast<ConvItem*>(YASL_getuserpointer(c));
        emit worker->setConversationNameUI(conv, nickname);

        return YASL_SUCCESS;
    }

    int addGroup(YASL_State* S)
    {
        // string name, int index, worker
        YASL_Object* w = YASL_popobject(S);
        BackgroundWorker* worker = reinterpret_cast<BackgroundWorker*>(YASL_getuserpointer(w));

        YASL_Object* i = YASL_popobject(S);
        unsigned int index = static_cast<unsigned int>(YASL_getinteger(i));

        YASL_Object* n = YASL_popobject(S);
        QString name = QString::fromUtf8(YASL_getstring(n), YASL_getstringlen(n));

        QTreeWidgetItem* item = new QTreeWidgetItem;
        emit worker->addGroupUI(name, index, item);

        YASL_pushuserpointer(S, item);
        return YASL_SUCCESS;
    }

    int addConversation(YASL_State* S)
    {
        // Groupitem g, string peerId, string account, string nickname, int height, worker
        YASL_Object* w = YASL_popobject(S);
        BackgroundWorker* worker = reinterpret_cast<BackgroundWorker*>(YASL_getuserpointer(w));

        YASL_Object* h = YASL_popobject(S);
        unsigned int height = static_cast<unsigned int>(YASL_getinteger(h));

        YASL_Object* n = YASL_popobject(S);
        QString nickname;
        if(YASL_isstring(n) == YASL_SUCCESS && YASL_getstringlen(n) > 0)
        {
            nickname = QString::fromUtf8(YASL_getstring(n), YASL_getstringlen(n));
        }

        YASL_Object* a = YASL_popobject(S);
        QString account = "root";
        if(YASL_isstring(a) && YASL_getstringlen(a) > 0)
        {
            account = QString::fromUtf8(YASL_getstring(a), YASL_getstringlen(a));
        }

        YASL_Object* p = YASL_popobject(S);
        QString peerId = QString::fromUtf8(YASL_getstring(p), YASL_getstringlen(p));

        YASL_Object* g = YASL_popobject(S);
        GroupItem* groupItem = reinterpret_cast<GroupItem*>(YASL_getuserpointer(g));
        QTreeWidgetItem* item = new QTreeWidgetItem();
        emit worker->addConvUI(groupItem, peerId, account, nickname, height, item);

        YASL_pushuserpointer(S, item);
        return YASL_SUCCESS;
    }

    int removeGroup(YASL_State *S)
    {
        // Group g, worker
        YASL_Object* w = YASL_popobject(S);
        BackgroundWorker* worker = reinterpret_cast<BackgroundWorker*>(YASL_getuserpointer(w));

        YASL_Object* g = YASL_popobject(S);
        GroupItem* item = reinterpret_cast<GroupItem*>(YASL_getuserpointer(g));
        emit worker->removeGroupUI(item);

        YASL_pushundef(S);
        return YASL_SUCCESS;
    }

    int removeConversation(YASL_State *S)
    {
        // Conversation c, worker
        YASL_Object* w = YASL_popobject(S);
        BackgroundWorker* worker = reinterpret_cast<BackgroundWorker*>(YASL_getuserpointer(w));

        YASL_Object* c = YASL_popobject(S);
        ConvItem* item = reinterpret_cast<ConvItem*>(YASL_getuserpointer(c));
        emit worker->removeConvUI(item);

        YASL_pushundef(S);
        return YASL_SUCCESS;
    }

    int addOrUpdateInMessage(YASL_State *S)
    {
        // From, verified, index, message, dateReceived, worker
        YASL_Object* w = YASL_popobject(S);
        BackgroundWorker* worker = reinterpret_cast<BackgroundWorker*>(YASL_getuserpointer(w));

        YASL_Object* d = YASL_popobject(S);
        QString dateReceived = QString::fromUtf8(YASL_getstring(d), YASL_getstringlen(d));

        YASL_Object* m = YASL_popobject(S);
        QString message = QString::fromUtf8(YASL_getstring(m), YASL_getstringlen(m));

        YASL_Object* i = YASL_popobject(S);
        int64_t index = YASL_getinteger(i);

        YASL_Object* v = YASL_popobject(S);
        bool verified = YASL_getboolean(v);

        InMessage* im = new InMessage(verified, message, static_cast<uint64_t>(index), dateReceived);
        emit worker->addOrUpdateMessageUI(im);

        YASL_pushundef(S);
        return YASL_SUCCESS;
    }
    int addOrUpdateOutMessage(YASL_State *S)
    {
        // Status, index, message, lastStatusChange, worker
        YASL_Object* w = YASL_popobject(S);
        BackgroundWorker* worker = reinterpret_cast<BackgroundWorker*>(YASL_getuserpointer(w));

        YASL_Object* l = YASL_popobject(S);
        QString lastStatusChange = QString::fromUtf8(YASL_getstring(l), YASL_getstringlen(l));

        YASL_Object* m = YASL_popobject(S);
        QString message = QString::fromUtf8(YASL_getstring(m), YASL_getstringlen(m));

        YASL_Object* i = YASL_popobject(S);
        int64_t index = YASL_getinteger(i);

        YASL_Object* s = YASL_popobject(S);
        QString status = QString::fromUtf8(YASL_getstring(s), YASL_getstringlen(s));

        OutMessage* om = new OutMessage(status, message, index, lastStatusChange);
        emit worker->addOrUpdateMessageUI(om);

        YASL_pushundef(S);
        return YASL_SUCCESS;
    }
}


// Member functions
// ================

BackgroundWorker::BackgroundWorker(QObject *parent) : QObject(parent) {}

void BackgroundWorker::doWork(QString homeserver, QString peerId, QString account, QString authToken)
{
    AuthParams authParams(homeserver, peerId, account, authToken);
    AtomicState* atomicState = AtomicState::getInstance();

    atomicState->lockState();
    YASL_State* S = atomicState->getState();


    // PUSH VALUES
    // ------------

    // Pass auth params
    YASL_pushuserpointer(S, &authParams);
    YASL_setglobal(S, "$");

    // Pass homeserver
    YASL_declglobal(S, "homeserver");
    patch_pushcstring(S, homeserver.toUtf8());
    YASL_setglobal(S, "homeserver");

    // Pass self
    YASL_declglobal(S, "worker");
    YASL_pushuserpointer(S, this);
    YASL_setglobal(S, "worker");


    // PUSH FUNCTIONS
    // --------------

    // Pass escape function
    YASL_declglobal(S, "escape");
    YASL_pushcfunction(S, escape, 1);
    YASL_setglobal(S, "escape");

    // Pass send function
    YASL_declglobal(S, "send");
    YASL_pushcfunction(S, send, 4);
    YASL_setglobal(S, "send");

    // Pass setActiveGroup function
    YASL_declglobal(S, "setActiveGroup");
    YASL_pushcfunction(S, setActiveGroup, 3);
    YASL_setglobal(S, "setActiveGroup");

    // Pass setSubgroupIndex function
    YASL_declglobal(S, "setSubgroupIndex");
    YASL_pushcfunction(S, setSubgroupIndex, 3);
    YASL_setglobal(S, "setSubgroupIndex");

    // Pass setConversationName function
    YASL_declglobal(S, "setConversationName");
    YASL_pushcfunction(S, setConversationName, 3);
    YASL_setglobal(S, "setConversationName");

    // Pass addGroup function
    YASL_declglobal(S, "addGroup");
    YASL_pushcfunction(S, addGroup, 3);
    YASL_setglobal(S, "addGroup");

    // Pass addConversation function
    YASL_declglobal(S, "addConversation");
    YASL_pushcfunction(S, addConversation, 6);
    YASL_setglobal(S, "addConversation");

    // Pass removeGroup function
    YASL_declglobal(S, "removeGroup");
    YASL_pushcfunction(S, removeGroup, 2);
    YASL_setglobal(S, "removeGroup");

    // Pass removeConversation function
    YASL_declglobal(S, "removeConversation");
    YASL_pushcfunction(S, removeConversation, 2);
    YASL_setglobal(S, "removeConversation");

    // Pass addOrUpdateInMessage function
    YASL_declglobal(S, "addOrUpdateInMessageFromBackground");
    YASL_pushcfunction(S, addOrUpdateInMessage, 5);
    YASL_setglobal(S, "addOrUpdateInMessageFromBackground");

    // Pass addOrUpdateOutMessage function
    YASL_declglobal(S, "addOrUpdateOutMessageFromBackground");
    YASL_pushcfunction(S, addOrUpdateOutMessage, 5);
    YASL_setglobal(S, "addOrUpdateOutMessageFromBackground");

    while(!QThread::currentThread()->isInterruptionRequested())
    {
        YASL_resetstate(S, "scripts/background.yasl");
        if(!S)
        {
            std::cerr << "scripts/background.yasl: can't find it" << std::endl;
            atomicState->unlockState();
            return;
        }

        if(int status = YASL_execute(S) != YASL_MODULE_SUCCESS)
        {
            std::cerr << "scripts/background.yasl: module failed: " << status << std::endl;
        }
        else
        {
            YASL_Object* r = YASL_popobject(S);
            if(!YASL_getboolean(r))
            {
                std::cerr << "scripts/background.yasl: returned failure" << std::endl;
            }
        }

        if(QThread::currentThread()->isInterruptionRequested())
        {
            break;
        }

        atomicState->unlockState();
        QThread::sleep(5);
        atomicState->lockState();
    }
    atomicState->unlockState();
}
