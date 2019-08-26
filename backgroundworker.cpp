#include "backgroundworker.h"
#include "authparams.h"

#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

BackgroundWorker::BackgroundWorker(QObject *parent) : QObject(parent) {}

void BackgroundWorker::doWork(std::string homeserver, std::string peerId, std::string account, std::string authToken)
{
    AuthParams authParams(homeserver, peerId, account, authToken);

    while(true)
    {

        std::this_thread::sleep_for(std::chrono::minutes(1));
    }
}
