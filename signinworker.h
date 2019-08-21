#ifndef SIGNINWORKER_H
#define SIGNINWORKER_H

#include "authparams.h"
#include "distortexception.h"

#include <QObject>

class SignInWorker : public QObject
{
    Q_OBJECT
public:
    explicit SignInWorker(QObject *parent = nullptr);

signals:
    void success(AuthParams* authParams);
    void failure(std::string e);

public slots:
    void doWork(std::string homeserver, std::string account, std::string password);
};

#endif // SIGNINWORKER_H
