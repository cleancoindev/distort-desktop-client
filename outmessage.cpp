#include "outmessage.h"

OutMessage::OutMessage(QString s, QString m, uint64_t i, QString l) :
    Message(m, i), status(s), lastStatusChange(l)
{}
OutMessage::~OutMessage() {}

const QString& OutMessage::getType() const
{
    return Message::TYPE_OUT;
}

QString OutMessage::getStatus() const
{
    return status;
}

QString OutMessage::getLastStatusChange() const
{
    return lastStatusChange;
}
