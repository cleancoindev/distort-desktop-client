#ifndef BACKGROUNDWORKER_H
#define BACKGROUNDWORKER_H

#include <QObject>

class BackgroundWorker : public QObject
{
    Q_OBJECT
public:
    explicit BackgroundWorker(QObject *parent = nullptr);

signals:

public slots:
    void doWork(std::string homeserver, std::string peerId, std::string account, std::string authToken);
};

#endif // BACKGROUNDWORKER_H
