#include "message.h"

const QString Message::TYPE_IN("IN");
const QString Message::TYPE_OUT("OUT");

Message::Message(QString messageStr, uint64_t i) :
    message(messageStr), index(i)
{}

QString Message::getMessage() const
{
    return message;
}

uint64_t Message::getIndex() const
{
    return index;
}

void Message::setMessage(QString m)
{
    message = m;
}

void Message::setIndex(uint64_t i)
{
    index = i;
}

QColor Message::getInColour()
{
    return QColor("#d7e7ff");
}

QColor Message::getSentColour()
{
    return QColor("#ceffdb");
}

QColor Message::getUnverifiedColour()
{
    return QColor("#ff9900");
}

QColor Message::getEnqueuedColour()
{
    return QColor("#c4cec9");
}
