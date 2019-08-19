#include "message.h"

Message::Message(QString fromStr, QString messageStr, uint64_t i) :
    from(fromStr), message(messageStr), index(i)
{
}

QString Message::getFrom() const
{
    return from;
}

QString Message::getMessage() const
{
    return message;
}

uint64_t Message::getIndex() const
{
    return index;
}

void Message::setFrom(QString f)
{
    from = f;
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
