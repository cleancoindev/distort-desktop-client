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
    void failure(QString e);

public slots:
    void doWork(QString homeserver, QString account, QString password);
};

#endif // SIGNINWORKER_H
