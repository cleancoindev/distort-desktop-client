#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>
#include <QColor>

class Message
{
public:
    static QColor getInColour();
    static QColor getSentColour();
    static QColor getUnverifiedColour();
    static QColor getEnqueuedColour();

    Message(QString from, QString message, uint64_t index);
    QString getFrom() const;
    QString getMessage() const;
    uint64_t getIndex() const;

    void setFrom(QString from);
    void setMessage(QString message);
    void setIndex(uint64_t index);

private:
    QString from;
    QString message;
    uint64_t index;
};

#endif // MESSAGE_H
