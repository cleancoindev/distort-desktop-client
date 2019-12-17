#ifndef OUTMESSAGE_H
#define OUTMESSAGE_H

#include "message.h"

class OutMessage : public Message
{
public:
    OutMessage(QString status, QString message, uint64_t index, QString lastStatusChange);
    ~OutMessage();

    const QString& getType() const;
    QString getStatus() const;
    QString getLastStatusChange() const;

private:
    QString status;
    QString lastStatusChange;
};

#endif // OUTMESSAGE_H
